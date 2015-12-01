#include "Core.h"

void Core::init()
{
	glfwInit();
	i = Input::getInput();

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

	serverRender = false;

	timepass = 0.0f;
	
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

	ReleaseSound();

	Input* i = Input::getInput();
	i->release();

	CameraInput* cam = CameraInput::getCam();
	cam->release();

	saveSettings();

	glfwTerminate();

}

void Core::update(float dt)
{
	if (recreate)
		createWindow(winX, winY, fullscreen);

	glfwPollEvents();
	console.update(_name, 'A'); //Updates to check for new messages and commands

	switch (current)
	{
	case START:		upStart(dt);	break;
	case MENU:		upMenu(dt);		break;
	case ROAM:		upRoam(dt);		break;
	case CLIENT:	upClient(dt);	break;
	case SERVER:	upServer(dt);	break;
	default:						break;
	}

	i->clearOnPress();
	console.discardCommandAndLastMsg();

	glfwSwapBuffers(win);

	//TEMPORARY
	static bool given = false;
	if (game != nullptr && !given)
	{
		sendPlayerBox();
		sendWorldBoxes();
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
		startHandleCmds();
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
	switch (subState)
	{
	case 0: //init
	{
		initPipeline();

		game = new Game();
		game->init(MAX_CONNECT, current);

		Player* p = new Player();
		p->init("Roam", glm::vec3(0, 0, 0));
		game->createPlayer(p, 0, true);
		delete p;
		subState++;
		break;
	}
	case 1:
		subState++;
		break;
	case 2: //main loop

		/*roamHandleCmds();
		game->update(dt);
		renderWorld(dt);
		break;*/

		//Command and message handle
		roamHandleCmds();

		//update game
		game->update(dt);

		GetSound()->setLocalPlayerDir(game->getPlayer(0)->getDir());
		GetSound()->setLocalPlayerPos(game->getPlayer(0)->getPos());

		if (game->playerWantsToRespawn())
		{
			game->allowPlayerRespawn(0, glm::vec3(0, 0, 0));
		}

		if (game->weaponSwitchReady())
		{
			WEAPON_TYPE wt = game->getWpnSwitch();
			game->handleWeaponSwitch(0, wt);
		}

		if (game->fireEventReady())
		{
			Player* p = game->getPlayer(0);
			WEAPON_TYPE wt;
			int bID;
			game->getLatestWeaponFired(0, wt, bID);
			int team = 0;
			game->handleWeaponFire(0, bID, wt, p->getPos(), p->getDir());
		}

		renderWorld(dt);
		break;
	}
}

void Core::upClient(float dt)
{
	switch (subState)
	{
	case 0: //init
		initPipeline();
		if (top)
			delete top;
		top = new Client();
		top->init(&console, _port, _addrs);

		//attempt to connect
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
				return; //On sucsess
			}
		}
		//On Fail
		console.printMsg("Connection Failed", "System", 'S');
		current = START;
		subState = 0;
		delete top;
		top = 0;
		return;

		break;
	case 1: //get full server info & map check
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
	case 2: //main client loop

		//fetch new network data
		if (top->network_IN(dt) == false)
		{
			disconnect();
			return;
		}

		//update game
		game->update(dt);

		GetSound()->setLocalPlayerDir(game->getPlayer(top->getConId())->getDir());
		GetSound()->setLocalPlayerPos(game->getPlayer(0)->getPos());

		//Command and message handle
		if (console.messageReady())
		{
			top->msg_out = console.getMessage();
			top->scope_out = Uint8(ALL);
		}

		clientHandleCmds();
		if (top == nullptr)//check for disconnected command
			return;

		if (game->playerWantsToRespawn())
		{
			top->command_respawn(top->getConId());
		}

		//Frame tick timer. Network out
		tick_timer += dt;
		if (tick_timer > tick)
		{
			//Fetch current player position
			//Add to topology packet
			Player* local = game->getPlayer(top->getConId());
			top->frame_pos(top->getConId(), local->getPos(), local->getDir());

			if (game->weaponSwitchReady())
			{
				WEAPON_TYPE ws = game->getWpnSwitch();
				top->frame_weapon_switch(top->getConId(), ws);
			}

			if (game->fireEventReady())
			{
				WEAPON_TYPE wt;
				int bID;
				game->getLatestWeaponFired(top->getConId(), wt, bID);
				int team = local->getTeam();
				top->frame_fire(wt, top->getConId(), bID, local->getPos(), local->getDir());
			}
			//send the package
			top->network_OUT(dt);
			tick_timer = 0;
		}

		renderWorld(dt);
		break;
	}
}

