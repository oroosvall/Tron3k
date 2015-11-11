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
	engine = nullptr;

	recreate = false;
	fullscreen = false;
	winX = winY = 200;
	
	createWindow(winX, winY, fullscreen);

	//musicPlayer.playMusic(1234);	// **** TEMP ****

	current = Gamestate::ROAM;

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

	recreate = false;
}

void Core::removeWindow()
{
	glfwHideWindow(win);
	glfwDestroyWindow(win);
	win = nullptr;
}

void Core::update(float dt)
{
	//update I/O
	if (recreate)
	{
		createWindow(winX, winY, fullscreen);
	}

	glfwPollEvents();

	if (console.update()) //Someone wrote a command
	{
		string cmd = console.getCommand();
		console.printMsg("Someone just wrote a command line! It was:");
		console.printMsg(cmd);
	}

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
		//update ui & sound
		//update engine
	}

	Input* i = Input::getInput();
	i->clearOnPress();

	glfwSwapBuffers(win);

	
}

void Core::upStart(float dt)
{
	printf("[1] Client \n");
	printf("[2] Server \n");
	printf("[3] Roam \n");
	
	Input* i = Input::getInput();

	//START
	//server -> SERVER
	//client -> MENU

	//return exit

	//current = Gamestate::CLIENT;
	current = Gamestate::SERVER;
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
	Input* i = Input::getInput();

	if (i->justPressed(GLFW_KEY_0))
	{
		fullscreen = !fullscreen;
		recreate = true;
	}

	if (i->justPressed(GLFW_KEY_9))
	{
		winX = 720;
		winY = 640;
		recreate = true;
	}

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

		subState = 1;
		break;
	case 1: //fill the server address and port to use

		//todo

		subState = 2;
		break;
		
	case 2: //attempt to connect

		for (int n = 0; n < 3; n++)
		{
			//connecting...
			if (top->new_connection())
			{
				//success
				subState = 3;
				return;
			}
		}
		//failed
		current = MENU;
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
			subState = 4;
		}
		
		break;
	case 4: //main client loop

		top->network_IN(dt);
		top->network_OUT(dt);

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
			//successful bind
		}
		else
		{
			subState = 1;
			return;
		}
		
		//load map

		subState = 3;
		break;
	case 3: //main server loop

		//look for clients
		top->network_IN(dt);

		//update game

		//network out
		top->network_OUT(dt);
		break;
	default:
		break;
	}
}
