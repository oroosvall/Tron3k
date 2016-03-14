#include "Core.h"

void Core::init()
{
	round = 0;
	warmUpBannerActive = false;
	calcTimer = false;
	escActive = false;
	inGameSettings = false;
	justSetFullScreenMainMenu = false;
	justSetFullScreenIngame = false;
	justSetFullScreen = false;
	firstTimeInWarmUp = true;
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
	//winX = winY = 1000;
	winX = 1280; winY = 720;
	//winX = 1920, winY = 1080;
	//winX = 1600, winY = 900;

	createWindow(winX, winY, fullscreen);

	glfwSwapBuffers(win);

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
	uiManager->LoadNextSet(UISets::Menu, winX, winY); //Load the first set of menus.
	uiManager->setMenu(MainMenu::StartMenu); //Set start menu as the current menu
	optionsDataSize = 12;
	optionsSavedData = new float[optionsDataSize];
	soundSavedData = new float[optionsDataSize];
	optionsSavedData[0] = 1.0f;
	soundSavedData[0] = 1.0f;
	soundSavedData[1] = 0.30f;
	for (int i = 2; i < optionsDataSize - 3; i++)
	{
		optionsSavedData[i] = 0.15;
		soundSavedData[i] = 50.0f;
	}
	optionsSavedData[9] = 0.;
	soundSavedData[9] = 0.0f;
	optionsSavedData[10] = 0.15;
	soundSavedData[10] = 0.0f;
	optionsSavedData[11] = -0.15;
	soundSavedData[11] = 0.0f;

	uiManager->HUD.skipBannerUpdate = true;
	uiManager->HUD.activeBanner = -1;
	uiManager->HUD.nrOfBanners = 5;
	uiManager->HUD.bannerCounter = 0;
	uiManager->HUD.bannerMaxTime.push_back(3);
	uiManager->HUD.bannerMaxTime.push_back(10);
	uiManager->HUD.bannerMaxTime.push_back(3);
	uiManager->HUD.bannerMaxTime.push_back(3);
	uiManager->HUD.bannerMaxTime.push_back(99);


	uiManager->setOptionsSaved(optionsSavedData);

	renderUI = false;
	startTeamSelect = true; //Temp
	renderMenu = true;
	menuIpKeyListener = false;
	menuNameKeyListener = false;
	menuSensKeyListener = false;

	nameNrOfKeys = 0;
	ipNrOfKeys = 0;
	senseNrOfKeys = 0;
}

Core::~Core()
{
	if (game != nullptr && current != SERVER)
	{
		if (game->getPlayer(game->GetLocalPlayerId()) != nullptr)
			saveControls();
	}

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
	{
		delete uiManager;
		uiManager = nullptr;
	}
	if (optionsSavedData != nullptr)
	{
		delete[] optionsSavedData;
		optionsSavedData = nullptr;
	}
	if (soundSavedData != nullptr)
	{
		delete[] soundSavedData;
		soundSavedData = nullptr;
	}
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

	if (!controlsLoaded && current != SERVER)
	{
		if (game != nullptr)
		{
			int id = game->GetLocalPlayerId();
			if (id != -1)
			{
				Player* p = game->getPlayer(id);
				if (p != nullptr)
				{
					loadControls();
					controlsLoaded = true;
				}
			}
		}
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
	if ((menuNameKeyListener || menuIpKeyListener || menuSensKeyListener) == true)
		otherListeners = false;

	if (otherListeners)
		console.update(_name); //Updates to check for new messages and commands
	else if (menuIpKeyListener)
		menuIpKeyInputUpdate(); //Updates to check input for the ip window
	else if(menuNameKeyListener)
		menuNameKeyInputUpdate(); //Updates to check input for the name window
	else if (menuSensKeyListener)
		menuSensitivityKeyInputUpdate();

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
						if (!console.getInChatMode())
						{
							renderPipe->setChatTypeMessage("");
							if (cursorInvisible)
								p->setLockedControls(false);
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
				//CameraInput* cam = CameraInput::getCam();
				//cam->setPlaybackSpeed(playbackSpeed);
			}
			if (i->justPressed(GLFW_KEY_6))
			{
				playbackSpeed *= 0.5f;
				if (playbackSpeed < 0.03125f)
					playbackSpeed = 0.03125f;
				//CameraInput* cam = CameraInput::getCam();
				//cam->setPlaybackSpeed(playbackSpeed);
			}
			if (i->justPressed(GLFW_KEY_0))
				playbackSpeed = 0.0f;

			if (i->justPressed(GLFW_KEY_9))
				playbackSpeed = 1.0f;

			if (renderPipe)
				if (i->justPressed(GLFW_KEY_8))
					renderPipe->reloadShaders();
		}
	}

	dt *= playbackSpeed;

	timepass += dt;

	switch (current)
	{
	case START:		upStart(dt);	break;
	case ROAM:		upRoam(dt);		break;
	case CLIENT:	upClient(dt);	break;
	case SERVER:	upServer(dt);	break;
	default:						break;
	}

	i->clearOnPress();
	console.discardCommandAndLastMsg();

	if (current != SERVER)
	{
		int swapTime = renderPipe->startExecTimer("Swap");
		if(current == CLIENT || current == ROAM)
			renderPipe->renderChatText();
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

		renderIntoFly(dt);

		if (renderMenu)
			upMenu(dt);
		break;
	}

}

