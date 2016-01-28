#include "Core.h"

void Core::init()
{
	uitmpcounter = 0;

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

	cursorVisible = true;
	recreate = false;
	fullscreen = false;
	winX = winY = 800;
	//winX = winY = 1000;
	winX = 1280; winY = 720;
	//winX = 1920, winY = 1080;

	createWindow(winX, winY, fullscreen);

	serverRender = false;

	timepass = 0.0f;
	
	current = Gamestate::START;
	tick_timer = 0;
	client_record = false;
	client_playback = false;

	uiManager = new UIManager();
	initPipeline();
	uiManager->init(&console);
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
	if (uiManager != nullptr)
		delete uiManager;
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

	bool chatMode = console.getInChatMode();

	if (cursorVisible != chatMode)
	{
		cursorVisible = chatMode;
		if (chatMode)
			glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	glfwPollEvents();
	console.update(_name, 'A'); //Updates to check for new messages and commands
	
	if (game)
	{
		if (console.getInChatMode() == false)
		{
			if (i->justPressed(GLFW_KEY_7))
			{
				playbackSpeed *= 2.0f;
				if (playbackSpeed > 16.0f)
					playbackSpeed = 16.0f;
				CameraInput* cam = CameraInput::getCam();
				cam->setPlaybackSpeed(playbackSpeed);
			}
			if (i->justPressed(GLFW_KEY_6))
			{
				playbackSpeed *= 0.5f;
				if (playbackSpeed < 0.03125f)
					playbackSpeed = 0.03125f;
				CameraInput* cam = CameraInput::getCam();
				cam->setPlaybackSpeed(playbackSpeed);
			}
		}
	}

	dt *= playbackSpeed;

	switch (current)
	{
	case START:		upStart(dt);	break;
	case MENU:		upMenu(dt);		break;
	case ROAM:		upRoam(dt);		break;
	case CLIENT:	upClient(dt);	break;
	case SERVER:	upServer(dt);	break;
	default:						break;
	}

	if (renderPipe)
	{
		if (i->justPressed(GLFW_KEY_8))
		{
			renderPipe->reloadShaders();
		}
	}

	i->clearOnPress();
	console.discardCommandAndLastMsg();

	glfwSwapBuffers(win);
}

void Core::upStart(float dt)
{
	switch (subState)
	{
	case 0:
		console.printMsg("[/1] Client", "System", 'S');
		console.printMsg("[/2] Server", "System", 'S');
		console.printMsg("[/3] Roam", "System", 'S');
		console.printMsg("[/4] Client_Record", "System", 'S');
		console.printMsg("[/5] Client_Playback", "System", 'S');

		loadSettings();
		subState++;
		break;

	case 1:
		//start console commands
		startHandleCmds();
		//uiManager->render();
		break;
	}
}

void Core::upMenu(float dt)
{
	double x = (0.0);
	double y = (0.0);
	//Get mouse position
	i->getCursor(x, y);
	double tX = (x / (double)winX) * 2 - 1.0; // (x/ResolutionX) * 2 - 1
	double tY = (-y / (double)winY) * 2 + 1.0; // (y/ResolutionY) * 2 - 1

	uiManager->render();
	
	if (i->justPressed(GLFW_MOUSE_BUTTON_LEFT))//button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		int eventIndex = uiManager->collisionCheck(glm::vec2((float)tX, (float)tY));
		switch (eventIndex)
		{
		case 0: //Roam
			current = ROAM;
			uiManager->removeAllMenus();
			//Load gui and the rest of in game ui.
			subState = 0;
			break;
		case 1: //Multiplayer
			//current = CLIENT;
			//uiManager->removeAllMenus();
			//Load gui and the rest of in game ui.
			subState = 0;
			break;
		case 2: //Settings
			break;
		case 3: //Exit
			glfwHideWindow(win);
			break;
		default:
			break;
		}
	}
	else
		uiManager->hoverCheck(glm::vec2((float)tX, (float)tY));
}

