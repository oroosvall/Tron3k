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
	winX = winY = 200;
	
	createWindow(winX, winY, fullscreen);

	musicPlayer.enableSounds(false); // **** TEMP ****
	musicPlayer.playMusic(1234);

	current = Gamestate::START;
	tick_timer = 0;

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
				console.printMsg("Error: failed to set pipeline setting: VIEWPORT");
			}
		}
	}

	glfwPollEvents();
	stringstream name;
	name << "temp ";
	if (top != nullptr)
		name << to_string(top->getConId());
	console.update(name.str(), 'A'); //Someone wrote a command


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
		if (renderPipe)
		{
			renderPipe->update();
			renderPipe->render();
		}

		//update ui & sound
		//update renderPipeline
	}

	Input* i = Input::getInput();
	i->clearOnPress();
	console.discardCommandAndLastMsg();

	glfwSwapBuffers(win);

	
}

void Core::upStart(float dt)
{
	switch (subState)
	{
	case 0:
		printf("[/1] Client \n");
		printf("[/2] Server \n");
		printf("[/3] Roam \n");
		subState++;
		break;

	case 1:
		if (console.commandReady())
		{
			string cmd = console.getCommand();
			if (cmd == "/1")
			{
				current = Gamestate::CLIENT;
				subState = 0;
				initPipeline();
			}

			else if (cmd == "/2")
			{
				current = Gamestate::SERVER;
				subState = 0;
			}

			else if (cmd == "/3")
			{
				current = Gamestate::ROAM;
				subState = 0;
				initPipeline();
			}
		}

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
		top->init();

		subState++;
		break;
	case 1: //fill the server address and port to use

		//todo

		subState++;
		break;
		
	case 2: //attempt to connect

		for (int n = 0; n < 3; n++)
		{
			console.printMsg("Connecting...", "Server", 'S');
			if (top->new_connection())
			{
				console.printMsg("Connecting Succsessfull", "Server", 'S');
				//send "new connection" event to server
				top->new_connection_packet();
				subState++;
				return;
			}
		}

		console.printMsg("Connection Failed", "Server", 'S');
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
			subState++;
		}
		
		break;
	case 4: //main client loop

		//fetch new network data
		top->network_IN(dt);

		if (console.messageReady())
		{
			top->msg_out = console.getMessage();
			top->scope_out = Uint8(ALL);
		}

		tick_timer += dt;
		if (tick_timer > tick)
		{
			top->network_OUT(dt);

			if (top->msg_in != "")
			{
				stringstream name;
				name << "temp ";
				name << to_string(top->conID_in);
				console.printMsg(top->msg_in, name.str(), top->scope_in);
				top->msg_in = "";
			}
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
		top->init();
		
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
			console.printMsg("Port bound", "Server", 'S');
		}
		else
		{
			console.printMsg("Port bind failed", "Server", 'S');
			subState = 1;
			return;
		}
		
		//load map

		subState = 3;
		break;
	case 3: //main server loop

		//get data
		top->network_IN(dt);

		//update game

		if (console.messageReady())
		{
			top->msg_out = console.getMessage();
			top->scope_out = Uint8(ALL);
		}

		tick_timer += dt;
		if (tick_timer > tick)
		{
			//look for clients
			top->new_connection();

			//network out
			top->network_OUT(dt);
			tick_timer = 0;

			if (top->msg_in != "")
			{
				stringstream name;
				name << "temp ";
				name << to_string(top->conID_in);
				console.printMsg(top->msg_in, name.str(), top->scope_in);
				top->msg_in = "";
			}
		}
		
		break;
	default:
		break;
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
	if (!renderPipe->init())
	{
		console.printMsg("Error: Pipeline could not be created!");
		renderPipe->release();
		renderPipe = nullptr;
	}
	else
	{

		PipelineValues pv;
		pv.type = pv.INT2;
		pv.xy[0] = winX;
		pv.xy[1] = winY;
		if (!renderPipe->setSetting(PIPELINE_SETTINGS::VIEWPORT, pv))
		{
			console.printMsg("Error: Failed to set pipeline setting: VIEWPORT");
		}
	}
}