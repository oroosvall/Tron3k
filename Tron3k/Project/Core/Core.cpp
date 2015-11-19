#include "Core.h"

void Core::init()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, false);

	//glfwWindowHint(GLFW_DECORATED, false); borderless toggle

	win = nullptr;
	renderPipe = nullptr;

	recreate = false;
	fullscreen = false;
	winX = winY = 800;
	
	createWindow(winX, winY, fullscreen);

	//******************* TEMP *************************
	theSound = SoundPlayer::getSound();
	theSound->init();
	theSound->playMusic(MUSIC::mainMenu);
	theSound->enableSounds();
	timepass = 0.0f;
	//**************************************************
	current = Gamestate::START;
	tick_timer = 0;
}

Core::~Core()
{
	if (game != nullptr)
		game->release();
	if (top != nullptr)
		delete top;
	if (win != nullptr)
	{
		glfwDestroyWindow(win);
		win = nullptr;
	}
	if (renderPipe != nullptr)
		renderPipe->release();
	
	SoundPlayer* theSound = SoundPlayer::getSound();
	theSound->release();

	Input* i = Input::getInput();
	i->release();

	CameraInput* cam = CameraInput::getCam();
	cam->release();

	saveSettings();
}

void Core::update(float dt)
{
	//update I/O
	if (recreate)
	{
		createWindow(winX, winY, fullscreen);

		if (renderPipe)
		{
			PipelineValues pv;
			pv.type = pv.INT2;
			pv.xy[0] = winX;
			pv.xy[1] = winY;
			renderPipe->setSetting(PIPELINE_SETTINGS::VIEWPORT, pv);
			if (!renderPipe->setSetting(PIPELINE_SETTINGS::VIEWPORT, pv))
			{
				console.printMsg("Error: failed to set pipeline setting: VIEWPORT", "System", 'S');
			}

		}
	}

	glfwPollEvents();
	string name = "Temp";
	if (top != nullptr)
	{
		if (game != nullptr)
		{
			if (top->is_client())
			{
				Player* p = game->getPlayer(top->getConId());
				if (p != nullptr)
					name = p->getName();
			}
			else
				name = "Server";
		}
	}
	console.update(name, 'A'); //Updates to check for new messages and commands


	switch (current)
	{
	case START:		upStart(dt);	break;
	case MENU:		upMenu(dt);		break;
	case ROAM:		upRoam(dt);		break;
	case CLIENT:	upClient(dt);	break;
	case SERVER:	upServer(dt);	break;
	default:						break;
	}

	if (current != START && current != SERVER)
	{
		if (game)
		{
			if (renderPipe)
			{
				renderPipe->update();
				renderPipe->renderIni(); 
				
				//render players
				for (size_t i = 0; i < MAX_CONNECT; i++)
				{
					Player* p = game->getPlayer(i);
					if (p)
					{
						renderPipe->renderPlayer(0, p->getWorldMat());
					}
				}
				for (int c = 0; c < BULLET_TYPE::NROFBULLETS; c++)
				{
					std::vector<Bullet*> bullets = game->getBullets(BULLET_TYPE(c));
					for (int i = 0; i < bullets.size(); i++)
					{
						renderPipe->renderPlayer(0, bullets[i]->getWorldMat());
					}
				}

				renderPipe->render();
			}

		}

		
		//update ui & sound
		//update renderPipeline
	}

	//*******TEMP**********
	theSound->update();
	//*********************

	//shouldnt get the ref every frame
	Input* i = Input::getInput();
	i->clearOnPress();
	console.discardCommandAndLastMsg();

	glfwSwapBuffers(win);

	//TEMPORARY
	static bool given = false;
	if (game != nullptr && !given)
	{
		givePlayerBoatExtremes();
		given = true;
	}
}

