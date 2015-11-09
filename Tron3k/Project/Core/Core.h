#ifndef CORE_H
#define CORE_H

#include "Input.h"
#include "sound\Sound.h"
#include "ui\Ui.h"

#include "../Network/Client.h"
#include "../Network/Server.h"

#include "../../Engine/IEngine.h"
#include "../../Engine/Game/IGame.h"

class Core
{
private:
	Topology* top;
	IGame* game;
	IEngine* engine;

public:
	void init();
	void update(float dt);
};

#endif