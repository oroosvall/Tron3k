#include "Core.h"

void Core::init()
{
	firstTimeInEnd = false;
	lowTicketsFirstTime = false;
	uitmpcounter = 0;

	glfwInit();
	i = Input::getInput();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, false);

	//glfwWindowHint(GLFW_DECORATED, false); //borderless toggle

	closeGame = false;

	cursorInvisible = false;

	win = nullptr;
	renderPipe = nullptr;

	recreate = false;
	fullscreen = false;
	winX = winY = 800;
	//winX = winY = 1000;
	winX = 1280; winY = 720;
	//winX = 1920, winY = 1080;
	//winX = 1600, winY = 900;

	createWindow(winX, winY, fullscreen);

	serverRender = false;

	timepass = 0.0f;

	current = Gamestate::START;

	tick_timer = 0;
	client_record = false;
	client_playback = false;

	namePlates = new int[MAX_CONNECT];

	uiManager = new UIManager();
	initPipeline();
	uiManager->init(&console, winX, winY);

	renderUI = false;
	startTeamSelect = true; //Temp
	renderMenu = true;
	menuIpKeyListener = false;
	menuNameKeyListener = false;

	nameNrOfKeys = 0;
	ipNrOfKeys = 0;
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
	{
		for (size_t i = 0; i < MAX_CONNECT; i++)
		{
			renderPipe->removeTextObject(namePlates[i]);
		}
		renderPipe->removeTextObject(leaderBoardTextID);
		renderPipe->removeTextObject(leaderBoardSmallTextID);

		renderPipe->release();
	}

	delete[] namePlates;

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
	if (shitBool && justAFrameCounterActivated)
	{
		justAFrameCounter++;
	}
	if (shitBool && justAFrameCounter > 5)
	{
		game->getPlayer(game->GetLocalPlayerId())->setLockedControls(false);
		shitBool = false;
		justAFrameCounterActivated = false;;
		justAFrameCounter = 0;
	}
	cursorBlink += dt;
	if (recreate)
		createWindow(winX, winY, fullscreen);

	if (!cursorInvisible)
		glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else
		glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	glfwPollEvents();

	bool otherListeners = true;
	if ((menuNameKeyListener || menuIpKeyListener) == true)
		otherListeners = false;

	if (otherListeners)
		console.update(_name); //Updates to check for new messages and commands
	else if (menuIpKeyListener)
		menuIpKeyInputUpdate(); //Updates to check input for the ip window
	else
		menuNameKeyInputUpdate(); //Updates to check input for the name window

	bool chatMode = console.getInChatMode();
	if (chatMode && renderPipe)
	{
		if (cursorBlink > 0.5f)
		{
			renderPipe->setChatTypeMessage(console.pollLatest() + "|");
			if (cursorBlink > 1.0f)
				cursorBlink = 0.0f;
		}
		else
			renderPipe->setChatTypeMessage(console.pollLatest());
	}
	if (console.getHistoryUpdate() && renderPipe)
	{
		renderPipe->setChatHistoryText(console.getHistory());
	}



	if (game)
	{
		if (current != SERVER)
		{
			if (!uiManager->isThereAMenuUp())
				cursorInvisible = true;

			int pid = game->GetLocalPlayerId();
			if (pid != -1)
			{
				Player* p = game->getPlayer(pid);
				if (p != nullptr)
				{
					if (current != SERVER)
					{
						if (!console.getInChatMode() && cursorInvisible)
						{
							p->setLockedControls(false);
							renderPipe->setChatTypeMessage("");
						}
						else if (console.getInChatMode())
						{
							p->setLockedControls(true);
						}

						if (!p->getLockedControls())
						{
							cursorInvisible = true;
						}
					}
				}
			}
		}

		if (console.getInChatMode() == false && cheatsOn)
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
			if (i->justPressed(GLFW_KEY_0))
			{
				playbackSpeed = 0.0f;
			}
			if (i->justPressed(GLFW_KEY_9))
			{
				playbackSpeed = 1.0f;
			}
		}
	}

	dt *= playbackSpeed;

	switch (current)
	{
	case START:		upStart(dt);	break;
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

	if (current != SERVER)
	{
		int swapTime = renderPipe->startExecTimer("Swap");
		glfwSwapBuffers(win);
		renderPipe->stopExecTimer(swapTime);
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
		console.printMsg("[/4] Client_Record", "System", 'S');
		console.printMsg("[/5] Client_Playback", "System", 'S');

		if (!settingsLoaded)
		{
			loadSettings();
			settingsLoaded = true;
		}

		subState++;
		break;

	case 1:
		//start console commands
		startHandleCmds();
		if (renderMenu)
			upMenu(dt);
		break;
	}

}