void Core::upMenu(float dt)
{
	//renderUI = true;
	double tX = (0.0);
	double tY = (0.0);
	//Get mouse position
	i->getCursor(tX, tY);
	tX = (tX / (double)winX) * 2 - 1.0; // (x/ResolutionX) * 2 - 1
	tY = (-tY / (double)winY) * 2 + 1.0; // (y/ResolutionY) * 2 - 1

	cursorInvisible = false;
	uiManager->menuRender();

	if (justSetFullScreen)
	{
		//uiManager->setFirstMenuSet(false);
		uiManager->setMenu(MainMenu::StartMenu); //Set start menu as the current menu
		//uiManager->setMenu(MainMenu::Settings);
		justSetFullScreenMainMenu = false;
		justSetFullScreen = false;
		if (fullscreen)
			uiManager->changeTex(9, 1);
		else
			uiManager->changeTex(9, 0);
	}

	if (i->justPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		float newVolume = 1.0;
		menuIpKeyListener = false;
		menuNameKeyListener = false;
		menuSensKeyListener = false;
		int eventIndex = uiManager->collisionCheck(glm::vec2((float)tX, (float)tY), newVolume);
		switch (eventIndex)
		{
		case 0: //Roam
			last = current;
			current = ROAM;
			optionsSavedData = uiManager->getOptionsSaved();
			uiManager->LoadNextSet(UISets::InGame, winX, winY);
			uiManager->setRoleBool(false);
			uiManager->setFirstMenuSet(false);
			uiManager->setMenu(InGameUI::TeamSelect);
			subState = 0;
			renderMenu = false;
			renderUI = true;

			uiManager->setHoverCheckBool(true);
			break;
		case 1: //MainMenu -> Multiplayer
			uiManager->setFirstMenuSet(true);
			uiManager->setMenu(MainMenu::Multiplayer);
			break;
		case 2: //MainMenu -> Settings
		{
			uiManager->setFirstMenuSet(true);
			uiManager->setMenu(MainMenu::Settings);
			if (fullscreen)
				uiManager->changeTex(9, 1);
			else
				uiManager->changeTex(9, 0);

			std::string input = std::to_string(serverCam->getSensitivity());
			senseNrOfKeys = input.size();

			uiManager->clearText(scaleAndText::Sense, MainMenu::Settings);

			if (senseNrOfKeys > 4)
			{
				input.erase(4, input.size());
				senseNrOfKeys = input.size();
			}

			if (senseNrOfKeys != 1)
				uiManager->setText(input, scaleAndText::Sense, MainMenu::Settings); //Set sense object
			else
				uiManager->setText(input + ".", scaleAndText::Sense, MainMenu::Settings); //Set sense object
			break;
		}
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

			fullscreen = false;
			createWindow(640, 420, fullscreen);

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

			last = current;
			current = Gamestate::CLIENT; //Start the game as a client
			client_record = false;
			client_playback = false;
			subState = 0;
			break;
		}
		case 7: //Back
			nameNrOfKeys = 0;
			ipNrOfKeys = 0;
			menuSensKeyListener = false;
			senseNrOfKeys = 0;

			if (uiManager->getCurrentMenu() == MainMenu::Settings)
			{
				SoundPlayer* sound = GetSound();
				if (sound != nullptr)
				{
					sound->SetMasterVolume(soundSavedData[1]); //0-1
					sound->SetFootstepsVolume((int)soundSavedData[2]); //0-100
					sound->SetGunsVolume((int)soundSavedData[3]); //0-100
					sound->SetEffectVolume((int)soundSavedData[4]); //0-100
					sound->SetAmbientVolume((int)soundSavedData[5]); //0-100
					sound->SetAnnouncerVolume((int)soundSavedData[6]); //0-100
					sound->setVolumeMusic((int)soundSavedData[7]); //0-100
					//sense
				}
				uiManager->setOptionsSaved(optionsSavedData);
			}
			uiManager->setMenu(MainMenu::Back); //Last menu

			if (inGameSettings)
			{
				if (uiManager->getCurrentMenu() == -1)
				{
					Gamestate tmp = current;
					current = last;
					last = tmp;
					subState = 2;

					uiManager->LoadNextSet(UISets::InGame, winX, winY);
					uiManager->setFirstMenuSet(false);
					uiManager->setMenu(InGameUI::GUI);
					uiManager->setMenu(InGameUI::ESC);
					renderMenu = false;
					renderUI = true;
					inGameSettings = false;
					calcTimer = true;

					//Set HUD stats
					setHUDText(InGameUI::GUI);

					if (warmUpBannerActive)
					{
						uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
						uiManager->resetHidableWorldMatrix(hideAbleObj::Banner, 0);
						uiManager->setHideableWorldMatrix(hideAbleObj::Banner, 0, vec2(0.0, 1.0));
						uiManager->changeTextureHideAble(hideAbleObj::Banner, 0, BannerTextureIDs::WarmUp);
						uiManager->HUD.activeBanner = ActiveBannerID::WarmUpRound;
						uiManager->HUD.skipBannerUpdate = true;
						uiManager->HUD.bannerCounter = 0;
					}
					playerIconsSet();
				}
			}
			break;
		case 8: //Settings -> confirm
		{
			menuSensKeyListener = false;
			senseNrOfKeys = 0;

			std::string temp = uiManager->getText(scaleAndText::Sense, MainMenu::Settings);
			currentSense = std::stof(temp);
			serverCam->setSensitivity(currentSense);

			SoundPlayer* sound = GetSound();
			if(sound != nullptr)
				soundSavedData = sound->GetAllSoundAsAList();//GetAllSoundAsAList();
			sound = nullptr;

			clientHandleCmds("/sens" + std::to_string(currentSense));
			optionsSavedData = uiManager->getOptionsSaved();
			uiManager->setMenu(MainMenu::Back);

			if (justSetFullScreenIngame || justSetFullScreenMainMenu)
			{
				if (fullscreen)
					fullscreen = false;
				else
					fullscreen = true;
				recreate = true;
			}
			else if (inGameSettings)
			{
				if (uiManager->getCurrentMenu() == -1)
				{
					Gamestate tmp = current;
					current = last;
					last = tmp;
					subState = 2;

					uiManager->LoadNextSet(UISets::InGame, winX, winY);
					uiManager->setFirstMenuSet(false);
					uiManager->setMenu(InGameUI::GUI);
					uiManager->setMenu(InGameUI::ESC);
					renderMenu = false;
					renderUI = true;
					inGameSettings = false;

					calcTimer = true;

					//Set HUD stats
					setHUDText(InGameUI::GUI);

					if (warmUpBannerActive)
					{
						uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
						uiManager->resetHidableWorldMatrix(hideAbleObj::Banner, 0);
						uiManager->setHideableWorldMatrix(hideAbleObj::Banner, 0, vec2(0.0, 1.0));
						uiManager->changeTextureHideAble(hideAbleObj::Banner, 0, BannerTextureIDs::WarmUp);
						uiManager->HUD.activeBanner = ActiveBannerID::WarmUpRound;
						uiManager->HUD.skipBannerUpdate = true;
						uiManager->HUD.bannerCounter = 0;
					}
					playerIconsSet();
				}
			}
			break;
		}
		case 10: //Ip input
			menuIpKeyListener = true;
			break;
		case 11: //Port
			menuNameKeyListener = true;
			break;
		case 12: //Master sound
		{
			SoundPlayer* sound = GetSound();
			if(sound != nullptr)
				sound->SetMasterVolume(newVolume);

			sound = nullptr;
			break;
		}
		case 13: //Footsteps sound
		{
			SoundPlayer* sound = GetSound();
			int volume = newVolume * 100;
			if (sound != nullptr)
				sound->SetFootstepsVolume(volume);

			sound = nullptr;
			break;
		}
		case 14: //Weapon sound
		{
			SoundPlayer* sound = GetSound();
			int volume = newVolume * 100;
			if (sound != nullptr)
				sound->SetGunsVolume(volume);//(int)newVolume * 100);
			sound = nullptr;
			break;
		}
		case 15: //Effects sound
		{
			SoundPlayer* sound = GetSound();
			int volume = newVolume * 100;
			if (sound != nullptr)
				sound->SetEffectVolume(volume);
			sound = nullptr;
			break;
		}
		case 16: //Ambient sound
		{
			SoundPlayer* sound = GetSound();
			int volume = newVolume * 100;
			if (sound != nullptr)
				sound->SetAmbientVolume(volume);
			sound = nullptr;
			break;
		}
		case 17: //Music
		{
			SoundPlayer* sound = GetSound();
			int volume = newVolume * 100;
			if (sound != nullptr)
				sound->setVolumeMusic(volume);
			sound = nullptr;
			break;
		}
		case 18: //Announcer sound
		{
			SoundPlayer* sound = GetSound();
			int volume = newVolume * 100;
			if (sound != nullptr)
				sound->SetAnnouncerVolume(volume);
			sound = nullptr;
			break;
		}
		case 19: //Sensitivity
			menuSensKeyListener = true;
			//currentSense = newVolume;
			break;
		case 20: //Fullscreen
			//if (fullscreen)
			//	fullscreen = false;
			//else
			//	fullscreen = true;
			//recreate = true;
			if (inGameSettings)
				justSetFullScreenIngame = true;
			else
				justSetFullScreenMainMenu = true;
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


		std::vector<MovableParticle*>* movable = game->getAllMovableParticle();
		for (size_t i = 0; i < movable->size(); i++)
		{
			if (!(*movable)[i]->created)
			{
				(*movable)[i]->id = renderPipe->createMappedParticleEffect(movable->at(i)->bt, *(*movable)[i]->pPos, (*movable)[i]->dir, (*movable)[i]->color);
				(*movable)[i]->created = true;
			}
			else
			{
				if (!(*movable)[i]->dead)
					renderPipe->moveMappedParticleEffect((*movable)[i]->id, *(*movable)[i]->pPos);
			}
			if ((*movable)[i]->dead)
			{
				renderPipe->removeMappedParticleEffect((*movable)[i]->id);
				(*movable)[i]->allowRemove = true;
			}
		}

		std::vector<HitPosAndDirParticle> hitpositions = game->getAllBulletHitPlayerPos();
		for (size_t i = 0; i < hitpositions.size(); i++)
		{
			renderPipe->createTimedParticleEffect(hitpositions[i].btype, hitpositions[i].pos, hitpositions[i].dir, hitpositions[i].color);
		}

		game->clearAllBulletHitPlayerPos();

		std::vector<EffectParticle> effectpositions = game->getAllEffectParticleSpawn();
		for (size_t i = 0; i < effectpositions.size(); i++)
		{
			renderPipe->createTimedParticleEffect(effectpositions[i].etype, effectpositions[i].pos, effectpositions[i].color);
		}

		game->clearAllEffectParticleSpawn();


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
			uiManager->HUD.ammo = p->getAmmo();

			std::string nText = hudTextOutPutManager(false, uiManager->HUD.ammo, p->getMaxAmmo());//std::to_string(p->getAmmo()) + "/" + std::to_string(p->getMaxAmmo());
			uiManager->clearText(scaleAndText::Ammo, InGameUI::GUI);
			uiManager->setText(nText, scaleAndText::Ammo, InGameUI::GUI);

			switch (wt)
			{
			case 0: //Pulse Rifle
				uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::PulseRifle);
				break;
			case 2: //Disc Gun
				uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::Discgun);
				break;
			case 3: //Melee
				uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::Tail);
				break;
			case 4: //Battery slow, manipulator
			case 5: //Battery speed
				uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::BatteryShots);
				break;
			case 6: //Fusion Cannon
				uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::FusionCannon);
				break;
			case 9: //Grenade Launcher
				uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::GranadeLauncher);
				break;
			case 10: //Shotgun
				uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::Shotgun);
				break;
			}

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
			{
				if (effectHitsOnPlayer[c].et == EFFECT_TYPE::EXPLOSION)
					int x = 0;
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

		std::vector<EffectTimeOutInfo> effectTimeOut = game->getAllTimedOutEffects();
		if (effectTimeOut.size() != 0)
		{
			for (unsigned int c = 0; c < effectTimeOut.size(); c++)
			{
				game->handleEffectTimeOuts(effectTimeOut[c]);
			}
			game->clearTimedOutEffects();
		}

		game->fixLocalPlayerCamera(dt);

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
			uiManager->stopRendering(5, false, InGameUI::ClassSelect);
			renderMenu = false;
			renderUI = true;

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

				Player* camPlayer = nullptr;
				if (game->spectateID > -1)
				{
					camPlayer = game->getPlayer(game->spectateID);
				}
				else
					camPlayer = game->getPlayer(game->GetLocalPlayerId());

				if (firstTimeInEnd && tmp == KOTHSTATE::ENDMATCH)
				{
					GAMEMODE_MSG tMode = k->getLastMsg();
					int tTeam = camPlayer->getTeam();

					round = 0;

					if (tMode == GAMEMODE_MSG::MATCH_WIN_TEAM1)
					{
						if (tTeam == 1)
						{
							uiManager->changeTextureHideAble(hideAbleObj::Banner, 0, BannerTextureIDs::Victory);
							uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
						}
						else if (tTeam == 2)
						{
							uiManager->changeTextureHideAble(hideAbleObj::Banner, 0, BannerTextureIDs::Defeat);
							uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
						}
						uiManager->HUD.activeBanner = ActiveBannerID::VictoryDefeat;
						uiManager->HUD.skipBannerUpdate = false;
						uiManager->HUD.bannerCounter = 0;
						firstTimeInEnd = false;
					}
					else if (tMode == GAMEMODE_MSG::MATCH_WIN_TEAM2)
					{
						if (tTeam == 1)
						{
							uiManager->changeTextureHideAble(hideAbleObj::Banner, 0, BannerTextureIDs::Defeat);
							uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
						}
						else if (tTeam == 2)
						{
							uiManager->changeTextureHideAble(hideAbleObj::Banner, 0, BannerTextureIDs::Victory);
							uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
						}
						uiManager->HUD.activeBanner = ActiveBannerID::VictoryDefeat;
						uiManager->HUD.skipBannerUpdate = false;
						uiManager->HUD.bannerCounter = 0;
						firstTimeInEnd = false;
					}
					else if (tMode == GAMEMODE_MSG::MATCH_DRAW)
					{
						uiManager->changeTextureHideAble(hideAbleObj::Banner, 0, BannerTextureIDs::Victory);
						uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
						uiManager->HUD.activeBanner = ActiveBannerID::VictoryDefeat;
						uiManager->HUD.skipBannerUpdate = false;
						uiManager->HUD.bannerCounter = 0;
						firstTimeInEnd = false;
					}

					firstTimeInEnd = false;
					lowTicketsFirstTime = false;
					uiManager->HUDTime.movePointAdder1 = false;
					uiManager->HUDTime.movePointAdder2 = false;
					uiManager->HUDTime.moveTokenReducer1 = false;
					uiManager->HUDTime.moveTokenReducer2 = false;
					uiManager->HUD.firstSecondEachRound = false;
					//uiManager->hideOrShowHideAble(hideAbleObj::ScoreAdderTeam1, false);
					//uiManager->hideOrShowHideAble(hideAbleObj::ScoreAdderTeam2, false);
					//uiManager->hideOrShowHideAble(hideAbleObj::TicketReducerTeam1, false);
					//uiManager->hideOrShowHideAble(hideAbleObj::TicketReducerTeam2, false);

					uiManager->scaleAndTextChangeTexture(scaleAndText::Wins1, 0, InGameUI::GUI);
					uiManager->scaleAndTextChangeTexture(scaleAndText::Wins2, 0, InGameUI::GUI);
					uiManager->scaleAndTextChangeTexture(scaleAndText::TicketBar1, 0, InGameUI::GUI);
					uiManager->scaleAndTextChangeTexture(scaleAndText::TicketBar2, 0, InGameUI::GUI);

					uiManager->hideOrShowHideAble(hideAbleObj::TicketReducerTeam1, false);
					uiManager->hideOrShowHideAble(hideAbleObj::TicketReducerTeam2, false);
				}

				//Rights out the of round.
				if (firstTimeInEnd && tmp == KOTHSTATE::ENDROUND)
				{
   					if (camPlayer->getTeam() != 0)
					{
						uiManager->changeTextureHideAble(hideAbleObj::Banner, 0, BannerTextureIDs::EndOfRound);
						uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
						uiManager->HUD.activeBanner = ActiveBannerID::RoundEnd;
						uiManager->HUD.skipBannerUpdate = false;
						uiManager->HUD.bannerCounter = 0;
					}

					firstTimeInEnd = false;
					lowTicketsFirstTime = false;
					uiManager->HUDTime.movePointAdder1 = false;
					uiManager->HUDTime.movePointAdder2 = false;
					uiManager->HUDTime.moveTokenReducer1 = false;
					uiManager->scaleAndTextChangeTexture(scaleAndText::Wins2, 0, InGameUI::GUI);

					uiManager->hideOrShowHideAble(hideAbleObj::TicketReducerTeam1, false);
					uiManager->hideOrShowHideAble(hideAbleObj::TicketReducerTeam2, false);
				}

				//Checks to see if the banners "Final Assult" or Hold Your Ground" should be shown.
				if (lowTicketsFirstTime && tmp == KOTHSTATE::OVERTIME)
				{
					int tTeam = camPlayer->getTeam();

					if (uiManager->HUD.teamOneTokens <= 0 && uiManager->HUD.teamTwoTokens <= 0)
					{
						uiManager->changeTextureHideAble(hideAbleObj::Banner, 0, BannerTextureIDs::FinalAssult);
						uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
						uiManager->HUD.activeBanner = ActiveBannerID::OverTime;
						uiManager->HUD.skipBannerUpdate = false;
						uiManager->HUD.bannerCounter = 0;
						lowTicketsFirstTime = false;
					}
					else if (uiManager->HUD.teamOneTokens <= 0)
					{
						if (tTeam == 1)
						{
							uiManager->changeTextureHideAble(hideAbleObj::Banner, 0, BannerTextureIDs::FinalAssult);
							uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
						}
						else
						{
							uiManager->changeTextureHideAble(hideAbleObj::Banner, 0, BannerTextureIDs::HoldYourGround);
							uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
						}
						uiManager->HUD.activeBanner = ActiveBannerID::OverTime;
						uiManager->HUD.skipBannerUpdate = false;
						uiManager->HUD.bannerCounter = 0;
						lowTicketsFirstTime = false;
					}
					else if (uiManager->HUD.teamTwoTokens <= 0)
					{
						if (tTeam == 2)
						{
							uiManager->changeTextureHideAble(hideAbleObj::Banner, 0, BannerTextureIDs::FinalAssult);
							uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
						}
						else
						{
							uiManager->changeTextureHideAble(hideAbleObj::Banner, 0, BannerTextureIDs::HoldYourGround);
							uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
						}
						uiManager->HUD.activeBanner = ActiveBannerID::OverTime;
						uiManager->HUD.skipBannerUpdate = false;
						uiManager->HUD.bannerCounter = 0;
						lowTicketsFirstTime = false;
					}

					uiManager->scaleAndTextChangeTexture(scaleAndText::Wins1, 1);
					uiManager->scaleAndTextChangeTexture(scaleAndText::Wins2, 1);
					uiManager->scaleAndTextChangeTexture(scaleAndText::TicketBar1, 0);
					uiManager->scaleAndTextChangeTexture(scaleAndText::TicketBar2, 0);
				}

				if (firstTimeInWarmUp && tmp == KOTHSTATE::WARMUP)
				{
					if (uiManager->getCurrentMenu() == 0)
					{
						uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
						uiManager->hideOrShowHideAble(hideAbleObj::ScoreAdderTeam1, false);
						uiManager->hideOrShowHideAble(hideAbleObj::ScoreAdderTeam2, false);
						uiManager->hideOrShowHideAble(hideAbleObj::TicketReducerTeam1, false);
						uiManager->hideOrShowHideAble(hideAbleObj::TicketReducerTeam2, false);

						uiManager->clearText(scaleAndText::TicketBar1);
						uiManager->clearText(scaleAndText::TicketBar2);
						uiManager->clearText(scaleAndText::Wins1);
						uiManager->clearText(scaleAndText::Wins2);
						//Ticketbar1
						uiManager->HUD.teamOneTokens = koth->getRespawnTokens(1);
						std::string nText = hudTextOutPutManager(true, uiManager->HUD.teamOneTokens);
						uiManager->setText(nText, scaleAndText::TicketBar1); //tickets
						//

						//Ticketbar2
						uiManager->HUD.teamTwoTokens = koth->getRespawnTokens(2);
						nText = hudTextOutPutManager(true, uiManager->HUD.teamTwoTokens);
						uiManager->setText(nText, scaleAndText::TicketBar2); //tickets
						//

						//Scores1
						uiManager->HUD.teamOneScore = koth->getRoundWins(1);
						nText = hudTextOutPutManager(true, uiManager->HUD.teamOneScore);
						uiManager->setText(nText, scaleAndText::Wins1); //tickets
						//

						//Scores2
						uiManager->HUD.teamTwoScore = koth->getRoundWins(2);
						nText = hudTextOutPutManager(true, uiManager->HUD.teamTwoScore);
						uiManager->setText(nText, scaleAndText::Wins2); //tickets
						//

						//uiManager->setText("0", scaleAndText::TicketBar1); //tickets
						//uiManager->setText("0", scaleAndText::TicketBar2); //tickets2
						//uiManager->setText("0", scaleAndText::Wins1); //wins1
						//uiManager->setText("0", scaleAndText::Wins2); //wins2

						firstTimeInWarmUp = false;
						uiManager->resetHidableWorldMatrix(hideAbleObj::Banner, 0);
						uiManager->setHideableWorldMatrix(hideAbleObj::Banner, 0, vec2(0.0, 1.0));
						uiManager->changeTextureHideAble(hideAbleObj::Banner, 0, BannerTextureIDs::WarmUp);
						uiManager->HUD.activeBanner = ActiveBannerID::WarmUpRound;
						uiManager->HUD.skipBannerUpdate = true;
						uiManager->HUD.bannerCounter = 0;
						warmUpBannerActive = true;
					}
				}

				if (tmp == KOTHSTATE::WARMUP)
				{
					if (!localp->getLockedControls())
					{
						if (i->justPressed(GLFW_KEY_PERIOD))
						{
							showTeamSelect();
						}
						if (i->justPressed(GLFW_KEY_COMMA))
						{
							showClassSelect();
						}
					}

					uiManager->hideOrShowHideAble(hideAbleObj::ScoreAdderTeam1, false);
					uiManager->hideOrShowHideAble(hideAbleObj::ScoreAdderTeam2, false);
				}

				if (tmp == KOTHSTATE::OVERTIME)
				{
					if (!localp->isAlive() && koth->getRespawnTokens(localp->getTeam()) == 0)
					{
						if (i->justPressed(localp->controls.fire))
						{
							std::vector<int>* pteam = koth->getTeamVector(localp->getTeam());
							if (pteam != nullptr)
							{
								bool stopthis = false;
								for (int c = 0; c < pteam->size() && !stopthis; c++)
								{
									if (game->spectateID == -1) //We're not observing anything, set our spectateID to the local player
									{
										if (pteam->at(c) == game->GetLocalPlayerId())
										{
											game->spectateID = game->GetLocalPlayerId(); //This won't last because of the next part
										}
									}

									if (pteam->at(c) == game->spectateID) //If we're looking at the team slot we're currently spectating (can be ourselves according to above)
									{
										if (c < pteam->size() - 1)
										{
											if (game->getPlayer(pteam->at(c + 1)) != nullptr)
												game->spectateID = pteam->at(c + 1); //Step to next player
										}
										else
										{
											if (game->getPlayer(pteam->at(0)) != nullptr)
												game->spectateID = pteam->at(0); //If we're about to step out of the array, instead jump to start
										}

										if (game->spectateID == game->GetLocalPlayerId())
											game->spectateID = -1;
										stopthis = true;
									}
								}
							}
						}
					}
				}
			}
		}


		game->update(newDt);

		std::vector<MovableParticle*>* movable = game->getAllMovableParticle();
		for (size_t i = 0; i < movable->size(); i++)
		{
			if (!(*movable)[i]->created)
			{
				(*movable)[i]->id = renderPipe->createMappedParticleEffect(movable->at(i)->bt, *(*movable)[i]->pPos, (*movable)[i]->dir, (*movable)[i]->color);
				(*movable)[i]->created = true;
			}
			else
			{
				if (!(*movable)[i]->dead)
					renderPipe->moveMappedParticleEffect((*movable)[i]->id, *(*movable)[i]->pPos);
			}
			if ((*movable)[i]->dead)
			{
				renderPipe->removeMappedParticleEffect((*movable)[i]->id);
				(*movable)[i]->allowRemove = true;
			}
		}

		std::vector<HitPosAndDirParticle> hitpositions = game->getAllBulletHitPlayerPos();
		for (size_t i = 0; i < hitpositions.size(); i++)
		{
			renderPipe->createTimedParticleEffect(hitpositions[i].btype, hitpositions[i].pos, hitpositions[i].dir, hitpositions[i].color);
		}

		game->clearAllBulletHitPlayerPos();

		std::vector<EffectParticle> effectpositions = game->getAllEffectParticleSpawn();
		for (size_t i = 0; i < effectpositions.size(); i++)
		{
			renderPipe->createTimedParticleEffect(effectpositions[i].etype, effectpositions[i].pos, effectpositions[i].color);
		}

		game->clearAllEffectParticleSpawn();

		if (kothState != tmp)
		{
			if (tmp == KOTHSTATE::PREROUND)
			{
				round++;

				if (localp->getRole()->getRole() == ROLES::NROFROLES)
				{
					top->command_role_change(top->getConId(), 1);
				}

				uiManager->resetHidableWorldMatrix(hideAbleObj::Banner, 0);
				uiManager->hideOrShowHideAble(hideAbleObj::Banner, false);
				warmUpBannerActive = false;

				//dont show class select when in spectate
				if (localp->getTeam() != 0)
					showClassSelect();

				if (round > -1)
				{
					uiManager->changeTex(5, round - 1, InGameUI::ClassSelect);
					uiManager->stopRendering(5, true, InGameUI::ClassSelect);
				}
				else
					uiManager->stopRendering(5, false, InGameUI::ClassSelect);

				uiManager->hideOrShowHideAble(hideAbleObj::ScoreAdderTeam1, false);
				uiManager->hideOrShowHideAble(hideAbleObj::ScoreAdderTeam2, false);
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

				//Ammo
				uiManager->HUD.ammo = localp->getAmmo();
				int maxAmmo = localp->getMaxAmmo();

				std::string nText = hudTextOutPutManager(false, uiManager->HUD.ammo, maxAmmo);
				uiManager->setText(nText, scaleAndText::Ammo);
				//

				//Ticketbar1
				uiManager->HUD.teamOneTokens = koth->getRespawnTokens(1);
				nText = hudTextOutPutManager(true, uiManager->HUD.teamOneTokens);
				uiManager->setText(nText, scaleAndText::TicketBar1); //tickets
				//

				//Ticketbar2
				uiManager->HUD.teamTwoTokens = koth->getRespawnTokens(2);
				nText = hudTextOutPutManager(true, uiManager->HUD.teamTwoTokens);
				uiManager->setText(nText, scaleAndText::TicketBar2); //tickets
				//

				//Scores1
				uiManager->HUD.teamOneScore = koth->getRoundWins(1);
				nText = hudTextOutPutManager(true, uiManager->HUD.teamOneScore);
				uiManager->setText(nText, scaleAndText::Wins1); //tickets
				//

				//Scores2
				uiManager->HUD.teamTwoScore = koth->getRoundWins(2);
				nText = hudTextOutPutManager(true, uiManager->HUD.teamTwoScore);
				uiManager->setText(nText, scaleAndText::Wins2); //tickets
				//
				
				if (int(koth->getTimer()) == 0)
				{
					uiManager->clearText(scaleAndText::Time);
					uiManager->setText("00:00", scaleAndText::Time); //time
				}
				uiManager->HUD.teamOneTokens = koth->getRespawnTokens(1);
				uiManager->HUD.teamTwoTokens = koth->getRespawnTokens(2);


				uiManager->scaleBar(scaleAndText::HP, 1.0f, false);
				uiManager->scaleBar(scaleAndText::AbilityMeter, 0.0f, false);
				uiManager->scaleBar(scaleAndText::LoseTicketsMeter, 1.0f, false);
				uiManager->HUD.ticketLostTimer = 14;

				uiManager->setRoleBool(true);
				uiManager->setHoverCheckBool(false);

				if(koth->getCapturePoint() == 0)
					uiManager->changeTextureHideAble(hideAbleObj::Banner, 0, BannerTextureIDs::ParkinAreaActive);
				else
					uiManager->changeTextureHideAble(hideAbleObj::Banner, 0, BannerTextureIDs::MarketActive);
				uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
				uiManager->HUD.bannerCounter = 0;
				uiManager->HUD.activeBanner = ActiveBannerID::ActiveCapturePoint;
				uiManager->HUD.skipBannerUpdate = false;

				firstTimeInWarmUp = true;
				firstTimeInEnd = true;
				lowTicketsFirstTime = true;
				uiManager->HUDTime.movePointAdder1 = false;
				uiManager->HUDTime.movePointAdder2 = false;
				uiManager->HUDTime.moveTokenReducer1 = false;
				uiManager->HUDTime.moveTokenReducer2 = false;
				uiManager->HUD.firstSecondEachRound = true;
				uiManager->hideOrShowHideAble(hideAbleObj::Banner, true);
				uiManager->hideOrShowHideAble(hideAbleObj::ScoreAdderTeam1, true);
				uiManager->hideOrShowHideAble(hideAbleObj::ScoreAdderTeam2, true);
				uiManager->hideOrShowHideAble(hideAbleObj::TicketReducerTeam1, true);
				uiManager->hideOrShowHideAble(hideAbleObj::TicketReducerTeam2, true);

				uiManager->scaleAndTextChangeTexture(scaleAndText::TicketBar1, 1);
				uiManager->scaleAndTextChangeTexture(scaleAndText::TicketBar2, 1);
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

			top->frame_playerdata(top->getConId(), pos, local->getDir(), local->getVelocity(), local->getGrounded(), local->getSpecialMeter(), local->roomID);

			if (game->weaponSwitchReady())
			{
				int swaploc = -1;
				WEAPON_TYPE ws = game->getWpnSwitch(swaploc);
				top->frame_weapon_switch(top->getConId(), ws, swaploc);
				
				uiManager->HUD.ammo = local->getAmmo();

				std::string nText = hudTextOutPutManager(false, uiManager->HUD.ammo, local->getMaxAmmo());//= std::to_string(local->getAmmo()) + "/" + std::to_string(local->getMaxAmmo());
				uiManager->clearText(scaleAndText::Ammo);
				uiManager->setText(nText, scaleAndText::Ammo); //ammo

				switch (ws)
				{
				case 0: //Pulse Rifle
					uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::PulseRifle);
					break;
				case 2: //Disc Gun
					uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::Discgun);
					break;
				case 3: //Melee
					uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::Tail);
					break;
				case 4: //Battery slow, manipulator
				case 5: //Battery speed
					uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::BatteryShots);
					break;
				case 6: //Fusion Cannon
					uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::FusionCannon);
					break;
				case 9: //Grenade Launcher
					uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::GranadeLauncher);
					break;
				case 10: //Shotgun
					uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::Shotgun);
					break;
				}
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

		game->fixLocalPlayerCamera(dt);

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
		console.printMsg("        ", "", ' ');
		console.printMsg("Local IP : " + top->getLocalIP(), "", ' ');
		console.printMsg("Public IP : " + top->getPublicIP(), "", ' ');
		console.printMsg("        ", "", ' ');
		console.printMsg("Console commands", "", ' ');
		console.printMsg("/players : List all connected players", "", ' ');
		console.printMsg("/setready : How many players need to /ready before the game starts", "", ' ');
		console.printMsg("/settokens : How many respawntokens the teams start with", "", ' ');
		console.printMsg("/restart : Take a match into Warmup-mode", "", ' ');
		console.printMsg("/help : show this text", "", ' ');
		console.printMsg("        ", "", ' ');
		console.printMsg("To type commands, press the server window (not console)", "", ' ');
		console.printMsg("Press T/Y to go into chat mode and then Enter to send the command", "", ' ');
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
		else if (token == "/fullscreen")
		{
			if (fullscreen)
				fullscreen = false;
			else
				fullscreen = true;
			recreate = true;
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

		else if (token == "/footsteps")
		{
			float footsteps = 0.0f;
			if (!(ss >> footsteps))
			{
				console.printMsg("Invalid input.", "System", 'S');
			}
			else
			{
				if (footsteps < -FLT_EPSILON)
				{
					console.printMsg("Positive numbers only, dummy.", "System", 'S');
				}
				else
					GetSound()->SetFootstepsVolume(footsteps);
			}
		}

		else if (token == "/announcer")
		{
			float announcer = 0.0f;
			if (!(ss >> announcer))
			{
				console.printMsg("Invalid input.", "System", 'S');
			}
			else
			{
				if (announcer < -FLT_EPSILON)
				{
					console.printMsg("Positive numbers only, dummy.", "System", 'S');
				}
				else
					GetSound()->SetAnnouncerVolume(announcer);
			}
		}

		else if (token == "/guns")
		{
			float guns = 0.0f;
			if (!(ss >> guns))
			{
				console.printMsg("Invalid input.", "System", 'S');
			}
			else
			{
				if (guns < -FLT_EPSILON)
				{
					console.printMsg("Positive numbers only, dummy.", "System", 'S');
				}
				else
					GetSound()->SetGunsVolume(guns);
			}
		}

		else if (token == "/ambient")
		{
			float ambient = 0.0f;
			if (!(ss >> ambient))
			{
				console.printMsg("Invalid input.", "System", 'S');
			}
			else
			{
				if (ambient < -FLT_EPSILON)
				{
					console.printMsg("Positive numbers only, dummy.", "System", 'S');
				}
				else
				{
					GetSound()->SetAmbientVolume(ambient);
				}
					
			}
		}

		else if (token == "/effects")
		{
			float effects = 0.0f;
			if (!(ss >> effects))
			{
				console.printMsg("Invalid input.", "System", 'S');
			}
			else
			{
				if (effects < -FLT_EPSILON)
				{
					console.printMsg("Positive numbers only, dummy.", "System", 'S');
				}
				else
					GetSound()->SetEffectVolume(effects);
			}
		}

		else if (token == "/master")
		{
			float master = 0.0f;
			if (!(ss >> master))
			{
				console.printMsg("Invalid input.", "System", 'S');
			}
			else
			{
				if (master < -FLT_EPSILON)
				{
					console.printMsg("Positive numbers only, dummy.", "System", 'S');
				}
				else
				{
					if (master !=0)
					{
						master = master / 100;
					}
					
					GetSound()->SetMasterVolume(master);
				}
					
			}
		}

		else if (token == "/music")
		{
			float music = 0.0f;
			if (!(ss >> music))
			{
				console.printMsg("Invalid input.", "System", 'S');
			}
			else
			{
				if (music < -FLT_EPSILON)
				{
					console.printMsg("Positive numbers only, dummy.", "System", 'S');
				}
				else
				{
					if (!GetSound()->getFading())
					{
						GetSound()->setVolumeMusic(music);
						game->musicVolumeForMenu = music;
					}
					
					
				}

			}
		}

		else if (token == "/cleanup")
		{
			GetSound()->playUserGeneratedSound(SOUNDS::announcerCleanup, CATEGORY::Announcer);
		}

		else if (token == "/background")
		{
			GetSound()->PlayBackground();
		}

		else if (token == "/fuckbackground")
		{
			GetSound()->NoBackground();
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

				uiManager->clearText(scaleAndText::HP, InGameUI::GUI);
				uiManager->clearText(scaleAndText::Ammo, InGameUI::GUI);
				uiManager->clearText(scaleAndText::TicketBar1, InGameUI::GUI);
				uiManager->clearText(scaleAndText::TicketBar2, InGameUI::GUI);
				uiManager->clearText(scaleAndText::Wins1, InGameUI::GUI);
				uiManager->clearText(scaleAndText::Wins2, InGameUI::GUI);
				uiManager->clearText(scaleAndText::Time, InGameUI::GUI);

				uiManager->setText(std::to_string(local->getHP()), scaleAndText::HP, InGameUI::GUI); //hp

				std::string nText = std::to_string(local->getAmmo()) + "/" + std::to_string(local->getMaxAmmo());
				uiManager->setText(nText, scaleAndText::Ammo, InGameUI::GUI); //ammo
				uiManager->setText("00", scaleAndText::TicketBar1, InGameUI::GUI); //tickets
				uiManager->setText("00", scaleAndText::TicketBar2, InGameUI::GUI); //tickets2
				uiManager->setText("00", scaleAndText::Wins1, InGameUI::GUI); //wins1
				uiManager->setText("00", scaleAndText::Wins2, InGameUI::GUI); //wins2
				uiManager->setText("00:00", scaleAndText::Time, InGameUI::GUI); //time


				uiManager->scaleBar(scaleAndText::AbilityMeter, 1.0f, true, InGameUI::GUI);

				uiManager->setHoverCheckBool(false);

				Player* p = game->getPlayer(0);
				Weapon* w = p->getPlayerCurrentWeapon();
				WEAPON_TYPE wt = w->getType();
				switch (wt)
				{
				case 0: //Pulse Rifle
					uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::PulseRifle);
					break;
				case 2: //Disc Gun
					uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::Discgun);
					break;
				case 3: //Melee
					uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::Tail);
					break;
				case 4: //Battery slow, manipulator
				case 5: //Battery speed
					uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::BatteryShots);
					break;
				case 6: //Fusion Cannon
					uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::FusionCannon);
					break;
				case 9: //Grenade Launcher
					uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::GranadeLauncher);
					break;
				case 10: //Shotgun
					uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::Shotgun);
					break;
				}
				switch (role)
				{
				case 1:
					uiManager->scaleAndTextChangeTexture(scaleAndText::consumable, consumables::Cluster);
					break;
				case 3:
					uiManager->scaleAndTextChangeTexture(scaleAndText::consumable, consumables::Dart);
					break;
				case 4:
					uiManager->scaleAndTextChangeTexture(scaleAndText::consumable, consumables::Thermite);
					break;
				case 5:
					uiManager->scaleAndTextChangeTexture(scaleAndText::consumable, consumables::Vortex);
					break;
				}
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
				console.printMsg("/rs  fxaa		FXAA_TOGGLE ", "", ' ');
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
			else if (token == "fxaa")
				renderPipe->setRenderFlag(RENDER_FXAA);
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
		if (token == "/ready")
		{
			if (game->getPlayer(game->GetLocalPlayerId())->getRole()->getRole() != ROLES::NROFROLES)
				top->command_gamemode_ready(top->getConId());
			else
				console.printMsg("Choose a class first, dumbfuck", "System", 'S');
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
			GetSound()->playUserGeneratedSound(SOUNDS::announcerCleanup, CATEGORY::Announcer);
		}
		else if (token == "/role")
		{
			ss >> token;
			if (token == "1" || token == "3" || token == "4" || token == "5")
			{
				if (GetSound())
				{
					if (token == "1")
					{
						GetSound()->playExternalSound(SOUNDS::TrapperPhrase, 0, 0, 0, CATEGORY::Announcer);
					}

					else if (token == "3")
						GetSound()->playExternalSound(SOUNDS::StalkerPhrase, 0, 0, 0, CATEGORY::Announcer);

					else if (token == "4")
						GetSound()->playExternalSound(SOUNDS::PunisherPhrase, 0, 0, 0, CATEGORY::Announcer);

					else if (token == "5")
						GetSound()->playExternalSound(SOUNDS::ManipulatorPhrase, 0, 0, 0, CATEGORY::Announcer);

				}
			
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
			int id;
			if (!(ss >> id))
				console.printMsg("Invalid Number", "System", 'S');
			else
			{
				Player* local = game->getPlayer(game->GetLocalPlayerId());
				//if (local->getTeam() == 0)
				//{
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
							uiManager->setMenu(InGameUI::GUI);
							uiManager->setTeamColor(p->getTeam());
							uiManager->changeColorTeam();
						}
						else
							console.printMsg("That Player Doesn't exist", "System", 'S');
					}
					else
						game->spectateID = -1;
				//}
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
				console.printMsg("/rs  fxaa		FXAA_TOGGLE ", "", ' ');
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
			else if (token == "fxaa")
				renderPipe->setRenderFlag(RENDER_FXAA);
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
			console.printMsg("        ", "", ' ');
			console.printMsg("Local IP : " + top->getLocalIP(), "", ' ');
			console.printMsg("Public IP : " + top->getPublicIP(), "", ' ');
			console.printMsg("        ", "", ' ');
			console.printMsg("Console commands", "", ' ');
			console.printMsg("/players : List all connected players", "", ' ');
			console.printMsg("/setready : How many players need to /ready before the game starts", "", ' ');
			console.printMsg("/settokens : How many respawntokens the teams start with", "", ' ');
			console.printMsg("/restart : Take a match into Warmup-mode", "", ' ');
			console.printMsg("/help : show this text", "", ' ');
			console.printMsg("        ", "", ' ');
			console.printMsg("To type commands, press the server window (not console)", "", ' ');
			console.printMsg("Press T/Y to go into chat mode and then Enter to send the command", "", ' ');

			//console.printMsg("/disconnect", "", ' ');
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
		
		//Everything below is KOTH specific :(
		else if (token == "/setready")
		{
			int readyNeeded = 0;
			if (!(ss >> readyNeeded))
			{
				console.printMsg("Invalid input", "System", 'S');
			}
			else
			{
				KingOfTheHill* koth = (KingOfTheHill*)game->getGameMode();
				koth->setReadyNeeded(readyNeeded);
				console.printMsg("Set readies needed", "System", 'S');
			}
		}
		else if (token == "/settokens")
		{
			int tokens = 0;
			if (!(ss >> tokens))
			{
				console.printMsg("Invalid input", "System", 'S');
			}
			else
			{
				KingOfTheHill* koth = (KingOfTheHill*)game->getGameMode();
				koth->setTeamTokens(tokens);
				console.printMsg("Tokens set", "System", 'S');
			}
		}
		else if (token == "/setwinscore")
		{
			int win = 0;
			if (!(ss >> win))
			{
				console.printMsg("Invalid input", "System", 'S');
			}
			else
			{
				KingOfTheHill* koth = (KingOfTheHill*)game->getGameMode();
				koth->setWinScore(win);
				console.printMsg("Win score set", "System", 'S');
			}
		}
		else if (token == "/setpoints")
		{
			int points = 0;
			if (!(ss >> points))
			{
				console.printMsg("Invalid input", "System", 'S');
			}
			else
			{
				KingOfTheHill* koth = (KingOfTheHill*)game->getGameMode();
				koth->setScorePerRound(points);
				console.printMsg("Points per round set", "System", 'S');
			}
		}
		else if (token == "/restart")
		{
			KingOfTheHill* koth = (KingOfTheHill*)game->getGameMode();
			koth->restartGame();
		}
	}
}