void Core::upServer(float dt)
{
	switch (subState)
	{
	case 0:  //init server
	{
		initPipeline();
		//createWindow(200, 200, false);
		serverCam = CameraInput::getCam();
		if (top)
			delete top;
		top = new Server();
		top->init(&console, _port, _addrs);
		_name = "Server";

		//bind port
		if (top->bind())
			console.printMsg("Port bound", "System", 'S');
		else
		{
			console.printMsg("Port bind failed", "System", 'S');
			current = Gamestate::START;
			subState = 0;
			return;
		}

		//atempting mapload
		if (game != nullptr)
			delete game;
		game = new Game();
		game->init(MAX_CONNECT, current);
		game->freecam = true;
		//load map

		top->setGamePtr(game);

		subState++;
		break;
	}
	case 1: //main server loop
		//get data
		top->network_IN(dt);

		//update game
		game->update(dt);

		//handle commands and messages
		if (console.messageReady())
		{
			top->msg_out = console.getMessage();
			top->scope_out = Uint8(ALL);
		}

		if (game->hitEventReady())
		{
			BulletHitInfo hi = game->getHitInfo();
			int newHPtotal = game->handleBulletHitEvent(hi);
			top->event_bullet_hit(hi, newHPtotal);
		}

		serverHandleCmds();
		if (top == nullptr)
			return;

		//event packages out
		tick_timer += dt;
		if (tick_timer > tick)
		{
			//look for clients
			top->new_connection();

			//network out
			top->network_OUT(dt);
			tick_timer = 0;
		}

		if (serverRender)
		{
			if (console.getInChatMode() == false)
				serverCam->update(dt, game->freecam);
			renderWorld(dt);
		}
		break;
	}
}

void Core::startHandleCmds()
{
	if (console.commandReady())
	{
		string token;
		istringstream ss = istringstream(console.getCommand());
		ss >> token;
		if (token == "/help")
		{
			console.printMsg("Console commands", "", ' ');
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
			if (token == "/port")
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
		}
	}
}

void Core::roamHandleCmds()
{
	if (console.commandReady())
	{
		string token;
		istringstream ss = istringstream(console.getCommand());
		ss >> token;
		if (token == "/help")
		{
			console.printMsg("Console commands", "", ' ');
			console.printMsg("/name " + _name, "", ' ');
			console.printMsg("/players", "", ' ');
			console.printMsg("/free (turns freecam on/off)", "", ' ');
			console.printMsg("/spec <Number> (spectate player id)", "", ' ');
		}
		else if (token == "/name")
		{
			ss >> token;
			if (token == "/name")
				console.printMsg("No name found. Use /name <new Name>", "System", 'S');
			else
			{
				/* Todo: Check for illegal names */
				Player* me = game->getPlayer(0);

				me->setName(token);
				_name = token;
				console.printMsg("You changed name to (" + token + ")", "System", 'S');
			}
		}
		else if (token == "/team")
		{
			ss >> token;
			if (token != "/team" || token == "0" || token == "1" || token == "2")
			{
				int team = stoi(token);
				game->addPlayerToTeam(0, team);
				game->allowPlayerRespawn(0, vec3(0,0,0)); //Add new spawn point probably
				if (team != 0)
					game->freecam = false;
				else
					game->freecam = true;
			}
			else
				console.printMsg("Invalid team. Use /team <1/2/3>", "System", 'S');
		}
		else if (token == "/free")
		{
			if (game->freecam && game->getPlayer(0)->getTeam() != 0)
			{
				//set view dir pos back to the player's view REMEMBER Roam conID 0
				CameraInput::getCam()->setCam(game->getPlayer(0)->getPos(), game->getPlayer(0)->getDir());
				game->freecam = false;
			}
			else
				game->freecam = true;

			game->spectateID = -1;
		}
	}
}