void Core::upStart(float dt)
{
	switch (subState)
	{
	case 0:
		console.printMsg("[/1] Client", "System", 'S');
		console.printMsg("[/2] Server", "System", 'S');
		console.printMsg("[/3] Roam", "System", 'S');

		loadSettings();
		subState++;
		break;

	case 1:
		//start console commands
		startHandleCmds(dt);
		break;
	}
}

void Core::upMenu(float dt)
{
	//MENU

	//menu opitons

	//roam, choose map -> ROAM
	//multiplayer, enter server IP -> CLIENT
	//settings
	//exit
	
	//return exit
}

void Core::upRoam(float dt)
{
	//ROAM
	//load
	//run

	game->update(dt);

	//return MENU
}

void Core::upClient(float dt)
{
	switch (subState)
	{
	case 0: //setup client object

		if (top)
			delete top;
		top = new Client();
		top->init(&console, _port, _addrs);

		subState++;
		break;
	case 1: //fill the server address and port to use

		//todo

		subState++;
		break;
		
	case 2: //attempt to connect

		for (int n = 0; n < 3; n++)
		{
			console.printMsg("Connecting...", "System", 'S');
			if (top->new_connection())
			{
				console.printMsg("Connecting Successful", "System", 'S');
				//send "new connection" event to server
				top->new_connection_packet(_name);
				
				if (game != nullptr)
					delete game;
				game = new Game();
				game->init(MAX_CONNECT, current);
				top->setGamePtr(game);
				subState++;
				return;
			}
		}

		console.printMsg("Connection Failed", "System", 'S');
		current = START;
		subState = 0;
		delete top;
		top = 0;
		return;

		break;
	case 3: //get full server info & map check

		top->network_IN(dt);

		if (top->firstPackageRecieved()) 
		{
			//can i load?

			// if not  -> menu
			//top->frame_name_change(top->getConId(), _name);
			Player* me = game->getPlayer(top->getConId());
			me->setName(_name);
			subState++;
		}
		
		break;
	case 4: //main client loop


		game->update(dt);
		Player* local = game->getPlayer(top->getConId());

	

		//fetch new network data
		if (top->network_IN(dt) == false)
		{
			disconnect();
			return;
		}

		if (console.messageReady())
		{
			top->msg_out = console.getMessage();
			top->scope_out = Uint8(ALL);
		}

		clientHandleCmds(dt);
		if (top == nullptr)//check for disconnected command
			return;

		tick_timer += dt;
		if (tick_timer > tick)
		{
			/*
			Fetch current player position
			Add to topology packet
			*/
			glm::vec3 lPos = local->getPos();
			glm::vec3 lDir = local->getDir();

			top->frame_pos(top->getConId(), lPos, lDir);

			if (game->weaponSwitchReady())
			{
				WEAPON_TYPE ws = game->getWpnSwitch();
				top->frame_weapon_switch(top->getConId(), ws);
			}

			if (game->fireEventReady())
			{
				WEAPON_TYPE wt = game->getLatestWeaponFired(top->getConId());
				int team = local->getTeam();
				top->frame_fire(wt, team, lPos, lDir);
			}

			top->network_OUT(dt);

			tick_timer = 0;
		}
		break;
	}
}

void Core::upServer(float dt)
{
	switch (subState)
	{
	case 0:  //create server object

		if (top)
			delete top;
		top = new Server();
		top->init(&console, _port, _addrs);

		subState = 1;
		break;
	case 1: //configure port & select map

			//TODO

		subState = 2;
		break;
	case 2: //atempting mapload & bind

			//bind port
		if (top->bind())
		{
			console.printMsg("Port bound", "System", 'S');
		}
		else
		{
			console.printMsg("Port bind failed", "System", 'S');
			subState = 1;
			return;
		}

		if (game != nullptr)
			delete game;
		game = new Game();
		game->init(MAX_CONNECT, current);

		top->setGamePtr(game);

		//load map

		subState = 3;
		break;
	case 3: //main server loop

			//get data
		top->network_IN(dt);

		//update game
		game->update(dt);

		if (console.messageReady())
		{
			top->msg_out = console.getMessage();
			top->scope_out = Uint8(ALL);
		}

		serverHandleCmds(dt);
		if (top == nullptr)
			return;

		tick_timer += dt;
		if (tick_timer > tick)
		{
			//look for clients
			top->new_connection();

			//network out
			top->network_OUT(dt);
			tick_timer = 0;
		}

		break;
	default:
		break;
	}
}

