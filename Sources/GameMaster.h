#pragma once

#include "Singleton.h"
#include "BlockManager.h"
#include "Player.h"
#include "UI.h"
#include "Effect.h"

class GameMaster : public Singleton<GameMaster>
{
public:
	BlockManager	blockManager[2];
	Player			player[2];
	UI				ui[2];
	Effect			effect[2];

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};
#define GAME_MASTER GameMaster::getInstance()
