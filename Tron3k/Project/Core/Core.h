#ifndef CORE_H
#define CORE_H

#include "Input.h"
#include "sound\Sound.h"
#include "ui\Ui.h"

#include "../Network/Client.h"
#include "../Network/Server.h"

#include "../../Engine/Engine.h"
#include "../../Engine/Game/Game.h"

class Core
{
private:
	Topology* top;
	Game* game;
	Engine* engine;

public:
	void init();
	void update(float dt);
};

#endif