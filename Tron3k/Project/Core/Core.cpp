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
		100, 100, "ASUM PROJECT", NULL, NULL);

	Input* i = Input::getInput();
	i->setupCallbacks(win);

	glfwShowWindow(win);

	//musicPlayer.playMusic(1234);	// **** TEMP ****

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
}

void Core::upServer(float dt)
{
	//SERVER
	//bind port ?
	//pick map
	//load map
	//run
	//look for clinets
	//network in / out

	//return START
}
