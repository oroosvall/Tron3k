#include "Core.h"

void Core::init()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, false);

	win = glfwCreateWindow(
		200, 200, "ASUM PROJECT", NULL, NULL);

	Input* i = Input::getInput();
	i->setupCallbacks(win);

	glfwShowWindow(win);

	//musicPlayer.playMusic(1234);	// **** TEMP ****

	current = Gamestate::START;

}

void Core::update(float dt)
{
	//update I/O

	glfwPollEvents();

	console.update();

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
	i->clearOnRelease();
}

void Core::upStart(float dt)
{
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
	//ROAM
	//load
	//run

	//return MENU
}

void Core::upClient(float dt)
{
	//CLIENT
	//connect
	//map valid ?
	//load
	//get full server info
	//run
	//network in / out
	//spectator
	//pick team
	//wait for server start event

	//return MENU


	switch (subState)
	{
	case 0: //create client object

		if (top)
			delete top;
		top = new Client();
		top->init();

		//fill the server address and port to use

		subState = 1;
		break;
	case 1: //attempt to connect

		if (top->new_connection())
		{
			//success
		}
		else
		{
			current = MENU;	
			subState = 0;
			delete top;		
			top = 0;
			return;
		}

		break;
	case 2: //map valid check, load

		//wait for map reply

		//can i load?
		// if not  -> menu
		

		subState = 3;
		break;
	case 3: //get full server info

		break;
	case 4: //main client loop


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

		//load map

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