void Core::saveSettings()
{
	if (current == Gamestate::SERVER)
		return;

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
			file << "Sound: 1" << endl;
		else
			file << "Sound: 0" << endl;

		if (fullscreen)
			file << "Fullscreen: 1" << endl;
		else
			file << "Fullscreen: 0" << endl;

		float sens = serverCam->getSensitivity();
		file << "Sensitivity: " << sens << endl;

		file << "WinX: " << winX << endl;
		file << "WinY: " << winY << endl;

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
			else if (in == "Fullscreen:")
			{
				bool isFull = atoi(in2.c_str());
				if (isFull != fullscreen)
				{
					recreate = true;
					fullscreen = isFull;
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
			else if (in == "WinX:")
			{
				int x = atoi(in2.c_str());
				if (x != winX)
				{
					recreate = true;
					winX = x;
				}
			}
			else if (in == "WinY:")
			{
				int y = atoi(in2.c_str());
				if (y != winY)
				{
					recreate = true;
					winY = y;
				}
			}
		}
		file.close();
	}
	else
	{
		saveSettings(); //save file with default values
	}

}

void Core::loadControls()
{
	fstream file("GameFiles/Config/controls.ini");
	Player* p = game->getPlayer(game->GetLocalPlayerId());
	if (file.is_open())
	{
		string in;
		string in2;
		while (getline(file, in))
		{
			stringstream ss(in);
			ss >> in;
			ss >> in2;
			if (in == "Fire:")
				p->controls.fire = i->getGLFWkeyFromString(in2);
			else if (in == "Forward:")
				p->controls.forward = i->getGLFWkeyFromString(in2);
			else if (in == "Back:")
				p->controls.back = i->getGLFWkeyFromString(in2);
			else if (in == "Left:")
				p->controls.left = i->getGLFWkeyFromString(in2);
			else if (in == "Right:")
				p->controls.right = i->getGLFWkeyFromString(in2);
			else if (in == "Jump:")
				p->controls.jump = i->getGLFWkeyFromString(in2);
			else if (in == "Reload:")
				p->controls.reload = i->getGLFWkeyFromString(in2);
			else if (in == "WeaponOne:")
				p->controls.weaponone = i->getGLFWkeyFromString(in2);
			else if (in == "WeaponTwo:")
				p->controls.weapontwo = i->getGLFWkeyFromString(in2);
			else if (in == "Consumable:")
				p->controls.item = i->getGLFWkeyFromString(in2);
			else if (in == "Mobility:")
				p->controls.mobility = i->getGLFWkeyFromString(in2);
			else if (in == "Super:")
				p->controls.special = i->getGLFWkeyFromString(in2);
			else if (in == "Minimap:")
				p->controls.minimap = i->getGLFWkeyFromString(in2);
			else if (in == "Scorescreen:")
				p->controls.scorescreen = i->getGLFWkeyFromString(in2);
			else if (in == "Dance:")
				p->controls.dance = i->getGLFWkeyFromString(in2);
			else if (in == "Allchat:")
				console.setChatKey(i->getGLFWkeyFromString(in2), true);
			else if (in == "Teamchat:")
				console.setChatKey(i->getGLFWkeyFromString(in2), false);
		}
		file.close();
	}
	else
	{
		saveControls(); //save file with default values
	}
}