void Core::clientHandleCmds()
{
	if (console.commandReady())
	{
		string token;
		istringstream ss = istringstream(console.getCommand());
		ss >> token;
		if (token == "/help")
		{
			console.printMsg("Console commands", "", ' ');
			console.printMsg("/name " + _name, "", ' ');
			console.printMsg("/team " + to_string(game->getPlayer(top->getConId())->getTeam()), "", ' ');
			console.printMsg("/players", "", ' ');
			console.printMsg("/disconnect", "", ' ');
			console.printMsg("/free (turns freecam on/off)", "", ' ');
			console.printMsg("/spec # (spectate player id)", "", ' ');
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
			if (token != "/team" || token == "0" || token == "1" || token == "2")
			{
				int team = stoi(token);
				top->command_team_change(top->getConId(), team);
				console.printMsg("Change team request sent to server", "System", 'S');
			}
			else
				console.printMsg("Invalid team. Use /team <1/2/3>", "System", 'S');
		}
		else if (token == "/players")
		{
			for (int n = 0; n < MAX_CONNECT; n++)
			{
				Player* p = game->getPlayer(n);
				if (p != nullptr)
				{
					string print;
					print += "ConID: " + to_string(n) + " Team: " + to_string(p->getTeam()) + " Name: " + p->getName();
					console.printMsg(print, "", ' ');
				}
			}
		}
		else if (token == "/disconnect")
			disconnect();
		else if (token == "/free")
		{
			if (game->freecam)
			{
				if (game->getPlayer(top->getConId())->getTeam() != 0)
				{
					//set view dir pos back to the player's view REMEMBER Roam conID 0
					CameraInput::getCam()->setCam(game->getPlayer(top->getConId())->getPos(), game->getPlayer(top->getConId())->getDir());
					game->freecam = false;
				}
			}
			else
				game->freecam = true;

			game->spectateID = -1;
		}
		else if (token == "/spec")
		{
			ss >> token;
			if (token == "/spec")
				console.printMsg("Invalid Number", "System", 'S');
			else
			{
				int id = stoi(token); // yes this is dangerous, dont write anything but numbers
				if (id > -1 && id < MAX_CONNECT)
				{
					//set view dir pos back to the player's view
					Player* p = game->getPlayer(id);
					if (p != nullptr)
					{
						CameraInput::getCam()->setCam(p->getPos(), p->getDir());
						game->spectateID = id;
						game->freecam = true;
					}
					else
						console.printMsg("That Player Doesn't exist", "System", 'S');
				}
				else
					game->spectateID = -1;
			}
		}
	}
}

void Core::serverHandleCmds()
{
	if (console.commandReady())
	{
		string token;
		istringstream ss = istringstream(console.getCommand());
		ss >> token;
		if (token == "/help")
		{
			console.printMsg("Console commands", "", ' ');
			console.printMsg("/players", "", ' ');
			console.printMsg("/disconnect", "", ' ');
			console.printMsg("/render", "", ' ');
			console.printMsg("/spec #", "", ' ');
		}
		else if (token == "/players")
		{
			for (int n = 0; n < MAX_CONNECT; n++)
			{
				Player* p = game->getPlayer(n);
				if (p != nullptr)
				{
					string print;
					print += "ConID: " + to_string(n) + " Team: " + to_string(p->getTeam()) + " Name: " + p->getName();
					console.printMsg(print, "", ' ');
				}
			}
		}
		else if (token == "/disconnect")
			disconnect();
		else if (token == "/render")
		{
			if (serverRender)
				serverRender = false; //createWindow(200, 200, false);
			else
				serverRender = true; //createWindow(winX, winY, fullscreen);
		}
		else if (token == "/spec")
		{
			ss >> token;
			if (token == "/spec")
				console.printMsg("Invalid Number", "System", 'S');
			else
			{
				int id = stoi(token); // yes this is dangerous, dont write anything but numbers
				if (id > -1 && id < MAX_CONNECT)
				{
					//set view dir pos back to the player's view
					Player* p = game->getPlayer(id);
					if (p != nullptr)
					{
						CameraInput::getCam()->setCam(p->getPos(), p->getDir());
						game->spectateID = id;
						game->freecam = true;
					}
					else
						console.printMsg("That Player Doesn't exist", "System", 'S');
				}
				else
					game->spectateID = -1;
			}
		}
	}
}

void Core::saveSettings()
{
	fstream file;
	file.open("GameFiles/Config/player.ini", fstream::trunc | fstream::out);

	if (file.is_open())
	{
		if (_name == "Server")
			file << "Name: ClientName" << endl;
		else
			file << "Name: " << _name << endl;
		file << "IP: " << _addrs.toString() << endl;
		file << "Port: " << _port;
	}
}