void Core::startHandleCmds(float dt)
{
	if (console.commandReady())
	{
		string token;
		istringstream ss = istringstream(console.getCommand());
		ss >> token;
		if (token == "/help")
		{
			console.printMsg("Console comands", "", ' ');
			console.printMsg("/name " + _name, "", ' ');
			console.printMsg("/ip " + _addrs.toString(), "", ' ');
			console.printMsg("/port " + to_string(_port), "", ' ');
		}
		else if (token == "/name")
		{
			ss >> token;

			if (token == "/name") //Same token = no more found
				console.printMsg("No name found. Use /name <new Name>", "System", 'S');
			else
			{
				/* Todo: Check for illegal names */
					_name = token;
					console.printMsg("You changed name to (" + token + ")", "System", 'S');
			}
		}
		else if (token == "/ip")
		{
			ss >> token;
			_addrs = IpAddress(token);
			console.printMsg("IP address set to: " + _addrs.toString(), "System", 'S');

		}
		else if (token == "/port")
		{
			ss >> token;
			if(token == "/port")
				console.printMsg("No port found. Use /port <new Port>", "System", 'S');
			else
			{
				_port = atoi(token.c_str());
				console.printMsg("Port set to: " + to_string(_port), "System", 'S');
			}
		}
		else if (token == "/1")
		{
			current = Gamestate::CLIENT;
			subState = 0;
			initPipeline();
		}

		else if (token == "/2")
		{
			current = Gamestate::SERVER;
			subState = 0;
		}

		else if (token == "/3")
		{
			current = Gamestate::ROAM;
			subState = 0;
			initPipeline();

			game = new Game();
			game->init(MAX_CONNECT, current);

			Player* p = new Player();
			
			p->init("Roam", glm::vec3(0, 0, 0));
			
			game->createPlayer(p, 0, true);
			
			delete p;
		}
	}
}

void Core::clientHandleCmds(float dt)
{
	if (console.commandReady())
	{
		string token;
		istringstream ss = istringstream(console.getCommand());
		ss >> token;
		if (token == "/help")
		{
			console.printMsg("Console comands", "", ' ');
			console.printMsg("/name " + _name, "", ' ');
			console.printMsg("/team " + to_string(game->getPlayer(top->getConId())->getTeam()), "", ' ');
			console.printMsg("/disconnect", "", ' ');
		}
		else if (token == "/name")
		{
			ss >> token;
			if (token == "/name")
				console.printMsg("No name found. Use /name <new Name>", "System", 'S');
			else
			{
				/* Todo: Check for illegal names */
				Player* me = game->getPlayer(top->getConId());
				
				me->setName(token);
				_name = token;
				console.printMsg("You changed name to (" + token + ")", "System", 'S');
				//send new name
				top->frame_name_change(top->getConId(), token);
			}
		}
		else if (token == "/team")
		{
			ss >> token;
			if(token != "/team" || token == "0" || token == "1" || token == "2")
			{
				int team = stoi(token);
				top->command_team_change(top->getConId(), team);
				console.printMsg("Change team request sent to server", "System", 'S');
			}
			else
				console.printMsg("Invalid team. Use /team <1/2/3>", "System", 'S');
		}
		else if (token == "/disconnect")
			disconnect();
	}
}