void Core::saveControls()
{
	/*Player* p = game->getPlayer(game->GetLocalPlayerId());
	fstream file;
	file.open("GameFiles/Config/controls.ini", fstream::trunc | fstream::out);

	if (file.is_open())
	{
		file << "Fire: " << p->controls.fire << endl;
		file << "Forward: " << p->controls.forward << endl;
		file << "Back: " << p->controls.back << endl;
		file << "Left: " << p->controls.left << endl;
		file << "Right: " << p->controls.right << endl;
		file << "Jump: " << p->controls.jump << endl;
		file << "Reload: " << p->controls.reload << endl;
		file << "WeaponOne: " << p->controls.weaponone << endl;
		file << "WeaponTwo: " << p->controls.weapontwo << endl;
		file << "Consumable: " << p->controls.item << endl;
		file << "Mobility: " << p->controls.mobility << endl;
		file << "Super: " << p->controls.special;
		file.close();
	}*/
}

void Core::renderWorld(float dt)
{
	if (renderPipe && game)
	{
		//temp set to see anims in 3p 
		CameraInput* cam = CameraInput::getCam();
		vec3 camPos = cam->getPos();
		vec3 oldPos = camPos;
		vec3 camDir = cam->getDir();
		bool force3rd = false;

		//float intennn = (sin(timepass / 5));
		//if (intennn < 0)
		//	intennn = 0;
		//renderPipe->setuniversalInten(intennn);

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

		Player* tmp_player = 0;
		int pid = game->GetLocalPlayerId();

		if (game->spectateID == -1)
		{
			tmp_player = game->getPlayer(pid);
			if(game->freecam == false)
				tmp_player->deadViewAngles();
		}
		else
		{
			tmp_player = game->getPlayer(game->spectateID);
			tmp_player->deadViewAngles();
		}

		if (game->spectateID == -1)
		{
			if (tmp_player->getLockedControls() == false)
			{
				if (i->getKeyInfo(tmp_player->controls.dance))
				{
					tmp_player->DANCEMAGICDANCE(true); //You remind me of the babe
					force3rd = true; //What babe?
					camPos = camPos - (camDir * 5.0f); //The babe with the power
					cam->setCam(camPos); //What power?

					if (game->freecam == false) //The power of voodoo!
					{
						tmp_player->resetRotation(); //Who do?
						tmp_player->rotatePlayer(vec3(0, 0, 1), camDir); //You do!
					}
				}
				else //Do what?
					tmp_player->DANCEMAGICDANCE(false); //REMIND ME OF THE BABE
			}		
		}

		glm::vec3 tmpEyePos = camPos;

		renderPipe->update(tmpEyePos.x, tmpEyePos.y, tmpEyePos.z, dt); // sets the view/proj matrix
		renderPipe->renderIni();

		SpotLight light;

		vec3 dgColor(0);
		//render skybox
		renderPipe->renderMISC(-3, (void*)&(CameraInput::getCam()->getSkyboxMat()), &dgColor.x, 0.0f);

		//send chunk glowvalues
		//dgColor = { 0.1, 0.1, 0.1 };
		//renderPipe->setChunkColorAndInten(0, &dgColor[0], 0);

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
							if (lastHP_blurreffect > p->getHP() && p->getHP() != p->getMaxHP())
								renderPipe->startTakeDamageEffect(6, 0.6f);
							lastHP_blurreffect = p->getHP();
						}
					}
					else if (game->spectateID == i)
					{
						if (lastHP_blurreffect > p->getHP() && p->getHP() != p->getMaxHP())
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
					light.AmbientIntensity = 0.5f;
					light.attenuation.w = 5.0f;
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
						if (p->isLocal() && game->spectateID != -1)
						{
							//dont render local fps view
						}
						else
						{
							if (p->isLocal())   //use current anim
								renderPipe->renderAnimation(i, p->getRole()->getRole(), &p->getFPSmat(), p->getAnimState_f_c(), &dgColor.x, hpval, true, p->getAnimPrimary(), cam->roomID);
							else //use peak anim
								renderPipe->renderAnimation(i, p->getRole()->getRole(), &p->getFPSmat(), p->getAnimState_f_p(), &dgColor.x, hpval, true, p->getAnimPrimary(), cam->roomID);
						}
					}
					else
					{
						if (p->getRole()->getRole() != ROLES::NROFROLES)
						{
							glm::mat4* playermat = p->getWorldMat();
							if (force3rd)
								playermat[0][1].w = p->getPos().y -1.45f;

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

		int cullArr[2];
		int cullhit;

		if (hackedTeam == -1)
		{
			for (int c = 0; c < BULLET_TYPE::NROFBULLETS; c++)
			{
				std::vector<Bullet*> bullets = game->getBullets(BULLET_TYPE(c));

				if (c == BULLET_TYPE::SHOTGUN_PELLET)
				{
					light.AmbientIntensity = 3.0f;
					light.attenuation.w = 3.0f;
				}
				else
				{
					light.AmbientIntensity = 0.6f;
					light.attenuation.w = 4.0f;
				}

				for (unsigned int i = 0; i < bullets.size(); i++)
				{
					game->getPhysics()->cullingPointvsRoom(&bullets[i]->getPos(), cullArr, cullhit, 1);
					if (cullhit != 1)
						cullArr[0] = 0;

					if (bullets[i]->getTeam() == 1)
					{
						renderPipe->renderBullet(c, bullets[i]->getWorldMat(), &TEAMONECOLOR.x, 0.0f);
						light.Color = TEAMONECOLOR; // +vec3(0.2f, 0.2f, 0.2f);
						light.Position = bullets[i]->getPos();
						renderPipe->addLight(&light, cullArr[0]);
					}

					else //(bullets[i]->getTeam() == 2)
					{
						renderPipe->renderBullet(c, bullets[i]->getWorldMat(), &TEAMTWOCOLOR.x, 0.0f);
						light.Color = TEAMTWOCOLOR; // +vec3(0.2f, 0.2f, 0.2f);
						light.Position = bullets[i]->getPos();
						renderPipe->addLight(&light, cullArr[0]);
					}
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
					renderPipe->renderBullet(c, bullets[i]->getWorldMat(), &TEAMTWOCOLOR.x, 0.0f);
					light.Color = TEAMTWOCOLOR;
					light.Position = bullets[i]->getPos();
					renderPipe->addLight(&light, 0);
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
					renderPipe->renderBullet(c, bullets[i]->getWorldMat(), &TEAMONECOLOR.x, 0.0f);
					light.Color = TEAMONECOLOR;
					light.Position = bullets[i]->getPos();
					renderPipe->addLight(&light, 0);
				}
			}
		} // render bullets end

		  // capturePoint
		if (current == CLIENT)
		{
			KingOfTheHill* koth = (KingOfTheHill*)game->getGameMode();
			renderPipe->renderCapturePoint(koth->getCapturePoint());
		}

		flamebarrelflicker();

		// render chunks
		renderPipe->render();

		// render trailquads
		trailQuadsRender(hackedTeam);

		// render effects
		int effectTime = renderPipe->startExecTimer("Effects & decals");
		effectsRender(hackedTeam);

		renderPipe->enableBlend(false);

		// render Decals
		renderPipe->renderDecals(game->getAllDecalRenderInfo(), game->getNrOfDecals());

		renderPipe->stopExecTimer(effectTime);

		renderPipe->finalizeRender();

		renderPipe->disableDepthTest();
		renderPipe->enableBlend(false);

		renderPipe->renderCrosshair(CROSSHAIR_TRAPPER_P);

		if (tmp_player->hitMarker > 0.0f)
		{
			renderPipe->renderCrosshair(CROSSHAIR_SHANKER_P);
			tmp_player->hitMarker -= dt;
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

		renderPipe->renderTakeDamageDistort();

		if (game != nullptr)
		{
			if (current != SERVER)
			{
				Player* localp = game->getPlayer(game->GetLocalPlayerId());
				//render minimap
				if (i->getKeyInfo(localp->controls.minimap))
					if (game->getPlayer(game->GetLocalPlayerId())->getLockedControls() == false)
						minimapRender();

				// render score screen
				if (i->getKeyInfo(localp->controls.scorescreen))
					//if (game->getPlayer(game->GetLocalPlayerId())->getLockedControls() == false)
					scoreboardRender();

				if (i->getKeyInfo(GLFW_KEY_F1))
					renderPipe->renderHelp(1);
				if (i->getKeyInfo(GLFW_KEY_F2))
					renderPipe->renderHelp(2);
				if (i->getKeyInfo(GLFW_KEY_F3))
					renderPipe->renderHelp(3);
			}

			//press F1 for help
			renderPipe->setTextObjectText(leaderBoardTextID, "Help: F1");
			renderPipe->setTextPos(leaderBoardTextID, vec2(1150, 720));
			renderPipe->renderTextObject(leaderBoardTextID);


			//spectate this playername render
			if (game->spectateID != -1)
			{
				renderPipe->setTextObjectText(leaderBoardTextID, "Spectating: " + tmp_player->getName());
				renderPipe->setTextPos(leaderBoardTextID, vec2(500 - 42, 650));
				renderPipe->renderTextObject(leaderBoardTextID);
			}
			else
				if (game->GetGameState() == Gamestate::CLIENT)
					if (tmp_player->isAlive() == false)
					{
						KingOfTheHill* koth = (KingOfTheHill*)game->getGameMode();
						if (koth->getState() == KOTHSTATE::OVERTIME)
						{
							renderPipe->setTextObjectText(leaderBoardTextID, "Press 'FIRE' to spectate");
							renderPipe->setTextPos(leaderBoardTextID, vec2(500 - 42, 650));
							renderPipe->renderTextObject(leaderBoardTextID);
						}
					}


			renderPipe->disableBlend();

			Player* localplayer = game->getPlayer(game->GetLocalPlayerId());
			//viewing 3rd person anims in roam
			if (i->getKeyInfo(localplayer->controls.dance))
				cam->setCam(oldPos);
			//if (i->getKeyInfo(GLFW_KEY_L))
			//	cam->setCam(camPos, camDir);

			if (localplayer->getLockedControls() == false)
			{
				if (game->freecam)
				{
					if (i->justPressed(GLFW_KEY_1))
						game->spectateID = -1;
					if (i->justPressed(GLFW_KEY_2))
					{
						//search for closest player to cam and set that ConID to spectateID
						int closest = -1;
						float lenth = 999999.0f;
						//camPos
						Player* p;
						int localID = game->GetLocalPlayerId();
						for (int n = 0; n < 20; n++)
							if (n != localID)
							{
								p = game->getPlayer(n);
								if (p)
								{
									float len2 = length(p->getPos() - camPos);
									if (len2 < lenth)
									{
										lenth = len2;
										closest = n;
									}
								}
							}
						clientHandleCmds(string("/spec " + to_string(closest)));
					}
				}

			}
		}
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
		bool used = local->getRole()->getConsumable()->checkIfUsed();

		if (used)
			uiManager->stopRendering(7, false, InGameUI::GUI);
		else
			uiManager->stopRendering(7, true, InGameUI::GUI);

		if (local->getHP() != uiManager->HUD.HP) //Health
		{
			uiManager->HUD.HP = local->getHP();
			uiManager->clearText(scaleAndText::HP, InGameUI::GUI);
			uiManager->setText(std::to_string(uiManager->HUD.HP), scaleAndText::HP, InGameUI::GUI);
			if ((float)local->getMaxHP() > 0)
			{
				uiManager->scaleBar(scaleAndText::HP, (float)(uiManager->HUD.HP) / (float)(local->getMaxHP()), true, InGameUI::GUI);
			}
			else
				console.printMsg("Error: Function inGameUIUpdate in Core, local player maxHp has a value of 0 or below", "System", 'S');
		}
		if (local->getSpecialMeter() != uiManager->HUD.specialMeter) //Special Meter
		{
			float procent = 0.0f;
			uiManager->HUD.specialMeter = local->getSpecialMeter();
			if(uiManager->HUD.maxSpecialMeter > 0)
				procent = local->getSpecialMeter() / uiManager->HUD.maxSpecialMeter;
			uiManager->scaleBar(scaleAndText::AbilityMeter, procent, true, InGameUI::GUI);
		}
		if (local->getAmmo() != uiManager->HUD.ammo) //Ammo
		{
			uiManager->HUD.ammo = local->getAmmo();
			int maxAmmo = local->getMaxAmmo();

			std::string sAmmo = "0";
			std::string sMaxAmmo = "0";

			sAmmo += std::to_string(uiManager->HUD.ammo);
			sMaxAmmo += std::to_string(maxAmmo);

			if (sAmmo.size() == 3)
				sAmmo = std::to_string(uiManager->HUD.ammo);
			else if (sAmmo.size() > 3)
				sAmmo = "99";
			if (sMaxAmmo.size() == 3)
				sMaxAmmo = std::to_string(maxAmmo);
			else if (sMaxAmmo.size() > 3)
				sMaxAmmo = "99";

			std::string nText = sAmmo + "/" + sMaxAmmo;
			uiManager->clearText(scaleAndText::Ammo, InGameUI::GUI);
			uiManager->setText(nText, scaleAndText::Ammo, InGameUI::GUI);
		}
	} //player stats on HUD end

	 //Checks if the teams token number have changed.
	if (koth->getRespawnTokens(1) != uiManager->HUD.teamOneTokens) //Team 1
	{
		int difference = uiManager->HUD.teamOneTokens - koth->getRespawnTokens(1);

		uiManager->HUD.teamOneTokens = koth->getRespawnTokens(1);
		uiManager->clearText(scaleAndText::TicketBar1, InGameUI::GUI);
		std::string nText = hudTextOutPutManager(true, uiManager->HUD.teamOneTokens);
		uiManager->setText(nText, scaleAndText::TicketBar1, InGameUI::GUI);
		
		int tmp = uiManager->addNewWM(hideAbleObj::TicketReducerTeam1);
		if (tmp != -1)
		{
			if (uiManager->HUDTime.wmIdListTicket1.size() == 0)
				uiManager->HUDTime.moveTokenReducer1 = true;

			uiManager->HUDTime.wmIdListTicket1.push_back(tmp);
			uiManager->HUDTime.counterListTicket1.push_back(0);

			if (difference > 0 && difference < 4)
				uiManager->changeTextureHideAble(hideAbleObj::TicketReducerTeam1, uiManager->HUDTime.wmIdListTicket1.size() - 1, difference - 1);
		}
	}

	if (koth->getRespawnTokens(2) != uiManager->HUD.teamTwoTokens) //Team 2
	{
		int difference = uiManager->HUD.teamTwoTokens - koth->getRespawnTokens(2);

		uiManager->HUD.teamTwoTokens = koth->getRespawnTokens(2);
		uiManager->clearText(scaleAndText::TicketBar2, InGameUI::GUI);
		std::string nText = hudTextOutPutManager(true, uiManager->HUD.teamTwoTokens);
		uiManager->setText(nText, scaleAndText::TicketBar2, InGameUI::GUI);

		int tmp = uiManager->addNewWM(hideAbleObj::TicketReducerTeam2);
		if (tmp != -1)
		{
			if (uiManager->HUDTime.wmIdListTicket2.size() == 0)
				uiManager->HUDTime.moveTokenReducer2 = true;

			uiManager->HUDTime.wmIdListTicket2.push_back(tmp);
			uiManager->HUDTime.counterListTicket2.push_back(0);

			if (difference > 0 && difference < 4)
				uiManager->changeTextureHideAble(hideAbleObj::TicketReducerTeam2, uiManager->HUDTime.wmIdListTicket2.size() - 1, difference - 1);
		}
	}

	//**********************************************************************************//
	//Checks if the teams score have changed.
	if (koth->getRoundWins(1) != uiManager->HUD.teamOneScore) //Team 1
	{
		if (koth->getRoundWins(1) != -1)
		{
			int difference = koth->getRoundWins(1) - uiManager->HUD.teamOneScore;
	
			uiManager->HUD.teamOneScore = koth->getRoundWins(1);
			uiManager->clearText(scaleAndText::Wins1, InGameUI::GUI);
			std::string nText = hudTextOutPutManager(true, uiManager->HUD.teamOneScore);
			uiManager->setText(nText, scaleAndText::Wins1, InGameUI::GUI);

			if (difference > 0 && difference < 4)
			{
				int tmp = uiManager->addNewWM(hideAbleObj::ScoreAdderTeam1);
				if (tmp != -1)
				{
					if (uiManager->HUDTime.wmIdListScore1.size() == 0)
						uiManager->HUDTime.movePointAdder1 = true;
	
					uiManager->HUDTime.wmIdListScore1.push_back(tmp);
					uiManager->HUDTime.counterListScore1.push_back(0);
					
					if (difference > 0 && difference < 4)
						uiManager->changeTextureHideAble(hideAbleObj::ScoreAdderTeam1, uiManager->HUDTime.wmIdListScore1.size() - 1, difference - 1);
				}
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
			uiManager->clearText(scaleAndText::Wins2, InGameUI::GUI);
			std::string nText = hudTextOutPutManager(true, uiManager->HUD.teamTwoScore);
			uiManager->setText(nText, scaleAndText::Wins2, InGameUI::GUI);

			if (difference > 0 && difference < 4)
			{
				int tmp = uiManager->addNewWM(hideAbleObj::ScoreAdderTeam2);
				if (tmp != -1)
				{
					if (uiManager->HUDTime.wmIdListScore2.size() == 0)
						uiManager->HUDTime.movePointAdder2 = true;
	
					uiManager->HUDTime.wmIdListScore2.push_back(tmp);
					uiManager->HUDTime.counterListScore2.push_back(0);
	
					if (difference > 0 && difference < 4)
						uiManager->changeTextureHideAble(hideAbleObj::ScoreAdderTeam2, uiManager->HUDTime.wmIdListScore2.size() - 1, difference - 1);
				}
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
		
		if (seconds == 60)
		{
			minutes += 1;
			seconds = 0;
		}

		std::string sMinutes = "0";
		std::string sSeconds = "0";

		sMinutes += std::to_string(minutes);
		sSeconds += std::to_string(seconds);

		if (sMinutes.size() > 2)
			sMinutes = std::to_string(minutes);
		if (sSeconds.size() > 2)
			sSeconds = std::to_string(seconds);

		uiManager->clearText(scaleAndText::Time, InGameUI::GUI);
		uiManager->setText(sMinutes + ":" + sSeconds, scaleAndText::Time, InGameUI::GUI);

		if (koth->getState() == ROUND) //Tickets meter should only work during the rounds which is why this check is here.
		{
			if (calcTimer)
			{
				int tDiff = int(koth->getTimer()) - oldTime;
				tDiff = tDiff % 15;
				uiManager->HUD.ticketLostTimer -= tDiff;

				if (uiManager->HUD.ticketLostTimer < 0)
				{
					tDiff = uiManager->HUD.ticketLostTimer;
					tDiff *= -1;
					tDiff = 15 - tDiff;
					uiManager->HUD.ticketLostTimer = tDiff;

					uiManager->scaleBar(scaleAndText::LoseTicketsMeter, (float)(uiManager->HUD.ticketLostTimer) / (float)(uiManager->HUD.loseTicketPer), true, InGameUI::GUI);
					uiManager->HUD.ticketLostTimer -= 1;
				}
				else if (uiManager->HUD.ticketLostTimer == 0)
				{
					uiManager->HUD.ticketLostTimer = uiManager->HUD.loseTicketPer;
					uiManager->scaleBar(scaleAndText::LoseTicketsMeter, (float)(uiManager->HUD.ticketLostTimer) / (float)(uiManager->HUD.loseTicketPer), true, InGameUI::GUI);
				}
				else
				{
					uiManager->scaleBar(scaleAndText::LoseTicketsMeter, (float)(uiManager->HUD.ticketLostTimer) / (float)(uiManager->HUD.loseTicketPer), true, InGameUI::GUI);
					uiManager->HUD.ticketLostTimer -= 1;
				}
				calcTimer = false;
			}
			else if (!uiManager->HUD.firstSecondEachRound) //If it isn't the first second of the round
			{
				if (uiManager->HUD.ticketLostTimer == 0)
				{
					uiManager->scaleBar(scaleAndText::LoseTicketsMeter, 0.0f, true, InGameUI::GUI);
					uiManager->HUD.ticketLostTimer = uiManager->HUD.loseTicketPer;
				}
				else
				{
					if (uiManager->HUD.loseTicketPer > 0)
					{
						uiManager->scaleBar(scaleAndText::LoseTicketsMeter, (float)(uiManager->HUD.ticketLostTimer) / (float)(uiManager->HUD.loseTicketPer), true, InGameUI::GUI);
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
					uiManager->scaleBar(scaleAndText::LoseTicketsMeter, (float)(uiManager->HUD.ticketLostTimer) / (float)(uiManager->HUD.loseTicketPer), true, InGameUI::GUI);
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
			uiManager->scaleBar(scaleAndText::LoseTicketsMeter, 1.0f, true, InGameUI::GUI);
		}

		if (!uiManager->HUD.skipBannerUpdate)
		{
			//Counters
			if (uiManager->HUD.bannerCounter == uiManager->HUD.bannerMaxTime[uiManager->HUD.activeBanner])
			{
				uiManager->hideOrShowHideAble(hideAbleObj::Banner, false);
				uiManager->HUD.bannerCounter = 0;
				uiManager->HUD.skipBannerUpdate = true;
				uiManager->resetHidableWorldMatrix(hideAbleObj::Banner, 0); //The 0 is WorldMatrix index since it lies in a vector
			}
			else
				uiManager->HUD.bannerCounter++;
		}
		if (uiManager->HUDTime.counterListScore1.size() > 0)
		{
			for (int i = 0; i < uiManager->HUDTime.counterListScore1.size(); i++)
			{
				if (uiManager->HUDTime.counterListScore1[i] >= 2)
				{
					uiManager->deleteOldestWM(hideAbleObj::ScoreAdderTeam1);

					if (uiManager->HUDTime.wmIdListScore1.size() > 0)
						uiManager->HUDTime.wmIdListScore1.erase(uiManager->HUDTime.wmIdListScore1.begin());

					uiManager->HUDTime.counterListScore1.erase(uiManager->HUDTime.counterListScore1.begin());

					if (uiManager->HUDTime.wmIdListScore1.size() < 1)
						uiManager->HUDTime.movePointAdder1 = false;
				}
			}
			for (size_t i = 0; i < uiManager->HUDTime.counterListScore1.size(); i++)
				uiManager->HUDTime.counterListScore1[i]++;
		}

		if (uiManager->HUDTime.counterListScore2.size() > 0)
		{
			for (int i = 0; i < uiManager->HUDTime.counterListScore2.size(); i++)
			{
				if (uiManager->HUDTime.counterListScore2[i] >= 2)
				{
					uiManager->deleteOldestWM(hideAbleObj::ScoreAdderTeam2);

					if (uiManager->HUDTime.wmIdListScore2.size() > 0)
						uiManager->HUDTime.wmIdListScore2.erase(uiManager->HUDTime.wmIdListScore2.begin());

					uiManager->HUDTime.counterListScore2.erase(uiManager->HUDTime.counterListScore2.begin());

					if (uiManager->HUDTime.wmIdListScore2.size() < 1)
						uiManager->HUDTime.movePointAdder2 = false;
				}
			}
			for (size_t i = 0; i < uiManager->HUDTime.counterListScore2.size(); i++)
				uiManager->HUDTime.counterListScore2[i]++;
		}

		if (uiManager->HUDTime.counterListTicket1.size() > 0)
		{
			for (int i = 0; i < uiManager->HUDTime.counterListTicket1.size(); i++)
			{
				if (uiManager->HUDTime.counterListTicket1[i] >= 2)
				{
					uiManager->deleteOldestWM(hideAbleObj::TicketReducerTeam1);

					if (uiManager->HUDTime.wmIdListTicket1.size() > 0)
						uiManager->HUDTime.wmIdListTicket1.erase(uiManager->HUDTime.wmIdListTicket1.begin());

					uiManager->HUDTime.counterListTicket1.erase(uiManager->HUDTime.counterListTicket1.begin());

					if (uiManager->HUDTime.wmIdListTicket1.size() < 1)
						uiManager->HUDTime.moveTokenReducer1 = false;
				}
			}
			for (size_t i = 0; i < uiManager->HUDTime.counterListTicket1.size(); i++)
				uiManager->HUDTime.counterListTicket1[i]++;
		}

		if (uiManager->HUDTime.counterListTicket2.size() > 0)
		{
			for (int i = 0; i < uiManager->HUDTime.counterListTicket2.size(); i++)
			{
				if (uiManager->HUDTime.counterListTicket2[i] >= 2)
				{
					uiManager->deleteOldestWM(hideAbleObj::TicketReducerTeam2);

					if (uiManager->HUDTime.wmIdListTicket2.size() > 0)
						uiManager->HUDTime.wmIdListTicket2.erase(uiManager->HUDTime.wmIdListTicket2.begin());

					uiManager->HUDTime.counterListTicket2.erase(uiManager->HUDTime.counterListTicket2.begin());

					if (uiManager->HUDTime.wmIdListTicket2.size() < 1)
						uiManager->HUDTime.moveTokenReducer2 = false;	
				}
			}
			for (size_t i = 0; i < uiManager->HUDTime.counterListTicket2.size(); i++)
				uiManager->HUDTime.counterListTicket2[i]++;
		}
	}

	if (uiManager->HUDTime.moveTokenReducer1)
		for (int i = 0; i < uiManager->HUDTime.wmIdListTicket1.size(); i++)
			uiManager->setHideableWorldMatrix(hideAbleObj::TicketReducerTeam1, i, glm::vec2(0.0f, -0.001f));
	if (uiManager->HUDTime.moveTokenReducer2)
		for (int i = 0; i < uiManager->HUDTime.wmIdListTicket2.size(); i++)
			uiManager->setHideableWorldMatrix(hideAbleObj::TicketReducerTeam2, i, glm::vec2(0.0f, -0.001f));
	if (uiManager->HUDTime.movePointAdder1)
		for (int i = 0; i < uiManager->HUDTime.wmIdListScore1.size(); i++)
			uiManager->setHideableWorldMatrix(hideAbleObj::ScoreAdderTeam1, i, glm::vec2(0.0f, -0.001f));
	if (uiManager->HUDTime.movePointAdder2)
		for (int i = 0; i < uiManager->HUDTime.wmIdListScore2.size(); i++)
			uiManager->setHideableWorldMatrix(hideAbleObj::ScoreAdderTeam2, i, glm::vec2(0.0f, -0.001f));

	if (i->justPressed(GLFW_KEY_ESCAPE))
	{
		if (!escActive)
		{
			escActive = true;
			uiManager->setMenu(InGameUI::ESC);

			uiManager->setHoverCheckBool(true);
			game->getPlayer(game->GetLocalPlayerId())->setLockedControls(true);
			cursorInvisible = false;
		}
		else
		{
			escActive = false;
			uiManager->setMenu(InGameUI::RemoveMenu);

			uiManager->setHoverCheckBool(false);
			game->getPlayer(game->GetLocalPlayerId())->setLockedControls(false);
			cursorInvisible = true;
		}
	}

	uiManager->renderHideAble();
	uiManager->inGameRender();

	if (justSetFullScreenIngame)
	{
		uiManager->setMenu(InGameUI::ESC);
		setHUDText(InGameUI::GUI);
		justSetFullScreenIngame = false;
	}

	double x = (0.0);
	double y = (0.0);
	float nothing = 1.0f;
	//Get mouse position
	i->getCursor(x, y);
	double tX = (x / (double)winX) * 2 - 1.0;
	double tY = (-y / (double)winY) * 2 + 1.0;

	if (uiManager->getHoverCheckBool())
	{
		if (i->justPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			int eventIndex = uiManager->collisionCheck(glm::vec2((float)tX, (float)tY), nothing);
			switch (eventIndex)
			{
			case 21: //Team 1
				if (current == ROAM)
				{
					uiManager->setTeamColor(TeamColors::TeamOne);
					roamHandleCmds("/team 2");
				}
				else
					clientHandleCmds("/team 2");
				uiManager->stopRendering(5, false, InGameUI::ClassSelect);
				break;
			case 22: //Team 2
				if (current == ROAM)
				{
					uiManager->setTeamColor(TeamColors::TeamTwo);
					roamHandleCmds("/team 1");
				}
				else
					clientHandleCmds("/team 1");
				uiManager->stopRendering(5, false, InGameUI::ClassSelect);
				break;
			case 23: //Spectate
				if (current == CLIENT)
				{
					game->freecam = true;
					uiManager->setFirstMenuSet(false);
					uiManager->setMenu(InGameUI::GUI);
					if (current == ROAM)
						roamHandleCmds("/team 0");
					else
						clientHandleCmds("/team 0");
				}
				break;
			case 24: //cancle
				uiManager->setFirstMenuSet(false);
				uiManager->setMenu(InGameUI::GUI);
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
				escActive = false;
				uiManager->setMenu(InGameUI::RemoveMenu);

				uiManager->setHoverCheckBool(true);
				game->getPlayer(game->GetLocalPlayerId())->setLockedControls(true);
				cursorInvisible = false;
				break;
			case 41: //Settings
			{
				last = current;
				current = Gamestate::START;
				subState = 1;

				uiManager->setHoverCheckBool(true);
				uiManager->setRoleBool(false);
				uiManager->setFirstMenuSet(false);

				uiManager->LoadNextSet(UISets::Menu, winX, winY);
				uiManager->setOptionsSaved(optionsSavedData);
				uiManager->setMenu(MainMenu::Settings);
				inGameSettings = true;
				renderMenu = true;
				renderUI = false;

				std::string input = std::to_string(serverCam->getSensitivity());
				senseNrOfKeys = input.size();

				uiManager->clearText(scaleAndText::Sense, MainMenu::Settings);

				if (senseNrOfKeys > 4)
				{
					input.erase(4, input.size());
					senseNrOfKeys = input.size();
				}

				if (senseNrOfKeys != 1)
					uiManager->setText(input, scaleAndText::Sense, MainMenu::Settings); //Set sense object
				else
					uiManager->setText(input + ".", scaleAndText::Sense, MainMenu::Settings); //Set sense object

				oldTime = int(koth->getTimer());
				break;
			}
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
		else if (current != Gamestate::SERVER && current != Gamestate::START) // if we are in freecam, spectatiing some one eles or what not. need to update our player's roomid
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
	bool hoverTemp = false;

	if (win != 0)
	{
		glfwMakeContextCurrent(NULL);
		glfwHideWindow(win);
		glfwDestroyWindow(win);
		win = nullptr;
		//removeWindow();
	}
	if (!fullscreen)
		win = glfwCreateWindow(
			x, y, title.c_str(), NULL, NULL);
	else
		win = glfwCreateWindow(
			x, y, title.c_str(), glfwGetPrimaryMonitor(), NULL);

	i->setupCallbacks(win);

	glfwShowWindow(win);

	glfwMakeContextCurrent(win);

	//set vsync off f
	glfwSwapInterval(0);

	if (renderPipe)
	{
		for (size_t i = 0; i < MAX_CONNECT; i++)
		{
			renderPipe->removeTextObject(namePlates[i]);
		}

		if (uiManager != nullptr)
		{
			hoverTemp = uiManager->getHoverCheckBool();
			optionsSavedData = uiManager->getOptionsSaved();
			delete uiManager;
		}
		renderPipe->release();
		
		renderPipe = nullptr;

		uiManager = new UIManager();
		initPipeline();
		uiManager->init(&console, winX, winY);

		uiManager->HUD.skipBannerUpdate = true;
		uiManager->HUD.activeBanner = -1;
		uiManager->HUD.nrOfBanners = 4;
		uiManager->HUD.bannerCounter = 0;
		uiManager->HUD.bannerMaxTime.push_back(3);
		uiManager->HUD.bannerMaxTime.push_back(10);
		uiManager->HUD.bannerMaxTime.push_back(3);
		uiManager->HUD.bannerMaxTime.push_back(3);

		if (inGameSettings)
		{
			Gamestate tmp = current;
			current = last;
			last = tmp;
			subState = 2;

			renderMenu = false;
			renderUI = true;
			inGameSettings = false;
			uiManager->setFirstMenuSet(false);
			uiManager->LoadNextSet(UISets::InGame, winX, winY);
			uiManager->setMenu(InGameUI::GUI);

			if (warmUpBannerActive)
			{
				uiManager->resetHidableWorldMatrix(hideAbleObj::Banner, 0);
				uiManager->setHideableWorldMatrix(hideAbleObj::Banner, 0, vec2(0.0, 1.0));
				uiManager->changeTextureHideAble(hideAbleObj::Banner, 0, BannerTextureIDs::WarmUp);
				uiManager->HUD.activeBanner = ActiveBannerID::WarmUpRound;
				uiManager->HUD.skipBannerUpdate = true;
				uiManager->HUD.bannerCounter = 0;
			}
			playerIconsSet();
		}
		else if (renderMenu)
		{
			uiManager->LoadNextSet(UISets::Menu, winX, winY); //Load the first set of menus.
			uiManager->setMenu(MainMenu::StartMenu);

			uiManager->setHoverCheckBool(hoverTemp);

			uiManager->setOptionsSaved(optionsSavedData);

			justSetFullScreen = true;
		}
		else
		{
			uiManager->setFirstMenuSet(false);
			uiManager->LoadNextSet(UISets::InGame, winX, winY);
			uiManager->setMenu(InGameUI::GUI);
			uiManager->HUD.maxSpecialMeter = 100.0f;
			if (warmUpBannerActive)
			{
				uiManager->resetHidableWorldMatrix(hideAbleObj::Banner, 0);
				uiManager->setHideableWorldMatrix(hideAbleObj::Banner, 0, vec2(0.0, 1.0));
				uiManager->changeTextureHideAble(hideAbleObj::Banner, 0, BannerTextureIDs::WarmUp);
				uiManager->HUD.activeBanner = ActiveBannerID::WarmUpRound;
				uiManager->HUD.skipBannerUpdate = true;
				uiManager->HUD.bannerCounter = 0;
			}
			playerIconsSet();
		}

		if(top != nullptr)
			top->setNewUIPtr(uiManager);

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

		leaderBoardTextID = renderPipe->createTextObject("", 20, vec2(winX / 2, winY / 2), true);
		leaderBoardSmallTextID = renderPipe->createTextObject("", 15, vec2(winX / 2, winY / 2), true);

	}

	uiManager->setRenderPtr(renderPipe);
}

void Core::setfps(int fps)
{
	if (win != nullptr)
	{
		//if (game != nullptr)
		//{
		//	CameraInput* cam = CameraInput::getCam();
		//	string print("X: " + to_string(cam->getPos().x) + "  Y: " + to_string(cam->getPos().y) + "  Z: " + to_string(cam->getPos().z) + "  FPS: " + to_string(fps));
		//	glfwSetWindowTitle(win, print.c_str());
		//}
		//else
			glfwSetWindowTitle(win, (title + " " + to_string(fps)).c_str());
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

	escActive = false;

	uiManager->setHoverCheckBool(true);
	uiManager->setRoleBool(false);
	uiManager->setFirstMenuSet(false);
	cursorInvisible = false;
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	uiManager->LoadNextSet(UISets::Menu, winX, winY);
	uiManager->setMenu(MainMenu::StartMenu);
	uiManager->setOptionsSaved(optionsSavedData);
	if (GetSound())
	{
		GetSound()->playMusic(mainMenu);
		GetSound()->setVolumeMusic(GetSound()->getVolumeMusic());
		GetSound()->SetFading(false);
	}

	renderMenu = true;
	renderUI = false;
}

void Core::showTeamSelect()
{
	if (startTeamSelect)
	{
		uiManager->setHoverCheckBool(true);
		game->getPlayer(game->GetLocalPlayerId())->setLockedControls(true);
		cursorInvisible = false;

		if (renderMenu)
		{
			//optionsSavedData = uiManager->getOptionsSaved();
			uiManager->LoadNextSet(UISets::InGame, winX, winY);
			renderMenu = false;
		}
	
		uiManager->setFirstMenuSet(true);
		uiManager->setMenu(InGameUI::TeamSelect);
	}
	else
	{
		uiManager->setHoverCheckBool(false);
		uiManager->setFirstMenuSet(false);

		if (renderMenu)
		{
			//optionsSavedData = uiManager->getOptionsSaved();
			uiManager->LoadNextSet(UISets::InGame, winX, winY);
			renderMenu = false;
		}
		
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

void Core::playerIconsSet()
{
	Player* p = game->getPlayer(game->GetLocalPlayerId());
	Weapon* w = p->getPlayerCurrentWeapon();
	WEAPON_TYPE wt = w->getType();
	Role* r = p->getRole();

	if (p != nullptr && r->getRole() != 5)
	{
		switch (wt)
		{
		case 0: //Pulse Rifle
			uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::PulseRifle, InGameUI::GUI);
			break;
		case 2: //Disc Gun
			uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::Discgun, InGameUI::GUI);
			break;
		case 3: //Melee
			uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::Tail, InGameUI::GUI);
			break;
		case 4: //Battery slow, manipulator
		case 5: //Battery speed
			uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::BatteryShots, InGameUI::GUI);
			break;
		case 6: //Fusion Cannon
			uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::FusionCannon, InGameUI::GUI);
			break;
		case 9: //Grenade Launcher
			uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::GranadeLauncher, InGameUI::GUI);
			break;
		case 10: //Shotgun
			uiManager->scaleAndTextChangeTexture(scaleAndText::weapon, weapons::Shotgun, InGameUI::GUI);
			break;
		}
		switch (r->getRole())
		{
		case 0:
			uiManager->scaleAndTextChangeTexture(scaleAndText::consumable, consumables::Cluster, InGameUI::GUI);
			break;
		case 2:
			uiManager->scaleAndTextChangeTexture(scaleAndText::consumable, consumables::Dart, InGameUI::GUI);
			break;
		case 3:
			uiManager->scaleAndTextChangeTexture(scaleAndText::consumable, consumables::Thermite, InGameUI::GUI);
			break;
		case 4:
			uiManager->scaleAndTextChangeTexture(scaleAndText::consumable, consumables::Vortex, InGameUI::GUI);
			break;
		}
	}
	p = nullptr;
	w = nullptr;
	r = nullptr;
}

void Core::setHUDText(int menuID)
{
	uiManager->clearText(scaleAndText::HP, menuID);
	uiManager->clearText(scaleAndText::Ammo, menuID);

	Player* local = game->getPlayer(game->GetLocalPlayerId());
	KingOfTheHill* koth = (KingOfTheHill*)game->getGameMode();

	uiManager->HUD.HP = local->getMaxHP();
	uiManager->setText(std::to_string(local->getHP()), scaleAndText::HP, menuID); //hp

	std::string sFirst = "0";
	std::string sSecond = "0";

	//Ammo
	uiManager->HUD.ammo = local->getAmmo();
	int maxAmmo = local->getMaxAmmo();

	sFirst += std::to_string(uiManager->HUD.ammo);
	sSecond += std::to_string(maxAmmo);

	if (sFirst.size() == 3)
		sFirst = std::to_string(uiManager->HUD.ammo);
	else if (sFirst.size() > 3)
		sFirst = "99";
	if (sSecond.size() == 3)
		sSecond = std::to_string(maxAmmo);
	else if (sSecond.size() > 3)
		sSecond = "99";

	std::string nText = sFirst + "/" + sSecond;
	uiManager->setText(nText, scaleAndText::Ammo, menuID);
	//

	//Ticketbar1
	uiManager->HUD.teamOneTokens = koth->getRespawnTokens(1);

	sFirst = "0";
	sFirst += std::to_string(uiManager->HUD.teamOneTokens);

	if (sFirst.size() == 3)
		sFirst = std::to_string(uiManager->HUD.teamOneTokens);
	else if (sFirst.size() > 3)
		sFirst = "99";

	uiManager->setText(sFirst, scaleAndText::TicketBar1, menuID); //tickets
	//

	//Ticketbar2
	uiManager->HUD.teamTwoTokens = koth->getRespawnTokens(2);

	sFirst = "0";
	sFirst += std::to_string(uiManager->HUD.teamTwoTokens);

	if (sFirst.size() == 3)
		sFirst = std::to_string(uiManager->HUD.teamTwoTokens);
	else if (sFirst.size() > 3)
		sFirst = "99";

	uiManager->setText(sFirst, scaleAndText::TicketBar2, menuID); //tickets
	//

													  //Scores1
	uiManager->HUD.teamOneScore = koth->getRoundWins(1);

	sFirst = "0";
	sFirst += std::to_string(uiManager->HUD.teamOneScore);

	if (sFirst.size() == 3)
		sFirst = std::to_string(uiManager->HUD.teamOneScore);
	else if (sFirst.size() > 3)
		sFirst = "99";

	uiManager->setText(sFirst, scaleAndText::Wins1, menuID); //tickets
	//

	//Scores2
	uiManager->HUD.teamTwoScore = koth->getRoundWins(2);

	sFirst = "0";
	sFirst += std::to_string(uiManager->HUD.teamTwoScore);

	if (sFirst.size() == 3)
		sFirst = std::to_string(uiManager->HUD.teamTwoScore);
	else if (sFirst.size() > 3)
		sFirst = "99";

	uiManager->setText(sFirst, scaleAndText::Wins2, menuID); //tickets
	//

	if (int(koth->getTimer()) == 0)
	{
		uiManager->clearText(scaleAndText::Time, menuID);
		uiManager->setText("00:00", scaleAndText::Time, menuID); //time
	}

	uiManager->HUD.specialMeter = 0.0f;
	uiManager->HUD.maxSpecialMeter = 100.0f;
	uiManager->HUD.loseTicketPer = 14;

	//int temp = int(koth->getTimer());
	//int timeTemp = temp - oldTime;
	//timeTemp = timeTemp % uiManager->HUD.loseTicketPer;
	//uiManager->HUD.ticketLostTimer = timeTemp;

	uiManager->scaleBar(scaleAndText::LoseTicketsMeter, (float)(uiManager->HUD.ticketLostTimer) / (float)(uiManager->HUD.loseTicketPer), true, InGameUI::GUI);
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

				uiManager->setText(fromChar, scaleAndText::IP, MainMenu::Connect); //Set ip object
				ipNrOfKeys++;
			}
		}
	}
	if (i->justPressed(GLFW_KEY_BACKSPACE))
	{
		uiManager->removeLastInput(scaleAndText::IP, MainMenu::Connect); //remove from ip object
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

				uiManager->setText(fromChar, scaleAndText::Name, MainMenu::Connect); //Set ip object

				nameNrOfKeys++;
			}
		}
	}
	if (i->justPressed(GLFW_KEY_BACKSPACE))
	{
		uiManager->removeLastInput(scaleAndText::Name, MainMenu::Connect); //remove from ip object
		nameNrOfKeys--;
	}
}

void Core::menuSensitivityKeyInputUpdate()
{
	Input* i = Input::getInput();
	for (int c = 0; c < VALIDKEYSENS; c++)
	{
		if (i->justPressed(validKeyboardInputs3[c]))
		{
			if (senseNrOfKeys < 4)
			{
				char ch = i->keyToChar(validKeyboardInputs3[c]);
				std::string fromChar = "";
				fromChar += ch;

				uiManager->setText(fromChar, scaleAndText::Sense, MainMenu::Settings); //Set sense object

				senseNrOfKeys++;

				if (senseNrOfKeys == 1)
				{
					uiManager->setText(".", scaleAndText::Sense, MainMenu::Settings); //Set sense object

					senseNrOfKeys++;
				}
			}
		}
	}
	if (i->justPressed(GLFW_KEY_BACKSPACE))
	{
		uiManager->removeLastInput(scaleAndText::Sense, MainMenu::Settings); //remove from sense object
		senseNrOfKeys--;
		if (senseNrOfKeys < 0)
			senseNrOfKeys = 0;
		if (senseNrOfKeys == 1)
		{
			uiManager->removeLastInput(scaleAndText::Sense, MainMenu::Settings); //remove from sense object
			senseNrOfKeys--;
		}
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
	Player* p;
	if(game->spectateID == -1)
		p = game->getPlayer(game->GetLocalPlayerId());
	else
		p = game->getPlayer(game->spectateID);

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

void Core::effectsRender(int hackedTeam)
{
	renderPipe->enableBlend(true);
	//renderPipe->disableDepthTest();
	vec3 color;
	float lightwallOffset = 0;
	SpotLight light;
	int team;
	float percentLifeleft = 0;
	float rad = 0;

	light.Direction = vec3(0.0f);
	light.DiffuseIntensity = 0.2f;
	light.AmbientIntensity = 0.5f;
	light.attenuation.w = 5.0f;

	if (hackedTeam == 1)
		color = TEAMTWOCOLOR;
	else if (hackedTeam == 2)
		color = TEAMONECOLOR;

	//Light wall (Lightwall Shader)
	std::vector<Effect*> eff = game->getEffects(EFFECT_TYPE( EFFECT_TYPE::LIGHT_WALL));
	for (unsigned int i = 0; i < eff.size(); i++)
	{
		EFFECT_TYPE type = eff[i]->getType();
		int pid, eid;
		eff[i]->getId(pid, eid);
		team = eff[i]->getTeam();

		if (hackedTeam == -1)
		{
			if (team == 1)
				color = TEAMONECOLOR;
			else if (team == 2)
				color = TEAMTWOCOLOR;
		}

		LightwallEffect* asd = (LightwallEffect*)eff[i];
		renderPipe->renderWallEffect(&asd->getPos(), &asd->getEndPoint(), lightwallOffset, &color.x);
		lightwallOffset += glm::distance(asd->getPos(), asd->getEndPoint());
	}

	renderPipe->initRenderEffect();

	// Explosion shader objects	
	for (int c = THUNDER_DOME; c < HSCPICKUP; c++)
	{
		switch (c)
		{
		case EFFECT_TYPE::THUNDER_DOME:
		{
			eff = game->getEffects(EFFECT_TYPE(c));
			for (unsigned int i = 0; i < eff.size(); i++)
			{
				team = eff[i]->getTeam();

				if (hackedTeam == -1)
				{
					if (team == 1)
						color = TEAMONECOLOR;
					else if (team == 2)
						color = TEAMTWOCOLOR;
				}

				ThunderDomeEffect* asd = (ThunderDomeEffect*)eff[i];
				vec3 pos = asd->getPos();
				renderPipe->rendereffect(EFFECT_TYPE::THUNDER_DOME, &pos.x, asd->explotionRenderRad(), 0.7f, &color.x);
			}
		}
		break;
		case EXPLOSION:
		{
			eff = game->getEffects(EFFECT_TYPE(c));
			for (int i = 0; i < eff.size(); i++)
			{
				team = eff[i]->getTeam();

				if (hackedTeam == -1)
				{
					if (team == 1)
						color = TEAMONECOLOR;
					else if (team == 2)
						color = TEAMTWOCOLOR;
				}

				Explosion* asd = (Explosion*)eff[i];
				vec3 pos = asd->getPos();

				rad = asd->explosionRenderRad(&percentLifeleft);
				renderPipe->rendereffect(EFFECT_TYPE::EXPLOSION, &pos.x, rad * 1.3f, percentLifeleft, &color.x);
				light.attenuation.w = rad * 2 + 2.0f;
				light.Position = pos;
				light.DiffuseIntensity = 1.0f;
				if (percentLifeleft < 0.5f)
					light.DiffuseIntensity = (percentLifeleft * 2);

				light.Color = color;
				light.AmbientIntensity = 1.0f;
				renderPipe->addLight(&light, 0);
			}
		}
		break;
		case CLEANSENOVA:
		{
			eff = game->getEffects(EFFECT_TYPE(c));
			for (int i = 0; i < eff.size(); i++)
			{
				CleanseNova* asd = (CleanseNova*)eff[i];
				vec3 pos = asd->getPos();
				color = CLEANSENOVACOLOR;
				float inten = asd->lifepercentageleft();

				renderPipe->rendereffect(EFFECT_TYPE::CLEANSENOVA, &pos.x, asd->renderRad(), inten, &color.x);
			}
		}
		break;
		case BATTERY_SLOW:
		{
			eff = game->getEffects(EFFECT_TYPE(c));
			for (int i = 0; i < eff.size(); i++)
			{
				BatteryFieldSlow* asd = (BatteryFieldSlow*)eff[i];
				vec3 pos = asd->getPos();
				color = game->SLOWBUBBLECOLOR;
				renderPipe->rendereffect(EFFECT_TYPE::BATTERY_SLOW,&pos.x, asd->renderRad(), 0.8f, &color.x);
				light.attenuation.w = asd->renderRad();
				light.Color = color;
				light.Position = pos;
				light.AmbientIntensity = 0.1f;
				light.DiffuseIntensity = 1.0f;
				renderPipe->addLight(&light, 0);
			}
		}
		break;
		case BATTERY_SPEED:
		{
			eff = game->getEffects(EFFECT_TYPE(c));
			for (int i = 0; i < eff.size(); i++)
			{
				BatteryFieldSpeed* asd = (BatteryFieldSpeed*)eff[i];
				vec3 pos = asd->getPos();
				color = game->SPEEDBUBBLECOLOR;
				renderPipe->rendereffect(EFFECT_TYPE::BATTERY_SPEED,&pos.x, asd->renderRad(), 0.5f, &color.x);
				light.attenuation.w = asd->renderRad();
				light.Color = color;
				light.Position = pos;
				light.AmbientIntensity = 0.1f;
				light.DiffuseIntensity = 1.0f;
				renderPipe->addLight(&light, 0);
			}
		}
		break;
		case THERMITE_CLOUD:
		{
			eff = game->getEffects(EFFECT_TYPE(c));
			for (int i = 0; i < eff.size(); i++)
			{
				team = eff[i]->getTeam();

				if (hackedTeam == -1)
				{
					if (team == 1)
						color = TEAMONECOLOR;
					else if (team == 2)
						color = TEAMTWOCOLOR;
				}

				ThermiteCloud* asd = (ThermiteCloud*)eff[i];
				vec3 pos = asd->getPos();

				float inten = asd->lifetimepercentleft();

				if (inten > 0.8)
					inten = 1 - ((inten - 0.8f) / 0.2f);
				else if (inten < 0.2)
					inten = inten / 0.2f;
				else
					inten = 1.0f;

				renderPipe->rendereffect(EFFECT_TYPE::THERMITE_CLOUD,&pos.x, asd->explosionRenderRad(), inten, &color.x);
				light.attenuation.w = asd->explosionRenderRad() * 3;
				light.DiffuseIntensity = ((sin(timepass * 5) + 1) * 0.5f + 0.3f) * inten;
				light.Color = color;
				light.Position = pos;
				light.AmbientIntensity = 2.0f;
				renderPipe->addLight(&light, 0);
			}
		}
		break;
		case VACUUM:
		{
			eff = game->getEffects(EFFECT_TYPE(c));
			for (int i = 0; i < eff.size(); i++)
			{
				team = eff[i]->getTeam();
				if (hackedTeam == -1)
				{
					if (team == 1)
						color = TEAMONECOLOR;
					else if (team == 2)
						color = TEAMTWOCOLOR;
				}

				Vacuum* asd = (Vacuum*)eff[i];
				vec3 pos = asd->getPos();
				float inten =  1 - asd->lifepercentageleft();
				renderPipe->rendereffect(EFFECT_TYPE::VACUUM,&pos.x, asd->renderRad(), inten, &color.x);
				light.attenuation.w = asd->renderRad();
				light.Color = color;
				light.Position = pos;
				light.AmbientIntensity = 0.1f;
				renderPipe->addLight(&light, 0);
			}
		}
		break;
		case HEALTHPACK:
		{
			eff = game->getEffects(EFFECT_TYPE(c));
			for (int i = 0; i < eff.size(); i++)
			{
				vec3 pos = eff[i]->getPos();
				if (eff[i]->getTeam() == 1)
					color = TEAMTWOCOLOR;
				else
					color = TEAMONECOLOR;
				renderPipe->rendereffect(EFFECT_TYPE::HEALTHPACK,&pos.x, eff[i]->getInterestingVariable(), 1, &color.x);

				light.attenuation.w = eff[i]->getInterestingVariable();
				light.Color = color;
				light.Position = pos;
				light.DiffuseIntensity = 0.5f;
				light.attenuation.w = 5.0f;
				light.AmbientIntensity = 1.0f;
				renderPipe->addLight(&light, 0);
			}
		}
		break;
		}
	}

	//regular shader Double damage & HCS pickup
	renderPipe->initRenderRegular();
	//renderPipe->disableBlend();
	//renderPipe->enableDepthTest();
	//renderPipe->disableDepthMask();

	for (int c = EFFECT_TYPE::HSCPICKUP; c < EFFECT_TYPE::NROFEFFECTS; c++)
	{
		switch (c)
		{
		case HSCPICKUP:
		{
			eff = game->getEffects(EFFECT_TYPE(c));
			for (int i = 0; i < eff.size(); i++)
			{
				HSCPickup* temp = (HSCPickup*)eff[i];
				if (!temp->onCooldown())
				{
					vec3 pos = eff[i]->getPos();
					color = vec3(0, 0.2f, 1.0f);
					
					mat4 world;

					float rot = timepass;

					world[0].x = 2;
					world[1].y = 2;
					world[2].z = 2;

					world = world * mat4(cos(rot), 0.0f, -sin(rot), 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						sin(rot), 0.0f, cos(rot), 0.0f,
						0.0f, 0.0f, 0.0f, 0.0f);

					world[0].w = pos.x;
					world[1].w = pos.y + sin(timepass) * 0.2f;
					world[2].w = pos.z;

					float inten = ((sin(timepass * 3) + 1) * 0.2f) +0.3f;
					renderPipe->renderBullet(100, &world, &color[0], inten);

					light.attenuation.w = temp->renderRad();
					light.Color = color;
					light.Position = pos;
					light.DiffuseIntensity = inten * 0.7f;
					light.attenuation.w = 5.0f;
					light.AmbientIntensity = 1.0f;
					renderPipe->addLight(&light, 0);
				}
			}
		}
		break;
		case DOUBLEDAMAGEPICKUP:
		{
			eff = game->getEffects(EFFECT_TYPE(c));
			for (int i = 0; i < eff.size(); i++)
			{
				DoubleDamagePickup* temp = (DoubleDamagePickup*)eff[i];
				if (!temp->onCooldown())
				{
					vec3 pos = eff[i]->getPos();
					color = vec3(1, 0.2f, 0);

					mat4 world;

					world[0].x = 4;
					world[1].y = 4;
					world[2].z = 4;

					float rot = timepass * 3.0f;

					world = world * mat4(cos(rot), 0.0f, -sin(rot), 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						sin(rot), 0.0f, cos(rot), 0.0f,
						0.0f, 0.0f, 0.0f, 0.0f);

					world = glm::rotate(world, rot * 1.3f , vec3(1, 0, 0));

					world[0].w = pos.x;
					world[1].w = pos.y + sin(timepass) * 0.2f;
					world[2].w = pos.z;

					float inten = ((sin(timepass * 2) + 1) * 0.3f) +0.3f;
					renderPipe->renderBullet(101, &world, &color[0], inten * 0.5f);

					light.attenuation.w = temp->renderRad();
					light.Color = color;
					light.Position = pos;
					light.DiffuseIntensity = inten * 0.7f;
					light.attenuation.w = 8.0f;
					light.AmbientIntensity = 1.0f;
					renderPipe->addLight(&light, 0);
				}
			}
		}
		break;
		}
	}

	renderPipe->enableBlend(false);
	//renderPipe->disableDepthTest();
	//renderPipe->enableDepthMask();
}

std::string Core::hudTextOutPutManager(bool onlyOne, int first, int second)
{
	std::string sFirst = "0";
	std::string sSecond = "0";
	std::string nText = "";

	if (!onlyOne)
	{
		sFirst += std::to_string(first);
		sSecond += std::to_string(second);

		if (sFirst.size() == 3)
			sFirst = std::to_string(first);
		else if (sFirst.size() > 3)
			sFirst = "99";
		if (sSecond.size() == 3)
			sSecond = std::to_string(second);
		else if (sSecond.size() > 3)
			sSecond = "99";

		 nText = sFirst + "/" + sSecond;
	}
	else
	{
		sFirst += std::to_string(first);
		if (sFirst.size() == 3)
			sFirst = std::to_string(first);
		else if (sFirst.size() > 3)
			sFirst = "99";
		nText = sFirst;
	}
	return nText;
}

void Core::trailQuadsRender(int hackedTeam)
{		
	renderPipe->initRenderTrailQuad();

	vec3 campos = CameraInput::getCam()->getPos();
	vec3 color(1);
	vec3 pos;
	vec3 dir;
	vec3 crossdir;
	float inten;

	if (hackedTeam == 1)
		color = TEAMTWOCOLOR;
	else if (hackedTeam == 2)
		color = TEAMONECOLOR;

	for (int n = 0; n < BULLET_TYPE::NROFBULLETS; n++)
	{
		std::vector<Bullet*> bullets = game->getBullets(BULLET_TYPE(n));

		for (unsigned int i = 0; i < bullets.size(); i++)
		{
			if (hackedTeam == -1)
			{
				if (bullets[i]->getTeam() == 1)
					color = TEAMONECOLOR;
				else
					color = TEAMTWOCOLOR;
			}

			pos = bullets[i]->getPos();
			dir = -bullets[i]->getDir() * glm::length(bullets[i]->getVel()) * 0.05f;
			crossdir = normalize(cross(normalize(campos - pos), -dir)) * 0.05f;
			inten = bullets[i]->getCurrTTL();
			float maxttl = bullets[i]->getMaxTTL();
			if (inten < maxttl - 0.3f)
				inten = 1.0f;
			else
				inten = (maxttl - inten) / 0.3f;

			vec3 color2 = color * inten;

			renderPipe->renderTrailQUad(BULLET_TYPE::PULSE_SHOT, &pos.x, &dir.x, &crossdir.x, &color2[0]);
		}
	}
}

void Core::flamebarrelflicker()
{
	SpotLight light;
	//push Flame barrel flickering lights
	light.Color = { 1, 0.4f, 0.023f };
	light.Direction = {0,0,0};
	light.DiffuseIntensity = 2.5f + sin(timepass * 3.5f) * 0.15 + sin(timepass * 8) * 0.15f;
	light.Position = { 0.93f, 1.23f, 17.0f };
	light.AmbientIntensity = 1.0f;
	light.attenuation.w = 3.5f + sin(timepass * 3.5f) * 0.15 + sin(timepass * 8) * 0.1f;
	renderPipe->addLight(&light, 32);

	light.Position = { 90.14f, 1.51f, 80.1f };
	renderPipe->addLight(&light, 8);

	light.Position = { -90.0f, 1.24f, 11.306f };
	renderPipe->addLight(&light, 17);

	light.Position = { -85.3f, 1.29f, 49.12f };
	renderPipe->addLight(&light, 28);

	light.Position = { -29.15f, 1.16f, 104.6f };
	renderPipe->addLight(&light, 14);

	light.Position = { 27.54f, 1.36f, 108.91f };
	renderPipe->addLight(&light, 6);


}

void Core::renderIntoFly(float dt)
{
	if (renderPipe)
	{
		//temp set to see anims in 3p 
		CameraInput* cam = CameraInput::getCam();

		introFlyTimer -= dt;

		if (introFlyTimer < 0)
		{
			introFlyIndex++;
			if (introFlyIndex > 1)
				introFlyIndex = 0;

			switch (introFlyIndex)
			{
			// coast case 1: startFly = vec3(88,-1.3f, 31); endFly = vec3(28, -3.1f, 31); flyCamDir = vec3(0,0,-1); introFlyTimerStart = 15.0f; renderPipe->setCullingCurrentChunkID(1); break;
			case 0: startFly = vec3(98.0f, 9.0f, 35.0f); endFly = vec3(53.0f, 9.0f, 21.0f); flyCamDir = vec3(-0.33f, -0.025f, 0.94f); introFlyTimerStart = 15.0f; renderPipe->setCullingCurrentChunkID(15); break;
			case 1: startFly = vec3(-95.8, 5.6, 66.4); endFly = vec3(-85, 5.6, 81.5); flyCamDir = vec3(0.8, 0, -0.57); introFlyTimerStart = 15.0f; renderPipe->setCullingCurrentChunkID(31); break;
			}

			introFlyTimer = introFlyTimerStart;
		}

		float interpol = introFlyTimer / introFlyTimerStart;
		vec3 camPos = interpol * startFly + (1 - interpol) * endFly;

		cam->setCam(camPos, flyCamDir);

		glm::vec3 tmpEyePos = camPos;

		

		vec3 dgColor(0);

		renderPipe->update(tmpEyePos.x, tmpEyePos.y, tmpEyePos.z, dt); // sets the view/proj matrix
		renderPipe->renderIni();

		flamebarrelflicker();

		renderPipe->renderMISC(-3, (void*)&(CameraInput::getCam()->getSkyboxMat()), &dgColor.x, 1);
		renderPipe->disableBlend();
		renderPipe->render();

		// render effects
		//effectsRender(-1);
		renderPipe->enableDepthTest();
		renderPipe->enableDepthMask();

		renderPipe->enableBlend(false);

		renderPipe->finalizeRender();

		renderPipe->disableDepthTest();
		renderPipe->enableBlend(false);
	}
}