void Core::loadSettings()
{
	fstream file("GameFiles/Config/player.ini");

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

void Core::renderWorld(float dt)
{
	if (renderPipe && game)
	{
		glm::vec3 tmpEyePos = CameraInput::getCam()->getPos();
		renderPipe->update(tmpEyePos.x, tmpEyePos.y, tmpEyePos.z, dt); // sets the view/proj matrix
		renderPipe->renderIni();


		float dgColor[3];
		//render skybox
		dgColor[0] = 0; dgColor[1] = 0; dgColor[2] = 0;
		renderPipe->renderPlayer(1, (void*)&(CameraInput::getCam()->getSkyboxMat()), dgColor, 0.0f);

		//send all lights
		bool firstLight = true;
		for (size_t i = 0; i < MAX_CONNECT; i++)
		{
			Player* p = game->getPlayer(i);
			if (p)
			{
				SpotLight light;
				light.Position = p->getPos();
				light.Direction = p->getDir();
				if (firstLight)
				{
					light.AmbientIntensity = 0.0;
					firstLight = false;
				}
				renderPipe->addLight(&light);
			}
		}

		//render players
		for (size_t i = 0; i < MAX_CONNECT; i++)
		{
			Player* p = game->getPlayer(i);
			if (p)
			{
				if (p->getTeam() != 0) //Don't render spectators!
				{
					if (p->getHP() <= 0) { // set red
						dgColor[0] = 1; dgColor[1] = 0; dgColor[2] = 0;
					}
					else if (p->getTeam() == 1) { //team 1 color
						dgColor[0] = 0; dgColor[1] = 1; dgColor[2] = 0;
					}
					else if (p->getTeam() == 2) { // team 2 color
						dgColor[0] = 0.2f; dgColor[1] = 0.2f; dgColor[2] = 1;
					}
					else if (p->getTeam() == 0) { // spectate color
						dgColor[0] = 0; dgColor[1] = 0; dgColor[2] = 0;
					}
					//static intense based on health
					float hpval = float(p->getHP()) / 130.0f;

					renderPipe->renderPlayer(0, p->getWorldMat(), dgColor, hpval);
				}
			}
		}
		for (int c = 0; c < BULLET_TYPE::NROFBULLETS; c++)
		{
			dgColor[0] = 0; dgColor[1] = 0; dgColor[2] = 0;

			std::vector<Bullet*> bullets = game->getBullets(BULLET_TYPE(c));
			for (int i = 0; i < bullets.size(); i++)
			{
				renderPipe->renderPlayer(2, bullets[i]->getWorldMat(), dgColor, 1.0f);
			}
		}
		renderPipe->render();
	}
}

void Core::createWindow(int x, int y, bool fullscreen)
{
	if (win != 0)
		removeWindow();
	if (!fullscreen)
		win = glfwCreateWindow(
			x, y, "ASUM PROJECT", NULL, NULL);
	else
		win = glfwCreateWindow(
			x, y, "ASUM PROJECT", glfwGetPrimaryMonitor(), NULL);

	//set vsync off
	glfwSwapInterval(0);

	i->setupCallbacks(win);

	glfwShowWindow(win);

	glfwMakeContextCurrent(win);

	if (renderPipe)
	{
		PipelineValues pv;
		pv.type = pv.INT2;
		pv.xy[0] = winX;
		pv.xy[1] = winY;
		renderPipe->setSetting(PIPELINE_SETTINGS::VIEWPORT, pv);
		if (!renderPipe->setSetting(PIPELINE_SETTINGS::VIEWPORT, pv))
			console.printMsg("Error: failed to set pipeline setting: VIEWPORT", "System", 'S');
	}
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
	if (!renderPipe)
	{
		renderPipe = CreatePipeline();
	}
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
	if (win != nullptr)
		glfwSetWindowTitle(win, to_string(fps).c_str());
}

void Core::sendPlayerBox()
{
	std::vector<float> pBox;
	float xMax, xMin, yMax, yMin, zMax, zMin;
	xMax = 0.5f;
	xMin = -0.5f;
	yMax = 1.0f;
	yMin = -1.0f;
	zMax = 0.2f;
	zMin = -0.2f;

	if (renderPipe != nullptr)
	{
		renderPipe->getPlayerBox(xMax, xMin, yMax, yMin, zMax, zMin);
		
	}

	pBox.push_back(xMax);
	pBox.push_back(xMin);
	pBox.push_back(yMax);
	pBox.push_back(yMin);
	pBox.push_back(zMax);
	pBox.push_back(zMin);

	game->sendPlayerBox(pBox);
}

void Core::sendWorldBoxes()
{
	if (renderPipe != nullptr)
	{
		int size = renderPipe->getNrOfWorldBoxes();

		float xMax, yMax, zMax;
		float xMin, yMin, zMin;
		xMax = yMax = zMax = xMin = yMin = zMin = 0;
		std::vector<std::vector<float>> worldBoxes;
		std::vector<float> temp;
		for (int i = 0; i < size; i++)
		{
			//temp.clear();
			renderPipe->getWorldBoxes(i, xMax, xMin, yMax, yMin, zMax, zMin);
			temp.push_back(xMax);
			temp.push_back(xMin);
			temp.push_back(yMax);
			temp.push_back(yMin);
			temp.push_back(zMax);
			temp.push_back(zMin);

			worldBoxes.push_back(temp);
			temp.clear();
		}
		//Jag hatar .dll-er
		game->sendWorldBoxes(worldBoxes);
	}
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