void Core::serverHandleCmds(float dt)
{
	if (console.commandReady())
	{
		string token;
		istringstream ss = istringstream(console.getCommand());
		ss >> token;
		if (token == "/help")
		{
			console.printMsg("Console comands", "", ' ');
			console.printMsg("/disconnect", "", ' ');
		}
		else if (token == "/disconnect")
			disconnect();
	}
}

void Core::saveSettings()
{
	fstream file;
	file.open("settings.txt", fstream::trunc | fstream::out);

	if (file.is_open())
	{
		file << "Name: " << _name << endl;
		file << "IP: " << _addrs.toString() << endl;
		file << "Port: " << _port;
	}
}

void Core::loadSettings()
{
	fstream file("settings.txt");

	//default values
	_name = "ClientName";
	_port = PORT_DEFAULT;
	_addrs = IpAddress::LocalHost;

	if (file.is_open())
	{
		string in;
		string in2;
		while (getline(file, in))
		{
			stringstream ss(in);
			ss >> in;
			ss >> in2;
			if (in == "Name:")
				_name = in2;
			else if (in == "IP:")
				_addrs = IpAddress(in2);
			else if (in == "Port:")
				_port = atoi(in2.c_str());
		}
	}
	else
	{
		saveSettings(); //save file with default values
	}

}

void Core::createWindow(int x, int y, bool fullscreen)
{
	if (win != 0)
	{
		removeWindow();
	}
	if (!fullscreen)
	{
		win = glfwCreateWindow(
			x, y, "ASUM PROJECT", NULL, NULL);
	}
	else
	{
		win = glfwCreateWindow(
			x, y, "ASUM PROJECT", glfwGetPrimaryMonitor(), NULL);
	}

	//set vsync off
	glfwSwapInterval(0);

	Input* i = Input::getInput();
	i->setupCallbacks(win);

	glfwShowWindow(win);

	glfwMakeContextCurrent(win);

	recreate = false;
}

void Core::removeWindow()
{
	glfwMakeContextCurrent(NULL);
	glfwHideWindow(win);
	glfwDestroyWindow(win);
	win = nullptr;
}

void Core::initPipeline()
{
	renderPipe = CreatePipeline();
	if (!renderPipe->init(winX, winY))
	{
		console.printMsg("Error: Pipeline could not be created!", "System", 'S');
		renderPipe->release();
		renderPipe = nullptr;
	}
	else
	{

		PipelineValues pv;
		pv.type = pv.INT2;
		pv.xy[0] = winX;
		pv.xy[1] = winY;

		CameraInput* cam = CameraInput::getCam();
		cam->init((glm::mat4*)renderPipe->getView());

		if (!renderPipe->setSetting(PIPELINE_SETTINGS::VIEWPORT, pv))
		{
			console.printMsg("Error: Failed to set pipeline setting: VIEWPORT", "System", 'S');
		}
	}
}

void Core::setfps(int fps)
{
	if(win != nullptr)
		glfwSetWindowTitle(win, to_string(fps).c_str());
}

//TEMPORARY
void Core::givePlayerBoatExtremes()
{
	if (renderPipe != nullptr)
	{
		//to avoid leaks
		vec3* minExtremes = (vec3*)renderPipe->getMinExtremes();
		vec3* maxExtremes = (vec3*)renderPipe->getMaxExtremes();

		vec3 minEx = *minExtremes;
		vec3 maxEx = *maxExtremes;

		delete minExtremes;
		delete maxExtremes;

		game->getBoatCoordsFromCore(minEx, maxEx);
	}
	else
	game->getBoatCoordsFromCore(vec3(-1,-1,-1), vec3(1,1,1));
}

bool Core::windowVisible() const
{
	return !glfwWindowShouldClose(win);
}

void Core::disconnect()
{
	saveSettings();
	delete top;
	top = nullptr;
	game->release();
	game = nullptr;
	if (renderPipe != nullptr) //server might not have a renderpipe
	{
		renderPipe->release();
		renderPipe = nullptr;
	}
	current = Gamestate::START;
	subState = 0;
}