void Core::upRoam(float dt)
{
	switch (subState)
	{
	case 0: //init
	{
		game = new Game();
		game->init(MAX_CONNECT, current, &console);
		game->sendPlayerRadSize(0.9f);
		//map loaded, fetch spawnpoints from render
		renderPipe->getSpawnpoints(*game->getSpawnpoints());
		// fetch all collision boxes
		bool allchunksSent = false;
		for (int i = 0; !allchunksSent; i++)
			allchunksSent = sendChunkBoxes(i);
		Player* p = new Player();
		p->init("Roam", glm::vec3(0, 0, 0));
		game->createPlayer(p, 0, 100, 0, true);
		game->freecam = true;
		delete p;
		subState++;
		break;
	}
	case 1:
		subState++;
		break;
	case 2: //roam main loop

		/*roamHandleCmds();
		game->update(dt);
		renderWorld(dt);
		break;*/

		//Command and message handle
		roamHandleCmds();
		//if /disconnected command was entered
		if (game == nullptr)
			return;

		//update game
		game->update(dt);
		/*if (GetSoundActivated())
		{
				GetSound()->setLocalPlayerDir(game->getPlayer(0)->getDir());
				GetSound()->setLocalPlayerPos(game->getPlayer(0)->getPos());		
		}*/

		if (game->playerWantsToRespawn() && game->getPlayer(0)->getTeam() != 0)
		{
			game->allowPlayerRespawn(0, 0);
		}

		if (game->weaponSwitchReady())
		{
			int swaploc = 0;
			WEAPON_TYPE wt = game->getWpnSwitch(swaploc);
			game->handleWeaponSwitch(0, wt, swaploc);
		}

		if (game->fireEventReady())
		{
			WEAPON_TYPE wt;
			int bID;
			game->getLatestWeaponFired(0, wt, bID);
		}

		if (game->consumableReady())
		{
			game->getConsumableUsed(0);
		}

		if (game->specialActivationReady())
		{
			Player* p = game->getPlayer(0);
			int sid = -1;
			game->getSpecialAbilityUsed(0, sid);
		}

		std::vector<BulletHitWorldInfo> bulletHitsOnWorld = game->getAllBulletOnWorldCollisions();
		if (bulletHitsOnWorld.size() != 0)
		{
			for (unsigned int c = 0; c < bulletHitsOnWorld.size(); c++)
			{
				game->handleBulletHitWorldEvent(bulletHitsOnWorld[c]);
			}
			game->clearBulletOnWorldCollisions();
		}

		std::vector<BulletHitEffectInfo> bulletHitsOnEffect = game->getAllBulletOnEffectCollisions();
		if (bulletHitsOnEffect.size() != 0)
		{
			for (unsigned int c = 0; c < bulletHitsOnEffect.size(); c++)
			{
				game->handleBulletHitEffectEvent(bulletHitsOnEffect[c]);
			}
			game->clearBulletOnEffectCollisions();
		}

		std::vector<EffectHitPlayerInfo> effectHitsOnPlayer = game->getAllEffectOnPlayerCollisions();
		if (effectHitsOnPlayer.size() != 0)
		{
			for (unsigned int c = 0; c < bulletHitsOnWorld.size(); c++)
			{
				game->handleEffectHitPlayerEvent(effectHitsOnPlayer[c]);
			}
			game->clearEffectOnPlayerCollisions();
		}

		std::vector<BulletTimeOutInfo> bulletTimeOut = game->getAllTimedOutBullets();
		if (bulletTimeOut.size() != 0)
		{
			for (unsigned int c = 0; c < bulletTimeOut.size(); c++)
			{
				game->handleBulletTimeOuts(bulletTimeOut[c]);
			}
			game->clearTimedOutBullets();
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
		if (top)
			delete top;
		top = new Client();
		top->init(&console, _port, _addrs, client_record, client_playback);

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
				game->init(MAX_CONNECT, current, &console);
				game->sendPlayerRadSize(0.9f);
				//map loaded, fetch spawnpoints from render
				renderPipe->getSpawnpoints(*game->getSpawnpoints());
				// fetch all collision boxes
				bool allchunksSent = false;
				for (int i = 0; !allchunksSent; i++)
					allchunksSent = sendChunkBoxes(i);
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
		top->netlogUpdate(dt);
		if (top->network_IN(dt) == false)
		{
			disconnect();
			return;
		}

		//update game

		game->update(dt);

	/*	if (GetSoundActivated())
		{
			GetSound()->setLocalPlayerDir(game->getPlayer(top->getConId())->getDir());
			GetSound()->setLocalPlayerPos(game->getPlayer(top->getConId())->getPos());
		}*/

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
			if(game->getPlayer(top->getConId())->getTeam() != 0 ) // dont ask for respawns if we are on team 0
				top->command_respawn(top->getConId());
		}

		//Frame tick timer. Network out
		tick_timer += dt;
		if (tick_timer > tick)
		{
			//Fetch current player position
			//Add to topology packet
			Player* local = game->getPlayer(top->getConId());
			top->frame_pos(top->getConId(), local->getPos(), local->getDir(), local->getVelocity());
		
			if (game->weaponSwitchReady())
			{
				int swaploc = -1;
				WEAPON_TYPE ws = game->getWpnSwitch(swaploc);
				top->frame_weapon_switch(top->getConId(), ws, swaploc);
			}

			if (game->fireEventReady())
			{
				WEAPON_TYPE wt;
				int bID;
				game->getLatestWeaponFired(top->getConId(), wt, bID);
				top->frame_fire(wt, top->getConId(), bID, local->getPos(), local->getDir());
			}

			if (game->consumableReady())
			{
				CONSUMABLE_TYPE ct = game->getConsumableUsed(top->getConId());
				top->frame_consumable(ct, top->getConId(), local->getPos(), local->getDir());
			}

			if (game->specialActivationReady())
			{
				int sid = -1;
				SPECIAL_TYPE st = game->getSpecialAbilityUsed(top->getConId(), sid);
				top->frame_special_use(st, top->getConId(), sid, local->getPos(), local->getDir());
			}

			//send animstates
			if (local->getAnimState_t_p() == AnimationState::none)
			{
				int k = 0;
			}
			top->frame_anim(top->getConId(), local->getAnimState_f_p(), local->getAnimState_t_p());
			local->setAnimState_f_p(AnimationState::first_primary_idle);
			local->setAnimState_t_p(AnimationState::third_primary_idle);

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
		game->init(MAX_CONNECT, current, &console);
		game->sendPlayerRadSize(0.9f);
		//map loaded, fetch spawnpoints from render
		renderPipe->getSpawnpoints(*game->getSpawnpoints());
		// fetch all collision boxes
		bool allchunksSent = false;
		for (int i = 0; !allchunksSent; i++)
			allchunksSent = sendChunkBoxes(i);
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

		std::vector<BulletHitPlayerInfo> bulletHitsOnPlayer = game->getAllHitPlayerInfo();
		if (bulletHitsOnPlayer.size() != 0)
		{
			for (unsigned int c = 0; c < bulletHitsOnPlayer.size(); c++)
			{
				int newHP = game->handleBulletHitPlayerEvent(bulletHitsOnPlayer[c]);
				bulletHitsOnPlayer[c].newHPtotal = newHP;
			}
			top->event_bullet_hit_player(bulletHitsOnPlayer);
			game->clearBulletOnPlayerCollisions();
		}

		std::vector<BulletHitWorldInfo> bulletHitsOnWorld = game->getAllBulletOnWorldCollisions();
		if (bulletHitsOnWorld.size() != 0)
		{
			for (unsigned int c = 0; c < bulletHitsOnWorld.size(); c++)
			{
				game->handleBulletHitWorldEvent(bulletHitsOnWorld[c]);
			}
			top->event_bullet_hit_world(bulletHitsOnWorld);
			game->clearBulletOnWorldCollisions();
		}

		std::vector<BulletHitEffectInfo> bulletHitsOnEffect = game->getAllBulletOnEffectCollisions();
		if (bulletHitsOnEffect.size() != 0)
		{
			for (unsigned int c = 0; c < bulletHitsOnEffect.size(); c++)
			{
				game->handleBulletHitEffectEvent(bulletHitsOnEffect[c]);
			}
			top->event_bullet_hit_effect(bulletHitsOnEffect);
			game->clearBulletOnEffectCollisions();
		}

		std::vector<EffectHitPlayerInfo> effectHitsOnPlayer = game->getAllEffectOnPlayerCollisions();
		if (effectHitsOnPlayer.size() != 0)
		{
			for (unsigned int c = 0; c < effectHitsOnPlayer.size(); c++)
			{
				game->handleEffectHitPlayerEvent(effectHitsOnPlayer[c]);
			}
			top->event_effect_hit_player(effectHitsOnPlayer);
			game->clearEffectOnPlayerCollisions();
		}

		std::vector<BulletTimeOutInfo> bulletTimeOut = game->getAllTimedOutBullets();
		if (bulletTimeOut.size() != 0)
		{
			for (unsigned int c = 0; c < bulletTimeOut.size(); c++)
			{
				game->handleBulletTimeOuts(bulletTimeOut[c]);
			}
			top->event_bullet_timed_out(bulletTimeOut);
			game->clearTimedOutBullets();
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
			client_record = false;
			client_playback = false;
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
		else if (token == "/4")
		{
			current = Gamestate::CLIENT;
			client_record = true;
			client_playback = false;
			subState = 0;
		}
		else if (token == "/5")
		{
			current = Gamestate::CLIENT;
			client_record = false;
			client_playback = true;
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
			console.printMsg("/free (turns freecam on/off)", "", ' ');
			console.printMsg("/rs  show render settings", "", ' ');
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
				game->addPlayerToTeam(0, team, 0);
				game->allowPlayerRespawn(0, 0);
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
		else if (token == "/role")
		{
			ss >> token;
			if (token != "/role" || token == "1" || token == "2" || token == "3" || token == "4"|| token == "5")
			{
				int role = stoi(token);
				game->getPlayer(0)->getRole()->chooseRole(role - 1);
				game->sendPlayerRadSize(game->getPlayer(0)->getRole()->getBoxRadius());
				if (role == TRAPPER)
					game->getPlayer(0)->addModifier(MODIFIER_TYPE::TRAPPERSHAREAMMO);
				console.printMsg("You switched class!", "System", 'S');
			
			}
			else 
				console.printMsg("Invalid role. Use /role <1-5>", "System", 'S');

		}
		//render commands
		else if (token == "/rs")
		{
			ss >> token;
			if (token == "/rs") // help menu
			{
				console.printMsg("Render settings commands", "", ' ');
				console.printMsg("/rs  portal	PORTAL_CULLING ", "", ' ');
				console.printMsg("/rs  fportal	FREEZE_CULLING ", "", ' ');
				console.printMsg("/rs  chunk	RENDER_CHUNK ", "", ' ');
				console.printMsg("/rs  abb		RENDER_ABB ", "", ' ');
				console.printMsg("/rs  obb		RENDER_OBB ", "", ' ');
			}
			else if (token == "portal")
				renderPipe->setRenderFlag(PORTAL_CULLING);
			else if (token == "fportal")
				renderPipe->setRenderFlag(FREEZE_CULLING);
			else if (token == "chunk")
				renderPipe->setRenderFlag(RENDER_CHUNK);
			else if (token == "abb")
				renderPipe->setRenderFlag(RENDER_ABB);
			else if (token == "obb")
				renderPipe->setRenderFlag(RENDER_OBB);
		}
		else if (token == "/disconnect")
			disconnect();
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
		else if (token == "/role")
		{
			ss >> token;
			int role = stoi(token);
			if (role < 1 || role > 5)
				console.printMsg("Invalid role. Use /role <1-5>", "System", 'S');
			else
			{
				top->command_role_change(top->getConId(), role);
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
		file << "Port: " << _port << endl;
		if(GetSoundActivated() == 1)
			file << "Sound: " << "1";
		else
			file << "Sound: " << "0";
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
			else if (in == "Sound:")
			{
				int activateSound = atoi(in2.c_str());
				if (activateSound)
				{
					InitSound(CreateSound(), activateSound);
					GetSound()->playMusic(mainMenu);
				}
			}
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
		//temp set to see anims in 3p 
		CameraInput* cam = CameraInput::getCam();
		vec3 camPos = cam->getPos();
		vec3 camDir = cam->getDir();
		bool force3rd = false;
		if (i->getKeyInfo(GLFW_KEY_P))
		{
			cam->setCam(vec3(-11, 1, 14), vec3(0, 0, 1));
			force3rd = true;
		}

		//send chunk glowvalues
		vec3 color = { 0.1, 0.1, 0.1 };
		renderPipe->setChunkColorAndInten(0, &color[0], 0);
		//color = { 0, 0, 0.7 };
		renderPipe->setChunkColorAndInten(1, &color[0], 0);
		//color = { 0.7, 0, 0 };
		renderPipe->setChunkColorAndInten(2, &color[0], 0);
		//color = { 0, 0.7, 0 };
		renderPipe->setChunkColorAndInten(3, &color[0], 0);

		glm::vec3 tmpEyePos = CameraInput::getCam()->getPos();
		renderPipe->update(tmpEyePos.x, tmpEyePos.y, tmpEyePos.z, dt); // sets the view/proj matrix
		renderPipe->renderIni();

		//Culling
		handleCulling();

		float dgColor[3];
		//render skybox
		dgColor[0] = 0; dgColor[1] = 0; dgColor[2] = 0;
		renderPipe->renderMISC(-3, (void*)&(CameraInput::getCam()->getSkyboxMat()), dgColor, 0.0f);

		//render players
		SpotLight light;

		//find out if we are hacked, or the player we are spectating is hacked
		int hackedTeam = -1;
		if (current != ROAM)	//hacking disabled in roam
		{
			if (game->spectateID == -1)		//We are not spectating
			{
				if (top->is_client()) //server doenst have its own player
					if(game->getPlayer(top->getConId())->searchModifier(MODIFIER_TYPE::HACKINGDARTMODIFIER))
						hackedTeam = game->getPlayer(top->getConId())->getTeam(); //if we are hacked
			}
			else
			{
				if (game->getPlayer(game->spectateID)->isAlive()) // didnt fix the crash issue when spectating a player with a active hacking dart that dies
					if (game->getPlayer(game->spectateID)->searchModifier(MODIFIER_TYPE::HACKINGDARTMODIFIER))
						hackedTeam = game->getPlayer(game->spectateID)->getTeam(); //if we are hacked
			}
		}

		for (size_t i = 0; i < MAX_CONNECT; i++)
		{
			Player* p = game->getPlayer(i);
			if (p)
			{
				if (p->getTeam() != 0) //Don't render spectators!
				{	
					if (p->getHP() <= 0 || p->isAlive() == false) { // set red
						dgColor[0] = 1; dgColor[1] = 0; dgColor[2] = 0;
					}
					else
					{
						if (p->getTeam() == 1) { //team 1 color
							dgColor[0] = TEAMONECOLOR.r; dgColor[1] = TEAMONECOLOR.g; dgColor[2] = TEAMONECOLOR.b;
						}
						else if (p->getTeam() == 2) { // team 2 color
							dgColor[0] = TEAMTWOCOLOR.r; dgColor[1] = TEAMTWOCOLOR.g; dgColor[2] = TEAMTWOCOLOR.b;
						}
						else if (p->getTeam() == 0) { // spectate color
							dgColor[0] = 0; dgColor[1] = 0; dgColor[2] = 0;
						}
						//hacked team colors
						if (hackedTeam == 1) { //Show team 2's colour
							dgColor[0] = TEAMTWOCOLOR.r; dgColor[1] = TEAMTWOCOLOR.g; dgColor[2] = TEAMTWOCOLOR.b;
						}
						else if (hackedTeam == 2) { //Show team 1's colour
							dgColor[0] = TEAMONECOLOR.r; dgColor[1] = TEAMONECOLOR.g; dgColor[2] = TEAMONECOLOR.b;
						}
					}

					//static intense based on health
					float hpval = float(p->getHP()) / 130.0f;

					//send player light
					light.Position = p->getPos();
					light.Position.y -= 0.5f;
					light.Direction = vec3(0.0f);//p->getDir();
					light.Color = vec3(dgColor[0], dgColor[1], dgColor[2]);
					light.DiffuseIntensity = 0.2f;
					renderPipe->addLight(&light);
		
					//If first person render
					if (!force3rd && p->isLocal() && !game->freecam || game->spectateID == i)
					{
						if(p->isLocal())   //use current anim
							renderPipe->renderAnimation(i, p->getRole()->getRole(), &p->getFPSmat(), p->getAnimState_f_c(), dgColor, hpval, true);
						else			   //use peak anim
							renderPipe->renderAnimation(i, p->getRole()->getRole(), &p->getFPSmat(), p->getAnimState_f_p(), dgColor, hpval, true);
					}
					else
					{
						glm::mat4* playermat = p->getWorldMat();
						//if (force3rd)
						//	playermat[0][1].w -= 1.55f;

						if (p->isLocal()) //use current anim
							renderPipe->renderAnimation(i, p->getRole()->getRole(), playermat, p->getAnimState_t_c(), dgColor, hpval, false);
						else              //use peak anim
							renderPipe->renderAnimation(i, p->getRole()->getRole(), playermat, p->getAnimState_t_p(), dgColor, hpval, false);
					}
				}
			}
		}

		for (int c = 0; c < BULLET_TYPE::NROFBULLETS; c++)
		{
			dgColor[0] = 0; dgColor[1] = 0; dgColor[2] = 0;
			
			std::vector<Bullet*> bullets = game->getBullets(BULLET_TYPE(c));
			for (unsigned int i = 0; i < bullets.size(); i++)
			{
				if (bullets[i]->getTeamId() == 1)
				{
					dgColor[0] = TEAMONECOLOR.r; dgColor[1] = TEAMONECOLOR.g; dgColor[2] = TEAMONECOLOR.b;
				}
				else if (bullets[i]->getTeamId() == 2)
				{
					dgColor[0] = TEAMTWOCOLOR.r; dgColor[1] = TEAMTWOCOLOR.g; dgColor[2] = TEAMTWOCOLOR.b;
				}
				renderPipe->renderBullet(c, bullets[i]->getWorldMat(), dgColor, 0.0f);
			}
		}

		renderPipe->render();

		float herpderpOffset = 0;
		for (int c = 0; c < EFFECT_TYPE::NROFEFFECTS; c++)
		{
			dgColor[0] = 0; dgColor[1] = 0; dgColor[2] = 0;

			std::vector<Effect*> eff = game->getEffects(EFFECT_TYPE(c));
			for (unsigned int i = 0; i < eff.size(); i++)
			{
				LightwallEffect* derp = (LightwallEffect*)eff[i];
				int pid, eid;
				derp->getId(pid, eid);
				int team = game->getPlayer(pid)->getTeam();

				if (team == 1)
				{
					dgColor[0] = TEAMONECOLOR.r; dgColor[1] = TEAMONECOLOR.g; dgColor[2] = TEAMONECOLOR.b;
				}
				else if (team == 2)
				{
					dgColor[0] = TEAMTWOCOLOR.r; dgColor[1] = TEAMTWOCOLOR.g; dgColor[2] = TEAMTWOCOLOR.b;
				}

				renderPipe->renderWallEffect(&derp->getPos(), &derp->getEndPoint(), herpderpOffset, dgColor);

				herpderpOffset += glm::distance(derp->getPos(), derp->getEndPoint());
			}
		}

		renderPipe->finalizeRender();

		//viewing 3rd person anims in roam
		if (i->getKeyInfo(GLFW_KEY_P))
			cam->setCam(camPos, camDir);
	}
}

void Core::handleCulling()
{
	CameraInput* cam = CameraInput::getCam();
	if (game->spectateID == -1) // if we are not spectating some one
	{
		int newRoom = renderPipe->portalIntersection((float*)&lastCampos, (float*)&cam->getPos(), cam->roomID);
		if (newRoom != -1)
			cam->roomID = newRoom;
	
		if (game->freecam == false)
		{
			//will set local players roomID to same as cam in player update
		}
		else if (current != Gamestate::SERVER) // if we are in freecam, spectatiing some one eles or what not. need to update our player's roomid
		{
			Player* p = nullptr;
			if (current == Gamestate::ROAM)
				p = game->getPlayer(0);
			else
				p = game->getPlayer(top->getConId());
	
			if (p)
			{
				if (p->getJustRespawned())
					lastPlayerPos = p->getPos();
	
				newRoom = renderPipe->portalIntersection((float*)&lastPlayerPos, (float*)&p->getPos(), p->roomID);
				if (newRoom != -1)
					p->roomID = newRoom;
				lastPlayerPos = p->getPos();
			}
		}
	}
	
	lastCampos = cam->getPos();
	
	renderPipe->setCullingCurrentChunkID(cam->roomID);
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

	i->setupCallbacks(win);

	glfwShowWindow(win);

	glfwMakeContextCurrent(win);

	//set vsync off
	glfwSwapInterval(0);

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
		//s
		cam->init((glm::mat4*)renderPipe->getView(), winX / 2, winY / 2);

		if (!renderPipe->setSetting(PIPELINE_SETTINGS::VIEWPORT, pv))
		{
			console.printMsg("Error: Failed to set pipeline setting: VIEWPORT", "System", 'S');
		}
	}

	uiManager->setRenderPtr(renderPipe);
}

void Core::setfps(int fps)
{
	if (win != nullptr)
	{
		if (game != nullptr)
		{
			CameraInput* cam = CameraInput::getCam();
			string print( "X: " + to_string(cam->getPos().x) + "  Y: " + to_string(cam->getPos().y) + "  Z: " + to_string(cam->getPos().z) + "  FPS: " + to_string(fps));
			glfwSetWindowTitle(win, print.c_str());
		}
		else
			glfwSetWindowTitle(win, to_string(fps).c_str());
	}
		
}

void Core::sendPlayerBox()
{
	std::vector<float> pBox;
	float xMax, xMin, yMax, yMin, zMax, zMin;
	xMax = 0.5f;
	xMin = -0.5f;
	yMax = 2.0f;
	yMin = -2.0f;
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

bool Core::sendChunkBoxes(int chunkID)
{
	if (renderPipe != nullptr)
	{
		void* cBoxes = renderPipe->getChunkCollisionVectorAsPoint(chunkID);

		if (cBoxes != nullptr)
		{
			game->sendChunkBoxes(chunkID, cBoxes);
			return 0;
		}
	}
	//when we're done with all chunks, we'll get here cause cBoxes will be nullptr, so we'll end the for-loop
	return 1;
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
	if(top)
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