void Core::upMenu(float dt)
{
	renderUI = true;

	double tX = (0.0);
	double tY = (0.0);
	//Get mouse position
	i->getCursor(tX, tY);
	tX = (tX / (double)winX) * 2 - 1.0; // (x/ResolutionX) * 2 - 1
	tY = (-tY / (double)winY) * 2 + 1.0; // (y/ResolutionY) * 2 - 1

	cursorInvisible = false;
	uiManager->menuRender();

	if (i->justPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		menuIpKeyListener = false;
		menuNameKeyListener = false;
		int eventIndex = uiManager->collisionCheck(glm::vec2((float)tX, (float)tY));
		switch (eventIndex)
		{
		case 0: //Roam
			current = ROAM;
			uiManager->LoadNextSet(UISets::InGame, winX, winY);
			uiManager->setRoleBool(false);
			uiManager->setFirstMenuSet(false);
			uiManager->setMenu(InGameUI::TeamSelect);
			subState = 0;

			uiManager->setHoverCheckBool(true);
			break;
		case 1: //Multiplayer -> multiplayer window
			uiManager->setFirstMenuSet(true);
			uiManager->setMenu(MainMenu::Multiplayer);
			break;
		case 2: //Settings
			break;
		case 3: //Exit
			glfwSetWindowShouldClose(win, 1);
			break;
		case 4: //Client -> connect window
		{
			uiManager->setFirstMenuSet(true);
			uiManager->setMenu(MainMenu::Connect);

			string addr = _addrs.toString();
			clientHandleCmds("/ip " + addr);
			nameNrOfKeys = _name.size();
			ipNrOfKeys = addr.size();

			uiManager->setText(addr, scaleAndText::IP); //Ip
			uiManager->setText(_name, scaleAndText::Name); //Name
			break;
		}
		case 5: //Server -> starts a server
			current = SERVER;
			subState = 0;

			uiManager->removeAllMenus();

			renderPipe->clearColor();
			renderPipe->clearBothBuffers();
			glfwSwapBuffers(win);
			renderPipe->clearBothBuffers();

			renderMenu = false;
			renderUI = false;
			break;
		case 6: //Connect
		{
			menuIpKeyListener = false;
			std::string ip = "/ip ";
			ip += uiManager->getText(scaleAndText::IP); //From ip object
			startHandleCmds(ip);

			menuNameKeyListener = false;
			std::string name = "/name ";
			name += uiManager->getText(scaleAndText::Name); //From ip object
			startHandleCmds(name);

			uiManager->setRoleBool(false);

			nameNrOfKeys = 0;
			ipNrOfKeys = 0;
			current = Gamestate::CLIENT; //Start the game as a client
			client_record = false;
			client_playback = false;
			subState = 0;
			break;
		}
		case 7: //Back
			nameNrOfKeys = 0;
			ipNrOfKeys = 0;
			uiManager->setMenu(MainMenu::Back); //Last menu
			break;
		case 10: //Ip input
			menuIpKeyListener = true;
			break;
		case 11: //Port
			menuNameKeyListener = true;
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
	if (GetSoundActivated())
	{
		GetSound()->stopMusic();
	}

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
		sendCapPointBoxes();
		sendRoomBoxes();
		Player* p = new Player();
		p->init(_name, glm::vec3(0, 30, 0));
		game->createPlayer(p, 0, 100, ROLES::TRAPPER, true);
		game->freecam = true;
		delete p;

		uiManager->HUD.maxSpecialMeter = 100.0f;
		uiManager->HUD.specialMeter = 0.0f;
		game->getPlayer(game->GetLocalPlayerId())->setLockedControls(true);
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

			Player* p = game->getPlayer(0);
			uiManager->HUD.ammo = p->getMaxAmmo();
			std::string nText = std::to_string(p->getAmmo()) + "/" + std::to_string(p->getMaxAmmo());
			uiManager->clearText(scaleAndText::Ammo);
			uiManager->setText(nText, scaleAndText::Ammo);
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
			for (int c = 0; c < effectHitsOnPlayer.size(); c++)
				game->handleEffectHitPlayerEvent(effectHitsOnPlayer[c]);
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

		std::vector<EffectTimeOutInfo> effectTimeOut = game->getAllTimedOutEffects();
		if (effectTimeOut.size() != 0)
		{
			for (unsigned int c = 0; c < effectTimeOut.size(); c++)
			{
				game->handleEffectTimeOuts(effectTimeOut[c]);
			}
			game->clearTimedOutEffects();
		}

		renderWorld(dt);
		break;
	}
}

void Core::upClient(float dt)
{
	if (GetSoundActivated())
	{
		GetSound()->stopMusic();
	}

	switch (subState)
	{
	case 0: //init
		if (top)
			delete top;
		top = new Client();
		top->init(&console, uiManager, _port, _addrs, client_record, client_playback);

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
				sendCapPointBoxes();
				sendRoomBoxes();
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
			game->getPlayer(game->GetLocalPlayerId())->setLockedControls(true);
			showTeamSelect();

			subState++;
			uiManager->HUD.specialMeter = 0.0f;
			uiManager->HUD.maxSpecialMeter = 100.0f;
			uiManager->HUD.loseTicketPer = 14;

			KingOfTheHill* koth = (KingOfTheHill*)game->getGameMode();
			uiManager->HUD.maxTokens = koth->getMaxTokensPerTeam();
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
		KingOfTheHill* koth = (KingOfTheHill*)game->getGameMode();
		KOTHSTATE tmp = koth->getState();

		Player* localp = game->getPlayer(top->getConId());

		float newDt = dt;
		if (game)
		{
			//Slow down when we are in endround
			if (game->getGameMode()->getType() == GAMEMODE_TYPE::KOTH)
			{
				KingOfTheHill* k = (KingOfTheHill*)game->getGameMode();
				if (k->getIfSlowdown() && game->freecam == false)
				{
					if (slowdownTimer <= 0)
					{
						slowmode *= slowdownFactor;
						slowdownTimer = 0.5;
					}
					else
						slowdownTimer -= dt;
					if (slowmode < 0.1f)
					{
						slowmode = 0.00000001f;
					}
					CameraInput::getCam()->setPlaybackSpeed(slowmode);
					newDt *= slowmode;
				}
				else if (slowmode < 1.0f)
					slowmode = 1.0f;
				else
					CameraInput::getCam()->setPlaybackSpeed(1.0f);

				if (k->getFreeze() && game->freecam == false)
				{
					newDt = 0;

					//CameraInput::getCam()->setPlaybackSpeed(1.0f);
				}

				//Checks it was the end of the match
				if (firstTimeInEnd && tmp == KOTHSTATE::ENDMATCH)
				{
					GAMEMODE_MSG tMode = k->getLastMsg();
					int tTeam = localp->getTeam();

					if (tMode == GAMEMODE_MSG::MATCH_WIN_TEAM1)
					{
						if (tTeam == 1)
						{
							uiManager->changeTextureHideAble(hideAbleObj::Banner, BannerTextureIDs::Victory);
							uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
						}
						else if (tTeam == 2)
						{
							uiManager->changeTextureHideAble(hideAbleObj::Banner, BannerTextureIDs::Defeat);
							uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
						}
						uiManager->HUD.bannerCounter = 0;
						firstTimeInEnd = false;
					}
					else if (tMode == GAMEMODE_MSG::MATCH_WIN_TEAM2)
					{
						if (tTeam == 1)
						{
							uiManager->changeTextureHideAble(hideAbleObj::Banner, BannerTextureIDs::Defeat);
							uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
						}
						else if (tTeam == 2)
						{
							uiManager->changeTextureHideAble(hideAbleObj::Banner, BannerTextureIDs::Victory);
							uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
						}
						uiManager->HUD.bannerCounter = 0;
						firstTimeInEnd = false;
					}
					else if (tMode == GAMEMODE_MSG::MATCH_DRAW)
					{
						uiManager->changeTextureHideAble(hideAbleObj::Banner, BannerTextureIDs::Victory);
						uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
						uiManager->HUD.bannerCounter = 0;
						firstTimeInEnd = false;
					}
				}

				//Rights out the of round.
				if (firstTimeInEnd && tmp == KOTHSTATE::ENDROUND)
				{
					if (localp->getTeam() != 0)
					{
						uiManager->changeTextureHideAble(hideAbleObj::Banner, BannerTextureIDs::EndOfRound);
						uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
						uiManager->HUD.bannerCounter = 0;
						firstTimeInEnd = false;
					}
				}
			}
		}
		game->update(newDt);

		if (kothState != tmp)
		{
			if (tmp == KOTHSTATE::PREROUND)
			{
				top->command_role_change(top->getConId(), 1);
				//dont show class select when in spectate
				if (localp->getTeam() != 0)
					showClassSelect();
			}
			else if (tmp == KOTHSTATE::ROUND)
			{
				if (game->spectateID != -1)
					localp = game->getPlayer(game->spectateID);

				uiManager->setTeamColor(localp->getTeam());
				uiManager->changeColorTeam();

				uiManager->setFirstMenuSet(false);
				uiManager->setMenu(InGameUI::GUI);

				localp->setLockedControls(false);

				uiManager->clearText(scaleAndText::HP);
				uiManager->clearText(scaleAndText::Ammo);
				uiManager->clearText(scaleAndText::TicketBar1);
				uiManager->clearText(scaleAndText::TicketBar2);
				uiManager->clearText(scaleAndText::Wins1);
				uiManager->clearText(scaleAndText::Wins2);

				uiManager->setText(std::to_string(localp->getHP()), scaleAndText::HP); //hp

				std::string nText = std::to_string(localp->getAmmo()) + "/" + std::to_string(localp->getMaxAmmo());
				uiManager->setText(nText, scaleAndText::Ammo); //ammo
				uiManager->setText(std::to_string(koth->getRespawnTokens(1)), scaleAndText::TicketBar1); //tickets
				uiManager->setText(std::to_string(koth->getRespawnTokens(2)), scaleAndText::TicketBar2); //tickets2
				uiManager->setText(std::to_string(koth->getRoundWins(1)), scaleAndText::Wins1); //wins1
				uiManager->setText(std::to_string(koth->getRoundWins(2)), scaleAndText::Wins2); //wins2
				if (int(koth->getTimer()) == 0)
				{
					uiManager->clearText(6);
					uiManager->setText("00:00", scaleAndText::Time); //time
				}
				uiManager->HUD.teamOneTokens = koth->getRespawnTokens(1);
				uiManager->HUD.teamTwoTokens = koth->getRespawnTokens(2);


				uiManager->scaleBar(scaleAndText::HP, 1.0f, false);
				uiManager->scaleBar(scaleAndText::TicketBar1, (float)(koth->getRespawnTokens(1)) / (float)(koth->getMaxTokensPerTeam()), false);
				uiManager->scaleBar(scaleAndText::TicketBar2, (float)(koth->getRespawnTokens(2)) / (float)(koth->getMaxTokensPerTeam()), false);
				uiManager->scaleBar(scaleAndText::AbilityMeter, 0.0f, true);

				uiManager->setRoleBool(true);
				uiManager->setHoverCheckBool(false);

				if(koth->getCapturePoint() == 0)
					uiManager->changeTextureHideAble(hideAbleObj::Banner, BannerTextureIDs::ParkinAreaActive);
				else
					uiManager->changeTextureHideAble(hideAbleObj::Banner, BannerTextureIDs::MarketActive);
				uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
				uiManager->HUD.bannerCounter = 0;

				firstTimeInEnd = true;
				lowTicketsFirstTime = true;
			}
			kothState = tmp;
		}
		/*	if (GetSoundActivated())
		{
		GetSound()->setLocalPlayerDir(game->getPlayer(top->getConId())->getDir());
		GetSound()->setLocalPlayerPos(game->getPlayer(top->getConId())->getPos());
		}*/

		//Command and message handle
		if (console.messageReady())
		{
			char scope;
			top->msg_out = console.getMessage(scope);
			top->scope_out = Uint8(scope);
		}

		clientHandleCmds();
		if (top == nullptr)//check for disconnected command
			return;

		if (game->playerWantsToRespawn())
		{
			if (game->getPlayer(top->getConId())->getTeam() != 0) // dont ask for respawns if we are on team 0
				top->command_respawn(top->getConId());
		}

		//Frame tick timer. Network out
		tick_timer += dt;
		if (tick_timer > tick)
		{
			//Fetch current player position
			//Add to topology packet
			Player* local = game->getPlayer(top->getConId());
			glm::vec3 pos = local->getPos();
			if (!local->isAlive())
			{
				if (pos.y < -4.5f)
					pos.y = -4.5f;
			}

			top->frame_pos(top->getConId(), pos, local->getDir(), local->getVelocity(), local->roomID);

			if (game->weaponSwitchReady())
			{
				int swaploc = -1;
				WEAPON_TYPE ws = game->getWpnSwitch(swaploc);
				top->frame_weapon_switch(top->getConId(), ws, swaploc);
				
				uiManager->HUD.ammo = local->getMaxAmmo();
				std::string nText = std::to_string(local->getAmmo()) + "/" + std::to_string(local->getMaxAmmo());
				uiManager->clearText(scaleAndText::Ammo);
				uiManager->setText(nText, scaleAndText::Ammo); //ammo
			}

			if (game->specialActivationReady())
			{
				int sid = -1;
				SPECIAL_TYPE st = game->getSpecialAbilityUsed(top->getConId(), sid);
				top->frame_special_use(st, top->getConId(), sid, local->getPos(), local->getDir(), local->getTeam());
			}

			if (game->fireEventReady())
			{
				WEAPON_TYPE wt;
				int bID;
				game->getLatestWeaponFired(top->getConId(), wt, bID);
				top->frame_fire(wt, top->getConId(), bID, local->getPos(), local->getDir(), local->getTeam());
			}

			if (game->consumableReady())
			{
				CONSUMABLE_TYPE ct = game->getConsumableUsed(top->getConId());
				top->frame_consumable(ct, top->getConId(), local->getPos(), local->getDir(), local->getTeam());
			}

			//send animstates
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
	if (GetSoundActivated())
	{
		GetSound()->stopMusic();
	}

	switch (subState)
	{
	case 0:  //init server
	{
		//createWindow(200, 200, false);
		serverCam = CameraInput::getCam();
		if (top)
			delete top;
		top = new Server();
		top->init(&console, uiManager, _port, _addrs);
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

		//attempting mapload
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
		sendCapPointBoxes();
		sendRoomBoxes();
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
			char scope;
			top->msg_out = console.getMessage(scope);
			top->scope_out = Uint8(ALL); //Server always communicates with everyone
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
				int newHP = game->handleEffectHitPlayerEvent(effectHitsOnPlayer[c]);
				effectHitsOnPlayer[c].newHPtotal = newHP;
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

		std::vector<EffectTimeOutInfo> effectTimeOut = game->getAllTimedOutEffects();
		if (effectTimeOut.size() != 0)
		{
			for (unsigned int c = 0; c < effectTimeOut.size(); c++)
			{
				game->handleEffectTimeOuts(effectTimeOut[c]);
			}
			top->event_effect_timed_out(effectTimeOut);
			game->clearTimedOutEffects();
		}

		/*
		MIGHT WANT TO CHANGE THIS LOL
		*/
		static float serverDataTimer = 0.0f;
		serverDataTimer += dt;
		if (serverDataTimer > 0.25f)
		{
			top->event_gamemode_data();
			//top->event_player_data();
			serverDataTimer = 0.0f;
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

void Core::startHandleCmds(std::string com)
{
	if (console.commandReady() || com != "")
	{
		string token;
		istringstream ss;

		if (com != "")
			ss = istringstream(com);
		else
			ss = istringstream(console.getCommand());
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
			uiManager->removeAllMenus();
			renderPipe->clearBothBuffers();
			glfwSwapBuffers(win);
			renderPipe->clearBothBuffers();
			renderMenu = false;
			renderUI = false;
		}

		else if (token == "/3")
		{
			current = Gamestate::ROAM;
			subState = 0;
			renderMenu = false;
			renderUI = true;

			showTeamSelect();
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

void Core::roamHandleCmds(std::string com)
{
	if (console.commandReady() || com != "")
	{
		string token;
		istringstream ss;

		if (com != "")
			ss = istringstream(com);
		else
			ss = istringstream(console.getCommand());
		ss >> token;

		if (token == "/help")
		{
			console.printMsg("Console commands", "", ' ');
			console.printMsg("/name " + _name, "", ' ');
			console.printMsg("/free (turns freecam on/off)", "", ' ');
			console.printMsg("/rs  show render settings", "", ' ');
			console.printMsg("/disconnect", "", ' ');
			console.printMsg("/sens <positive decimal number>", "", ' ');

		}
		else if (token == "/slow")
		{
			if (cheatsOn)
			{
				ss >> token;
				if (token.size() > 0)
				{
					float slow = atof(token.c_str());
					playbackSpeed = slow;
				}
			}
		}
		else if (token == "/cheats")
		{
			if (cheatsOn)
				cheatsOn = false;
			else
				cheatsOn = true;
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
			if (token == "0" || token == "1" || token == "2")
			{
				int team = stoi(token);
				game->addPlayerToTeam(0, team);
				game->allowPlayerRespawn(0, 0);
				if (team != 0)
					game->freecam = false;
				else
					game->freecam = true;

				uiManager->setTeamColor(team);
				uiManager->changeColorTeam();

				if (!uiManager->getRoleBool())
				{
					uiManager->setMenu(InGameUI::RemoveMenu);
					uiManager->setFirstMenuSet(false);
					uiManager->setMenu(InGameUI::ClassSelect);
					uiManager->setRoleBool(true);
				}
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
		else if (token == "/sens")
		{
			float sens = 0.0f;
			if (!(ss >> sens))
			{
				console.printMsg("Invalid input.", "System", 'S');
			}
			else
			{
				if (sens < FLT_EPSILON)
				{
					console.printMsg("Positive numbers only, dummy.", "System", 'S');
				}
				else
					serverCam->setSensitivity(sens);
			}
		}

		else if (token == "/cleanup")
		{
			GetSound()->playUserGeneratedSound(SOUNDS::announcerCleanup);
		}

		else if (token == "/role")
		{
			ss >> token;
			if (token != "/role" || token == "1" || token == "3" || token == "4" || token == "5")
			{
				int role = stoi(token);
				game->getPlayer(0)->chooseRole(role - 1);
				game->sendPlayerRadSize(game->getPlayer(0)->getRole()->getBoxRadius());
				console.printMsg("You switched class!", "System", 'S');

				uiManager->setMenu(InGameUI::RemoveMenu);
				uiManager->setFirstMenuSet(false);
				uiManager->setMenu(InGameUI::GUI);

				justAFrameCounterActivated = true;
				shitBool = true;

				cursorInvisible = true;

				Player* local = game->getPlayer(game->GetLocalPlayerId());
				KingOfTheHill* koth = (KingOfTheHill*)game->getGameMode();

				uiManager->clearText(scaleAndText::HP);
				uiManager->clearText(scaleAndText::Ammo);
				uiManager->clearText(scaleAndText::TicketBar1);
				uiManager->clearText(scaleAndText::TicketBar2);
				uiManager->clearText(scaleAndText::Wins1);
				uiManager->clearText(scaleAndText::Wins2);
				uiManager->clearText(scaleAndText::Time);

				uiManager->setText(std::to_string(local->getHP()), scaleAndText::HP); //hp

				std::string nText = std::to_string(local->getAmmo()) + "/" + std::to_string(local->getMaxAmmo());
				uiManager->setText(nText, scaleAndText::Ammo); //ammo
				uiManager->setText(std::to_string(koth->getRespawnTokens(1)), scaleAndText::TicketBar1); //tickets
				uiManager->setText(std::to_string(koth->getRespawnTokens(2)), scaleAndText::TicketBar2); //tickets2
				uiManager->setText(std::to_string(koth->getRoundWins(1)), scaleAndText::Wins1); //wins1
				uiManager->setText(std::to_string(koth->getRoundWins(2)), scaleAndText::Wins2); //wins2
				uiManager->setText("00:00", scaleAndText::Time); //time
												//uiManager->setText(std::to_string(int(koth->getTimer())), 6); //time


				uiManager->scaleBar(scaleAndText::TicketBar1, 0.0f, false);
				uiManager->scaleBar(scaleAndText::TicketBar2, 0.0f, false);
				uiManager->scaleBar(scaleAndText::AbilityMeter, 1.0f, true);

				uiManager->setHoverCheckBool(false);
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
				console.printMsg("/rs  room		RENDER_ROOM ", "", ' ');
				console.printMsg("/rs  debug	RENDER_DEBUG_TEXT ", "", ' ');
				console.printMsg("/rs  gui		RENDER_GUI ", "", ' ');
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
			else if (token == "room")
				renderPipe->setRenderFlag(RENDER_ROOM);
			else if (token == "debug")
				renderPipe->setRenderFlag(RENDER_DEBUG_TEXT);
			else if (token == "gui")
				renderPipe->setRenderFlag(RENDER_GUI);
		}
		else if (token == "/disconnect")
		{
			disconnect();
		}
	}
}

void Core::clientHandleCmds(std::string com)
{
	if (console.commandReady() || com != "")
	{
		string token;
		istringstream ss;

		if (com != "")
			ss = istringstream(com);
		else
			ss = istringstream(console.getCommand());
		ss >> token;

		if (token == "/help")
		{
			console.printMsg("Console commands", "", ' ');
			console.printMsg("/name " + _name, "", ' ');
			console.printMsg("/sens <positive decimal number>", "", ' ');
			console.printMsg("/team " + to_string(game->getPlayer(top->getConId())->getTeam()), "", ' ');
			console.printMsg("/players", "", ' ');
			console.printMsg("/disconnect", "", ' ');
			console.printMsg("/free (turns freecam on/off)", "", ' ');
			console.printMsg("/spec # (spectate player id)", "", ' ');
			console.printMsg("/rs  show render settings", "", ' ');
		}
		else if (token == "/cheats")
		{
			if (cheatsOn)
				cheatsOn = false;
			else
				cheatsOn = true;
		}
		if (token == "/ready")
		{
			top->command_gamemode_ready(top->getConId());
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
				//RIP change team request message
			}
			else
				console.printMsg("Invalid team. Use /team <0/1/2>", "System", 'S');
		}
		else if (token == "/sens")
		{
			float sens = 0.0f;
			if (!(ss >> sens))
			{
				console.printMsg("Invalid input.", "System", 'S');
			}
			else
			{
				if (sens < FLT_EPSILON)
				{
					console.printMsg("Positive numbers only, dummy.", "System", 'S');
				}
				else
					serverCam->setSensitivity(sens);
			}
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

		else if (token == "/cleanup")
		{
			GetSound()->playUserGeneratedSound(SOUNDS::announcerCleanup);
		}
		else if (token == "/role")
		{
			ss >> token;
			if (token == "1" || token == "3" || token == "4" || token == "5")
			{
				int role = stoi(token);
				top->command_role_change(top->getConId(), role);
			}
			else
				console.printMsg("Invalid role. Use /role <1, 3, 4>", "System", 'S');
		}
		else if (token == "/disconnect")
		{
			disconnect();
		}
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
						//set team color
						uiManager->setTeamColor(p->getTeam());
						uiManager->changeColorTeam();
					}
					else
						console.printMsg("That Player Doesn't exist", "System", 'S');
				}
				else
					game->spectateID = -1;
			}
		}
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
				console.printMsg("/rs  room		RENDER_ROOM ", "", ' ');
				console.printMsg("/rs  debug	RENDER_DEBUG_TEXT ", "", ' ');
				console.printMsg("/rs  gui		RENDER_GUI ", "", ' ');
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
			else if (token == "room")
				renderPipe->setRenderFlag(RENDER_ROOM);
			else if (token == "debug")
				renderPipe->setRenderFlag(RENDER_DEBUG_TEXT);
			else if (token == "gui")
				renderPipe->setRenderFlag(RENDER_GUI);
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
		if (GetSoundActivated() == 1)
			file << "Sound: " << "1" << endl;
		else
			file << "Sound: " << "0" << endl;

		float sens = serverCam->getSensitivity();
		file << "Sensitivity: " << sens;

		file.close();
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
			else if (in == "Sensitivity:")
			{
				float sens = atof(in2.c_str());
				if (sens < FLT_EPSILON)
				{
					sens = 0.2f;
				}
				serverCam->setSensitivity(sens);
			}
		}
		file.close();
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
		/*		if (i->getKeyInfo(GLFW_KEY_P))
		{
		cam->setCam(vec3(-6, 1.5f, 33), vec3(0, 0, -1));
		force3rd = true;
		}
		if (i->getKeyInfo(GLFW_KEY_L))
		{
		cam->setCam(vec3(-6, 1.5f, 33), vec3(1, 0, 0));
		force3rd = true;
		}
		*/

		int pid = game->GetLocalPlayerId();
		Player* tmp_player = game->getPlayer(pid);
		tmp_player->deadViewAngles();

		glm::vec3 tmpEyePos = CameraInput::getCam()->getPos();

		renderPipe->update(tmpEyePos.x, tmpEyePos.y, tmpEyePos.z, dt); // sets the view/proj matrix
		renderPipe->renderIni();

		SpotLight light;

		vec3 dgColor(0);
		//render skybox
		renderPipe->renderMISC(-3, (void*)&(CameraInput::getCam()->getSkyboxMat()), &dgColor.x, 0.0f);

		//send chunk glowvalues
		dgColor = { 0.1, 0.1, 0.1 };
		renderPipe->setChunkColorAndInten(0, &dgColor[0], 0);
		//color = { 0, 0, 0.7 };
		renderPipe->setChunkColorAndInten(1, &dgColor[0], 0);
		//color = { 0.7, 0, 0 };
		renderPipe->setChunkColorAndInten(2, &dgColor[0], 0);
		//color = { 0, 0.7, 0 };
		renderPipe->setChunkColorAndInten(3, &dgColor[0], 0);

		int cap = ((KingOfTheHill*)game->getGameMode())->getCapturePoint();

		int capOwner = ((KingOfTheHill*)game->getGameMode())->getCapturePointOwner();

		if (capOwner == 0)
		{
			renderPipe->setCapRoomColor(cap, vec3(0x36, 0xb2, 0xd0), 1.0f);
		}
		else if (capOwner == 1)
		{
			renderPipe->setCapRoomColor(cap, TEAMONECOLOR, 1.0f);
		}
		else if (capOwner == 2)
		{
			renderPipe->setCapRoomColor(cap, TEAMTWOCOLOR, 1.0f);
		}


		//Culling
		handleCulling();

		//find out if we are hacked, or the player we are spectating is hacked 
		int hackedTeam = -1;
		if (current != ROAM)	//hacking disabled in roam
		{
			if (game->spectateID == -1)		//We are not spectating
			{
				if (top->is_client()) //server doenst have its own player
					if (game->getPlayer(top->getConId())->searchModifier(MODIFIER_TYPE::HACKINGDARTMODIFIER))
						hackedTeam = game->getPlayer(top->getConId())->getTeam(); //if we are hacked
			}
			else
			{
				if (game->getPlayer(game->spectateID)->isAlive()) // didnt fix the crash issue when spectating a player with a active hacking dart that dies
					if (game->getPlayer(game->spectateID)->searchModifier(MODIFIER_TYPE::HACKINGDARTMODIFIER))
						hackedTeam = game->getPlayer(game->spectateID)->getTeam(); //if we are hacked
			}
		}

		//*** render players ***

		//hacked team colors
		if (hackedTeam == 1)  //Show team 2's colour
			dgColor = TEAMTWOCOLOR;

		else if (hackedTeam == 2) { //Show team 1's colour
			dgColor = TEAMONECOLOR;
		}

		int playerTime = renderPipe->startExecTimer("Players");

		for (size_t i = 0; i < MAX_CONNECT; i++)
		{
			Player* p = game->getPlayer(i);
			if (p)
			{
				if (p->getTeam() != 0) //Don't render spectators!
				{
					if (p->getHP() <= 0 || p->isAlive() == false)  // set red
					{
						dgColor = vec3(1, 0, 0);
						hackedTeam = -1;
					}
					else
					{
						if (hackedTeam == -1)
						{
							if (p->getTeam() == 1)  //team 1 color
								dgColor = TEAMONECOLOR;
							else if (p->getTeam() == 2)  // team 2 color
								dgColor = TEAMTWOCOLOR;
						}
					}

					//Take damage effect
					if (game->spectateID == -1)
					{
						if (p->isLocal())
						{
							if (lastHP_blurreffect > p->getHP())
								renderPipe->startTakeDamageEffect(6, 0.6f);
							lastHP_blurreffect = p->getHP();
						}
					}
					else if (game->spectateID == i)
					{
						if (lastHP_blurreffect > p->getHP())
							renderPipe->startTakeDamageEffect(6, 0.6f);
						lastHP_blurreffect = p->getHP();
					}

					//static intense based on health
					float hpval = float(p->getHP()) / 130.0f;

					//send player light
					light.Position = p->getPos();
					light.Position.y -= 0.5f;
					light.Direction = vec3(0.0f);//p->getDir();
					light.Color = dgColor;
					light.DiffuseIntensity = 0.2f;
					light.AmbientIntensity = 0.0f;
					renderPipe->addLight(&light, p->roomID);

					if (p->forceDeathAnim > 0)
					{
						p->setAnimState_f_p(AnimationState::first_secondary_death);
						if (p->forceDeathAnim == 1)
							p->setAnimState_f_p(AnimationState::first_primary_death);

						p->setAnimState_t_p(AnimationState::third_primary_death);
						p->forceDeathAnim = 0;
					}
					if (p->unlockDeathAnim)
					{
						p->setAnimState_f_p(AnimationState::none);
						p->setAnimState_t_p(AnimationState::none);
						p->unlockDeathAnim = false;
					}

					//If first person render
					if (!force3rd && p->isLocal() && !game->freecam || game->spectateID == i)
					{
						if (p->isLocal())   //use current anim
							renderPipe->renderAnimation(i, p->getRole()->getRole(), &p->getFPSmat(), p->getAnimState_f_c(), &dgColor.x, hpval, true, p->getAnimPrimary(), cam->roomID);
						else //use peak anim
							renderPipe->renderAnimation(i, p->getRole()->getRole(), &p->getFPSmat(), p->getAnimState_f_p(), &dgColor.x, hpval, true, p->getAnimPrimary(), cam->roomID);
					}
					else
					{
						if (p->getRole()->getRole() != ROLES::NROFROLES)
						{
							glm::mat4* playermat = p->getWorldMat();
							if (force3rd)
							{
								*playermat = glm::mat4();
								playermat[0][0].w = p->getPos().x;
								playermat[0][1].w = p->getPos().y - 0.45;
								playermat[0][2].w = p->getPos().z;
							}

							if (p->isLocal()) //use current anim
								renderPipe->renderAnimation(i, p->getRole()->getRole(), playermat, p->getAnimState_t_c(), &dgColor.x, hpval, false, false, p->roomID);
							else              //use peak anim
								renderPipe->renderAnimation(i, p->getRole()->getRole(), playermat, p->getAnimState_t_p(), &dgColor.x, hpval, false, false, p->roomID);
						}
					}
				}
			}
		}

		renderPipe->stopExecTimer(playerTime);
		//*** Render Bullets ***

		if (hackedTeam == -1)
		{
			for (int c = 0; c < BULLET_TYPE::NROFBULLETS; c++)
			{
				std::vector<Bullet*> bullets = game->getBullets(BULLET_TYPE(c));
				for (unsigned int i = 0; i < bullets.size(); i++)
				{
					if (bullets[i]->getTeam() == 1)
						renderPipe->renderBullet(c, bullets[i]->getWorldMat(), &TEAMONECOLOR.x, 0.0f);

					else if (bullets[i]->getTeam() == 2)
						renderPipe->renderBullet(c, bullets[i]->getWorldMat(), &TEAMTWOCOLOR.x, 0.0f);
				}
			}
		}
		else if (hackedTeam == 1)
		{
			for (int c = 0; c < BULLET_TYPE::NROFBULLETS; c++)
			{
				std::vector<Bullet*> bullets = game->getBullets(BULLET_TYPE(c));
				for (unsigned int i = 0; i < bullets.size(); i++)
				{
					if (bullets[i]->getTeam() == 1)
						renderPipe->renderBullet(c, bullets[i]->getWorldMat(), &TEAMTWOCOLOR.x, 0.0f);

					else if (bullets[i]->getTeam() == 2)
						renderPipe->renderBullet(c, bullets[i]->getWorldMat(), &TEAMTWOCOLOR.x, 0.0f);
				}
			}
		}
		else // if hacked team == 2
		{
			for (int c = 0; c < BULLET_TYPE::NROFBULLETS; c++)
			{
				std::vector<Bullet*> bullets = game->getBullets(BULLET_TYPE(c));
				for (unsigned int i = 0; i < bullets.size(); i++)
				{
					if (bullets[i]->getTeam() == 1)
						renderPipe->renderBullet(c, bullets[i]->getWorldMat(), &TEAMONECOLOR.x, 0.0f);

					else if (bullets[i]->getTeam() == 2)
						renderPipe->renderBullet(c, bullets[i]->getWorldMat(), &TEAMONECOLOR.x, 0.0f);
				}
			}
		} // render bullets end

		  // capturePoint
		if (current == CLIENT)
		{
			KingOfTheHill* koth = (KingOfTheHill*)game->getGameMode();
			renderPipe->renderCapturePoint(koth->getCapturePoint());
		}

		// render chunks
		renderPipe->render();

		// render effects
		float herpderpOffset = 0;

		if (hackedTeam == 1)
			dgColor = TEAMTWOCOLOR;
		else if (hackedTeam == 2)
			dgColor = TEAMONECOLOR;

		int effectTime = renderPipe->startExecTimer("Effects & decals");

		for (int c = 0; c < EFFECT_TYPE::NROFEFFECTS; c++)
		{
			std::vector<Effect*> eff = game->getEffects(EFFECT_TYPE(c));
			for (unsigned int i = 0; i < eff.size(); i++)
			{
				EFFECT_TYPE type = eff[i]->getType();
				int pid, eid;
				eff[i]->getId(pid, eid);
				int team = eff[i]->getTeam();

				if (hackedTeam == -1)
				{
					if (team == 1)
						dgColor = TEAMONECOLOR;
					else if (team == 2)
						dgColor = TEAMTWOCOLOR;
				}

				switch (type)
				{
				case LIGHT_WALL:
				{
					LightwallEffect* asd = (LightwallEffect*)eff[i];
					renderPipe->renderWallEffect(&asd->getPos(), &asd->getEndPoint(), herpderpOffset, &dgColor.x);
					herpderpOffset += glm::distance(asd->getPos(), asd->getEndPoint());
				}
				break;
				case THUNDER_DOME:
				{
					ThunderDomeEffect* asd = (ThunderDomeEffect*)eff[i];
					vec3 pos = asd->getPos();
					renderPipe->renderThunderDomeEffect(&pos.x, asd->explotionRenderRad(), 0, &dgColor.x);
				}
				break;
				case EXPLOSION:
				{
					Explosion* asd = (Explosion*)eff[i];
					vec3 pos = asd->getPos();
					renderPipe->renderExploEffect(&pos.x, asd->explosionRenderRad(), 0, &dgColor.x);
				}
				break;
				case CLEANSENOVA:
				{
					CleanseNova* asd = (CleanseNova*)eff[i];
					vec3 pos = asd->getPos();
					dgColor = CLEANSENOVACOLOR;
					renderPipe->renderExploEffect(&pos.x, asd->renderRad(), 0, &dgColor.x);
				}
				break;
				case BATTERY_SLOW:
				{
					BatteryFieldSlow* asd = (BatteryFieldSlow*)eff[i];
					vec3 pos = asd->getPos();
					dgColor = SLOWBUBBLECOLOR;
					renderPipe->renderExploEffect(&pos.x, asd->renderRad(), 0, &dgColor.x);
				}
					break;
				case BATTERY_SPEED:
				{
					BatteryFieldSpeed* asd = (BatteryFieldSpeed*)eff[i];
					vec3 pos = asd->getPos();
					dgColor = SPEEDBUBBLECOLOR;
					renderPipe->renderExploEffect(&pos.x, asd->renderRad(), 0, &dgColor.x);
				}
					break;
				case THERMITE_CLOUD:
				{
					ThermiteCloud* asd = (ThermiteCloud*)eff[i];
					vec3 pos = asd->getPos();
					renderPipe->renderExploEffect(&pos.x, asd->explosionRenderRad(), 0, &dgColor.x);
				}
				break;
				case VACUUM:
				{
					Vacuum* asd = (Vacuum*)eff[i];
					vec3 pos = asd->getPos();
					renderPipe->renderExploEffect(&pos.x, asd->renderRad(), 0, &dgColor.x);
				}
				break;
				case HEALTHPACK:
				{
					vec3 pos = eff[i]->getPos();
					if (eff[i]->getTeam() == 1)
						dgColor = TEAMTWOCOLOR;
					else
						dgColor = TEAMONECOLOR;
					renderPipe->renderExploEffect(&pos.x, eff[i]->getInterestingVariable(), 0, &dgColor.x);
				}
				break;
				case HSCPICKUP:
				{
					HSCPickup* temp = (HSCPickup*)eff[i];
					if (!temp->onCooldown())
					{
						vec3 pos = eff[i]->getPos();
						dgColor = vec3(1.0f, 0, 1.0f);
						renderPipe->renderExploEffect(&pos.x, eff[i]->getInterestingVariable(), 0, &dgColor.x);
					}
				}
				break;
				case DOUBLEDAMAGEPICKUP:
				{
					DoubleDamagePickup* temp = (DoubleDamagePickup*)eff[i];
					if (!temp->onCooldown())
					{
						vec3 pos = eff[i]->getPos();
						dgColor = vec3(1.0f, 0, 0);
						renderPipe->renderExploEffect(&pos.x, eff[i]->getInterestingVariable(), 0, &dgColor.x);

					}
				}
				break;
				}
			}
		}

		// render Decals
		renderPipe->renderDecals(game->getAllDecalRenderInfo(), game->getNrOfDecals());

		renderPipe->stopExecTimer(effectTime);

		renderPipe->finalizeRender();

		renderPipe->disableDepthTest();
		renderPipe->enableBlend();

		renderPipe->renderCrosshair(CROSSHAIR_TRAPPER_P);

		if (game->getPlayer(game->GetLocalPlayerId())->hitMarker > 0.0f)
		{
			renderPipe->renderCrosshair(CROSSHAIR_SHANKER_P);
			game->getPlayer(game->GetLocalPlayerId())->hitMarker -= dt;
		}
		

		//name rendering
		if (current == CLIENT)
		{
			for (int c = 0; c < 20; c++)
			{
				Player* p = game->getPlayer(c);

				if (p == nullptr || !p->isAlive())
					continue;

				int local = game->GetLocalPlayerId();
				Player* lP = game->getPlayer(local);
				if (game->spectateID != -1)
					lP = game->getPlayer(game->spectateID);

				if ((HackingDartModifier*)lP->searchModifierGet(HACKINGDARTMODIFIER))
					continue;

				if (p->getTeam() == lP->getTeam() || lP->getTeam() == 0)
				{
					mat4 bacon;
					vec3 pPos = p->getPos();

					vec3 dir = normalize(camPos - pPos);

					float rotXZ = -(atan2(dir.x, dir.z) - atan2(0, 1));

					mat4 rotH = mat4(cos(rotXZ), 0.0f, -sin(rotXZ), 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						sin(rotXZ), 0.0f, cos(rotXZ), 0.0f,
						0.0f, 0.0f, 0.0f, 0.0f);
					bacon = rotH * bacon;

					bacon[0].w = pPos.x;
					bacon[1].w = pPos.y + 0.5f;
					bacon[2].w = pPos.z;

					if (p->nameChanged)
					{
						renderPipe->setTextObjectText(namePlates[c], p->getName());
						p->nameChanged = false;
					}
					renderPipe->renderTextObjectWorldPos(namePlates[c], bacon);
				}
			}
			renderPipe->enableDepthTest();
		}

		if (renderUI) //Temp
			inGameUIUpdate();

		//render minimap
		if (i->getKeyInfo(GLFW_KEY_F))
			if (game->getPlayer(game->GetLocalPlayerId())->getLockedControls() == false)
				minimapRender();

		// render score screen
		if (i->getKeyInfo(GLFW_KEY_TAB))
			if (game->getPlayer(game->GetLocalPlayerId())->getLockedControls() == false)
				scoreboardRender();

		renderPipe->disableBlend();

		//viewing 3rd person anims in roam
		if (i->getKeyInfo(GLFW_KEY_P))
			cam->setCam(camPos, camDir);
		if (i->getKeyInfo(GLFW_KEY_L))
			cam->setCam(camPos, camDir);
	}
}

void Core::inGameUIUpdate() //Ingame ui update
{
	Player* local;
	if (game->spectateID == -1)
		local = game->getPlayer(game->GetLocalPlayerId());
	else
		local = game->getPlayer(game->spectateID);

	KingOfTheHill* koth = (KingOfTheHill*)game->getGameMode();

	int tTeam = local->getTeam();
	int tClass = local->getRole()->getRole();

	bool playerExist = false;

	if (tClass != ROLES::NROFROLES) //Checks so its acually a player and not a spectator
		if (tTeam != 0)
			playerExist = true;

	if (playerExist)
	{
		if (local->getHP() != uiManager->HUD.HP) //Health
		{
			uiManager->HUD.HP = local->getHP();
			uiManager->clearText(scaleAndText::HP);
			uiManager->setText(std::to_string(uiManager->HUD.HP), scaleAndText::HP);
			if ((float)local->getMaxHP() > 0)
			{
				uiManager->scaleBar(scaleAndText::HP, (float)(uiManager->HUD.HP) / (float)(local->getMaxHP()), true);
			}
			else
				console.printMsg("Error: Function inGameUIUpdate in Core, local player maxHp has a value of 0 or below", "System", 'S');
		}
		if (local->getSpecialMeter() != uiManager->HUD.specialMeter) //Special Meter
		{
			uiManager->HUD.specialMeter = local->getSpecialMeter();
			float procent = local->getSpecialMeter() / uiManager->HUD.maxSpecialMeter;
			uiManager->scaleBar(scaleAndText::AbilityMeter, procent, true);
		}
		if (local->getAmmo() != uiManager->HUD.ammo) //Ammo
		{
			uiManager->HUD.ammo = local->getAmmo();
			std::string nText = std::to_string(uiManager->HUD.ammo) + "/" + std::to_string(local->getMaxAmmo());
			uiManager->clearText(scaleAndText::Ammo);
			uiManager->setText(nText, scaleAndText::Ammo);
		}
	} //player stats on HUD end

	 //Checks if the teams token number have changed.
	if (koth->getRespawnTokens(1) != uiManager->HUD.teamOneTokens) //Team 1
	{
		uiManager->HUD.teamOneTokens = koth->getRespawnTokens(1);
		uiManager->clearText(scaleAndText::TicketBar1);
		uiManager->setText(std::to_string(uiManager->HUD.teamOneTokens), scaleAndText::TicketBar1);
		
		if ((float)(uiManager->HUD.maxTokens) > 0)
			uiManager->scaleBar(scaleAndText::TicketBar1, (float)uiManager->HUD.teamOneTokens / (float)(uiManager->HUD.maxTokens), true);
		else
			console.printMsg("Error: Function inGameUIUpdate in Core, HUD.maxTokens has a value of 0 or below", "System", 'S');

		if (koth->getState() == KOTHSTATE::ROUND)
		{
			uiManager->hideOrShowHideAble(hideAbleObj::TicketReducerTeam1, true);
			uiManager->HUD.ticketReducer1Counter = 0;
		}

		uiManager->HUD.scoreChanged = true;
	}
	if (koth->getRespawnTokens(2) != uiManager->HUD.teamTwoTokens) //Team 2
	{
		uiManager->HUD.teamTwoTokens = koth->getRespawnTokens(2);
		uiManager->clearText(scaleAndText::TicketBar2);
		uiManager->setText(std::to_string(uiManager->HUD.teamTwoTokens), scaleAndText::TicketBar2);
		
		if ((float)(uiManager->HUD.maxTokens) > 0)
			uiManager->scaleBar(scaleAndText::TicketBar2, (float)(uiManager->HUD.teamTwoTokens) / (float)(uiManager->HUD.maxTokens), true);
		else
			console.printMsg("Error: Function inGameUIUpdate in Core, HUD.maxTokens has a value of 0 or below", "System", 'S');

		if (koth->getState() == KOTHSTATE::ROUND)
		{
			uiManager->hideOrShowHideAble(hideAbleObj::TicketReducerTeam2, true);
			uiManager->HUD.ticketReducer2Counter = 0;
		}

		uiManager->HUD.scoreChanged = true;
	}

	//Checks if the teams score have changed.
	if (koth->getRoundWins(1) != uiManager->HUD.teamOneScore) //Team 1
	{
		if (koth->getRoundWins(1) != -1)
		{
			int difference = koth->getRoundWins(1) - uiManager->HUD.teamOneScore;

			uiManager->HUD.teamOneScore = koth->getRoundWins(1);
			uiManager->clearText(scaleAndText::Wins1);
			uiManager->setText(std::to_string(uiManager->HUD.teamOneScore), scaleAndText::Wins1);

			if (difference > 0 && difference < 4)
			{
				uiManager->changeTextureHideAble(hideAbleObj::ScoreAdderTeam1, difference - 1);
				uiManager->hideOrShowHideAble(hideAbleObj::ScoreAdderTeam1, true);
				uiManager->HUD.scoreAdder1Counter = 0;
			}
		}
		else
			console.printMsg("Error: Function inGameUIUpdate in Core, koth->getRoundWins(1) has a value of -1", "System", 'S');
	}
	if (koth->getRoundWins(2) != uiManager->HUD.teamTwoScore) //Team 2
	{
		if (koth->getRoundWins(2) != -1)
		{
			int difference = koth->getRoundWins(2) - uiManager->HUD.teamTwoScore;

			uiManager->HUD.teamTwoScore = koth->getRoundWins(2);
			uiManager->clearText(scaleAndText::Wins2);
			uiManager->setText(std::to_string(uiManager->HUD.teamTwoScore), scaleAndText::Wins2);

			if (difference > 0 && difference < 4)
			{
				uiManager->changeTextureHideAble(hideAbleObj::ScoreAdderTeam2, difference - 1);
				uiManager->hideOrShowHideAble(hideAbleObj::ScoreAdderTeam2, true);
				uiManager->HUD.scoreAdder2Counter = 0;
			}
		}
		else
			console.printMsg("Error: Function inGameUIUpdate in Core, koth->getRoundWins(1) has a value of -1", "System", 'S');
	}

	//Does this check so the code only runs one time each second
	if (int(koth->getTimer()) != uiManager->HUD.time)
	{
		uiManager->HUD.time = int(koth->getTimer());

		int minutes = uiManager->HUD.time * 0.01666;
		int seconds = uiManager->HUD.time - 60 * minutes;

		std::string sMinutes = "0";
		std::string sSeconds = "0";

		sMinutes += std::to_string(minutes);
		sSeconds += std::to_string(seconds);

		if (sMinutes.size() > 2)
			sMinutes = std::to_string(minutes);
		if (sSeconds.size() > 2)
			sSeconds = std::to_string(seconds);

		uiManager->clearText(scaleAndText::Time);
		uiManager->setText(sMinutes + ":" + sSeconds, scaleAndText::Time);

		if (koth->getState() == ROUND) //Tickets meter should only work during the rounds which is why this check is here.
		{
			if (!uiManager->HUD.firstSecondEachRound) //If it isn't the first second of the round
			{
				if (uiManager->HUD.ticketLostTimer == 0)
				{
					uiManager->scaleBar(scaleAndText::LoseTicketsMeter, 0.0f, true);
					uiManager->HUD.ticketLostTimer = uiManager->HUD.loseTicketPer;
				}
				else
				{
					if (uiManager->HUD.loseTicketPer > 0)
					{
						uiManager->scaleBar(scaleAndText::LoseTicketsMeter, (float)(uiManager->HUD.ticketLostTimer) / (float)(uiManager->HUD.loseTicketPer), true);
						uiManager->HUD.ticketLostTimer -= 1;
					}
					else
						console.printMsg("Error: Function inGameUIUpdate in Core, HUD.loseTicketPer has a value of 0 or below", "System", 'S');
				}
			}
			else
			{
				if (uiManager->HUD.loseTicketPer > 1)
				{
					uiManager->HUD.ticketLostTimer -= 1;
					uiManager->scaleBar(scaleAndText::LoseTicketsMeter, (float)(uiManager->HUD.ticketLostTimer) / (float)(uiManager->HUD.loseTicketPer), true);
					uiManager->HUD.ticketLostTimer -= 1;
				}
				else
					console.printMsg("Error: Function inGameUIUpdate in Core, HUD.loseTicketPer has a value of 1 or below at the start of the round", "System", 'S');
			}
			uiManager->HUD.firstSecondEachRound = false;
		}
		else //In other states it resets but only once.
		{
			uiManager->HUD.firstSecondEachRound = true;
			uiManager->HUD.ticketLostTimer = 14;
			uiManager->scaleBar(scaleAndText::LoseTicketsMeter, 1.0f, true);
		}

		if (uiManager->HUD.bannerCounter == 3)
		{
			uiManager->hideOrShowHideAble(hideAbleObj::Banner, false);
			uiManager->HUD.bannerCounter = 4;
		}
		else
			uiManager->HUD.bannerCounter++;
		if (uiManager->HUD.scoreAdder1Counter == 2)
		{
			uiManager->hideOrShowHideAble(hideAbleObj::ScoreAdderTeam1, false);
			uiManager->HUD.scoreAdder1Counter = 3;
		}
		else
			uiManager->HUD.scoreAdder1Counter++;
		if (uiManager->HUD.scoreAdder2Counter == 2)
		{
			uiManager->hideOrShowHideAble(hideAbleObj::ScoreAdderTeam2, false);
			uiManager->HUD.scoreAdder2Counter = 3;
		}
		else
			uiManager->HUD.scoreAdder2Counter++;
		if (uiManager->HUD.ticketReducer1Counter == 2)
		{
			uiManager->hideOrShowHideAble(hideAbleObj::TicketReducerTeam1, false);
			uiManager->HUD.ticketReducer1Counter = 3;
		}
		else
			uiManager->HUD.ticketReducer1Counter++;
		if (uiManager->HUD.ticketReducer2Counter == 2)
		{
			uiManager->hideOrShowHideAble(hideAbleObj::TicketReducerTeam2, false);
			uiManager->HUD.ticketReducer2Counter = 3;
		}
		else
			uiManager->HUD.ticketReducer2Counter++;
	}

	//Checks to see if the banners "Final Assult" or Hold Your Ground" should be shown.
	if (lowTicketsFirstTime && uiManager->HUD.scoreChanged) 
	{
		if (uiManager->HUD.teamOneTokens <= 3 && uiManager->HUD.teamTwoTokens <= 3)
		{
			uiManager->changeTextureHideAble(hideAbleObj::Banner, BannerTextureIDs::FinalAssult);
			uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
			uiManager->HUD.bannerCounter = 0;
			lowTicketsFirstTime = false;
		}
		else if (uiManager->HUD.teamOneTokens <= 3)
		{
			if (tTeam == 1)
			{
				uiManager->changeTextureHideAble(hideAbleObj::Banner, BannerTextureIDs::FinalAssult);
				uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
			}
			else
			{
				uiManager->changeTextureHideAble(hideAbleObj::Banner, BannerTextureIDs::HoldYourGround);
				uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
			}
			uiManager->HUD.bannerCounter = 0;
			lowTicketsFirstTime = false;
		}
		else if (uiManager->HUD.teamTwoTokens <= 3)
		{
			if (tTeam == 2)
			{
				uiManager->changeTextureHideAble(hideAbleObj::Banner, BannerTextureIDs::FinalAssult);
				uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
			}
			else
			{
				uiManager->changeTextureHideAble(hideAbleObj::Banner, BannerTextureIDs::HoldYourGround);
				uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
			}
			uiManager->HUD.bannerCounter = 0;
			lowTicketsFirstTime = false;
		}
		uiManager->HUD.scoreChanged = false;
	}
	else
		uiManager->HUD.scoreChanged = false;

	uiManager->renderHideAble();
	uiManager->inGameRender();

	double x = (0.0);
	double y = (0.0);
	//Get mouse position
	i->getCursor(x, y);
	double tX = (x / (double)winX) * 2 - 1.0;
	double tY = (-y / (double)winY) * 2 + 1.0;

	if (uiManager->getHoverCheckBool())
	{
		if (i->justPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			int eventIndex = uiManager->collisionCheck(glm::vec2((float)tX, (float)tY));
			switch (eventIndex)
			{
			case 20: //Team 1
				if (current == ROAM)
				{
					uiManager->setTeamColor(TeamColors::TeamOne);
					roamHandleCmds("/team 2");
				}
				else
					clientHandleCmds("/team 2");
				break;
			case 21: //Team 2
				if (current == ROAM)
				{
					uiManager->setTeamColor(TeamColors::TeamTwo);
					roamHandleCmds("/team 1");
				}
				else
					clientHandleCmds("/team 1");
				break;
			case 30: //Class 1
				if (current == ROAM)
					roamHandleCmds("/role 1");
				else
					clientHandleCmds("/role 1");
				break;
			case 31: //Class 2
				if (current == ROAM)
					roamHandleCmds("/role 2");
				else
					clientHandleCmds("/role 2");
				break;
			case 32: //Class 3
				if (current == ROAM)
					roamHandleCmds("/role 3");
				else
					clientHandleCmds("/role 3");
				break;
			case 33: //Class 4
				if (current == ROAM)
					roamHandleCmds("/role 4");
				else
					clientHandleCmds("/role 4");
				break;
			case 34: //Class 5
				if (current == ROAM)
					roamHandleCmds("/role 5");
				else
					clientHandleCmds("/role 5");
				break;
			case 40: //Continue
				
				break;
			case 41: //Settings
				break;
			case 42: //Quit
				if (current == ROAM)
					roamHandleCmds("/disconnect");
				else
					clientHandleCmds("/disconnect");
				break;
			default:
				break;
			}
		}
		else
			uiManager->hoverCheck(glm::vec2((float)tX, (float)tY));
	}
}

void Core::handleCulling()
{
	CameraInput* cam = CameraInput::getCam();
	if (game->spectateID == -1) // if we are not spectating some one
	{
		int interarr[2];
		int count;
		//check vs roomboxes to verify. Roomboxes have priority if only intersecting with one room
		game->cullingPointvsRoom(&cam->getPos(), interarr, count, 2);
		if (count == 1)
		{
			if (cam->roomID != interarr[0])
			{
				cam->roomID = interarr[0];
				//printf("RoomIntersect Cam setRoomID %d", interarr[0]);
			}
		}
		//else
		//{
		//	int newRoom = renderPipe->portalIntersection((float*)&lastCampos, (float*)&cam->getPos(), cam->roomID);
		//	if (newRoom != -1)
		//	{
		//		cam->roomID = newRoom;
		//		//printf("Portal Cam setRoomID %d", newRoom);
		//	}
		//}

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
				game->cullingPointvsRoom(&p->getPos(), interarr, count, 2);
				if (count == 1)
				{
					if (p->roomID != interarr[0])
					{
						p->roomID = interarr[0];
						//printf("RoomIntersect Player setRoomID %d", interarr[0]);
					}
				}
				//else
				//{
				//	int newRoom = renderPipe->portalIntersection((float*)&lastPlayerPos, (float*)&p->getPos(), p->roomID);
				//	if (newRoom != -1)
				//	{
				//		p->roomID = newRoom;
				//		//printf("Portal Player setRoomID %d", interarr[0]);
				//	}
				//	lastPlayerPos = p->getPos();
				//}
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

		for (size_t i = 0; i < MAX_CONNECT; i++)
		{
			namePlates[i] = renderPipe->createTextObject("", 128, vec2(winX / 2, winY / 2));
		}

		leaderBoardTextID = renderPipe->createTextObject("", 20, vec2(winX / 2, winY / 2));
		leaderBoardSmallTextID = renderPipe->createTextObject("", 15, vec2(winX / 2, winY / 2));

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
			string print("X: " + to_string(cam->getPos().x) + "  Y: " + to_string(cam->getPos().y) + "  Z: " + to_string(cam->getPos().z) + "  FPS: " + to_string(fps));
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

void Core::sendCapPointBoxes()
{
	if (renderPipe != nullptr)
	{
		int count = 0;
		void* capBoxes = renderPipe->getCapPointsAsPoint(count);

		if (capBoxes != nullptr)
		{
			game->sendCapBoxes(count, capBoxes);
		}
	}
}

void Core::sendRoomBoxes()
{
	if (renderPipe != nullptr)
	{
		void* send = renderPipe->getRoomBoxes();
		if (send != nullptr)
		{
			game->sendRoomBoxes(send);
		}
	}
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

bool Core::windowVisible()
{
	if (glfwWindowShouldClose(win))
	{
		glfwHideWindow(win);
		closeGame = true;
	}
	return !closeGame;
}

void Core::disconnect()
{
	saveSettings();
	if (top)
		delete top;
	top = nullptr;
	game->release();
	game = nullptr;
	//if (renderPipe != nullptr) //server might not have a renderpipe
	//{
	//	renderPipe->release();
	//	renderPipe = nullptr;
	//}
	current = Gamestate::START;
	subState = 0;

	uiManager->setRoleBool(false);

	uiManager->setHoverCheckBool(true);
	uiManager->setRoleBool(false);
	uiManager->setFirstMenuSet(false);
	cursorInvisible = false;
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	uiManager->LoadNextSet(UISets::Menu, winX, winY);
	uiManager->setMenu(MainMenu::StartMenu);
	if (GetSound())
	{
		GetSound()->playMusic(mainMenu);
		GetSound()->setVolumeMusic(50);
		GetSound()->SetFading(false);
	}
}

void Core::showTeamSelect()
{
	if (startTeamSelect)
	{
		uiManager->setHoverCheckBool(true);
		uiManager->LoadNextSet(UISets::InGame, winX, winY);
		uiManager->setFirstMenuSet(true);
		uiManager->setMenu(InGameUI::TeamSelect);
	}
	else
	{
		uiManager->setHoverCheckBool(false);
		uiManager->LoadNextSet(UISets::InGame, winX, winY);
		uiManager->setFirstMenuSet(false);
		uiManager->setMenu(InGameUI::GUI);
	}
}

void Core::showClassSelect()
{
	uiManager->setHoverCheckBool(true);
	game->getPlayer(game->GetLocalPlayerId())->setLockedControls(true);
	cursorInvisible = false;
	uiManager->setFirstMenuSet(true);
	uiManager->setMenu(InGameUI::ClassSelect);
}

void Core::menuIpKeyInputUpdate()
{
	Input* i = Input::getInput();
	for (int c = 0; c < VALIDKEYSIP; c++)
	{
		if (i->justPressed(validKeyboardInputs[c]))
		{
			if (ipNrOfKeys < 15)
			{
				char ch = i->keyToChar(validKeyboardInputs[c]);
				std::string fromChar = "";
				fromChar += ch;

				uiManager->setText(fromChar, scaleAndText::IP); //Set ip object
				ipNrOfKeys++;
			}
		}
	}
	if (i->justPressed(GLFW_KEY_BACKSPACE))
	{
		uiManager->removeLastInput(scaleAndText::IP); //remove from ip object
		ipNrOfKeys--;
	}
}

void Core::menuNameKeyInputUpdate()
{
	Input* i = Input::getInput();
	for (int c = 0; c < VALIDKEYSNAME; c++)
	{
		if (i->justPressed(validKeyboardInputs2[c]))
		{
			if (nameNrOfKeys < 12)
			{
				char ch = i->keyToChar(validKeyboardInputs2[c]);
				if (!i->getKeyInfo(GLFW_KEY_LEFT_SHIFT))
					ch = tolower(ch);
				std::string fromChar = "";
				fromChar += ch;

				uiManager->setText(fromChar, scaleAndText::Name); //Set ip object

				nameNrOfKeys++;
			}
		}
	}
	if (i->justPressed(GLFW_KEY_BACKSPACE))
	{
		uiManager->removeLastInput(scaleAndText::Name); //remove from ip object
		nameNrOfKeys--;
	}
}

void Core::scoreboardRender()
{
	vector<int>* membersTeam1 = game->getTeamConIds(1);
	vector<int>* membersTeam2 = game->getTeamConIds(2);

	renderPipe->renderScoreBoard(membersTeam1[0].size(), membersTeam2[0].size());
	Player* p = 0;

	int sortedID[20];

	if (membersTeam1[0].size() > 0)
	{
		sortedID[0] = membersTeam1[0][0];
		//insertion sort
		for (int n = 1; n < membersTeam1[0].size(); n++)
		{
			sortedID[n] = membersTeam1[0][n];
			
			for (int k = n; k > 0; k--)
			{
				if (game->getPlayer(sortedID[k])->getKills() > game->getPlayer(sortedID[k - 1])->getKills())
				{
					//swap
					int temp = sortedID[k];
					sortedID[k] = sortedID[k - 1];
					sortedID[k - 1] = temp;
				}
				else
				{
					break;
				}
			}
		}
	}

	if (membersTeam1[0].size() > 0)
	{
		//team 1
		vec2 startpos = vec2(125, 260);

		//Render headder text (kills deaths)
		renderPipe->setTextObjectText(leaderBoardSmallTextID, "Kills");
		renderPipe->setTextPos(leaderBoardSmallTextID, startpos + vec2(220, -48));
		renderPipe->renderTextObject(leaderBoardSmallTextID);

		renderPipe->setTextObjectText(leaderBoardSmallTextID, "Deaths");
		renderPipe->setTextPos(leaderBoardSmallTextID, startpos + vec2(300, -48));
		renderPipe->renderTextObject(leaderBoardSmallTextID);

		for (int n = 0; n < membersTeam1[0].size(); n++)
		{
			p = game->getPlayer(sortedID[n]);
			if (p)
			{
				//render name
				renderPipe->setTextObjectText(leaderBoardTextID, p->getName());
				renderPipe->setTextPos(leaderBoardTextID, startpos + vec2(0, 79 * n));
				renderPipe->renderTextObject(leaderBoardTextID);

				//render role
				renderPipe->setTextObjectText(leaderBoardSmallTextID, p->getRole()->getRoleAsString());
				renderPipe->setTextPos(leaderBoardSmallTextID, startpos + vec2(0, 79 * n + 15));
				renderPipe->renderTextObject(leaderBoardSmallTextID);

				//render kills / deaths
				renderPipe->setTextObjectText(leaderBoardTextID, to_string(p->getKills()));
				renderPipe->setTextPos(leaderBoardTextID, startpos + vec2(220 + 40, 79 * n + 10));
				renderPipe->renderTextObject(leaderBoardTextID);

				renderPipe->setTextObjectText(leaderBoardTextID, to_string(p->getDeaths()));
				renderPipe->setTextPos(leaderBoardTextID, startpos + vec2(300 + 40, 79 * n + 10));
				renderPipe->renderTextObject(leaderBoardTextID);
			}
		}
	}

	//sort team 2

	if (membersTeam2[0].size() > 0)
	{
		sortedID[0] = membersTeam2[0][0];
		//insertion sort
		for (int n = 1; n < membersTeam2[0].size(); n++)
		{
			sortedID[n] = membersTeam2[0][n];

			for (int k = n; k > 0; k--)
			{
				if (game->getPlayer(sortedID[k])->getKills() > game->getPlayer(sortedID[k - 1])->getKills())
				{
					//swap
					int temp = sortedID[k];
					sortedID[k] = sortedID[k - 1];
					sortedID[k - 1] = temp;
				}
				else
				{
					break;
				}
			}
		}
	}

	//team 2
	if (membersTeam2[0].size() > 0)
	{
		vec2 startpos = vec2(760, 260);

		//Render headder text (kills deaths)
		renderPipe->setTextObjectText(leaderBoardSmallTextID, "Kills");
		renderPipe->setTextPos(leaderBoardSmallTextID, startpos + vec2(220, -48));
		renderPipe->renderTextObject(leaderBoardSmallTextID);

		renderPipe->setTextObjectText(leaderBoardSmallTextID, "Deaths");
		renderPipe->setTextPos(leaderBoardSmallTextID, startpos + vec2(300, -48));
		renderPipe->renderTextObject(leaderBoardSmallTextID);

		for (int n = 0; n < membersTeam2[0].size(); n++)
		{
			p = game->getPlayer(sortedID[n]);
			if (p)
			{
				//render name
				renderPipe->setTextObjectText(leaderBoardTextID, p->getName());
				renderPipe->setTextPos(leaderBoardTextID, startpos + vec2(0, 79 * n));
				renderPipe->renderTextObject(leaderBoardTextID);

				//render role
				renderPipe->setTextObjectText(leaderBoardSmallTextID, p->getRole()->getRoleAsString());
				renderPipe->setTextPos(leaderBoardSmallTextID, startpos + vec2(0, 79 * n + 20));
				renderPipe->renderTextObject(leaderBoardSmallTextID);

				//render kills / deaths
				renderPipe->setTextObjectText(leaderBoardTextID, to_string(p->getKills()));
				renderPipe->setTextPos(leaderBoardTextID, startpos + vec2(220 + 40, 79 * n + 10));
				renderPipe->renderTextObject(leaderBoardTextID);

				renderPipe->setTextObjectText(leaderBoardTextID, to_string(p->getDeaths()));
				renderPipe->setTextPos(leaderBoardTextID, startpos + vec2(300 + 40, 79 * n + 10));
				renderPipe->renderTextObject(leaderBoardTextID);
			}
		}
	}
}

void Core::minimapRender()
{
	Player* p = game->getPlayer(game->GetLocalPlayerId());
	Player* p2;
	int memb = 0;
	if (p)
	{
		vector<int>* members = game->getTeamConIds(p->getTeam());
		int membersize = members->size();

		vec3* data = new vec3[membersize * 2];
		int counter = 0;
		for (int n = 0; n < membersize; n++)
		{
			p2 = game->getPlayer(members[0][counter]);
			if (p2->isAlive())
			{
				data[n * 2] = p2->getPos();
				data[n * 2 + 1] = p2->getDir();
			}
			else
			{
				n--;
				membersize--;
			}
			counter++;
		}

		vec3 playerposs = p->getPos();
		vec3 playerdirr = p->getDir();

		KingOfTheHill* koth = (KingOfTheHill*)game->getGameMode();

		renderPipe->renderMinimap(&playerposs.x, &playerdirr.x, &data[0].x, membersize, koth->getCapturePoint());
		delete[] data;
	}
}