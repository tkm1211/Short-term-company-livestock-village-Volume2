#include "SceneGame.h"

#include <input_device.h>

#include "BG.h"
#include "Production.h"
#include "SceneManager.h"
#include "Player.h"
#include "BlockManager.h"
#include "UI.h"

/*--------------------------------------*/
//	Global area
/*--------------------------------------*/

Player provisionalPlayer;
BlockManager provisionalBlockManager;

/*--------------------------------------*/
//	Menber function
/*--------------------------------------*/
void SceneSingleGame::Init()
{
	// Member variable
	gameMode = SceneSelect::SelectGameMode::Single;
	readyTimer = 0;
	isGameReady = true;

	BG_INSTANCE->Init();
	regularPlayer[0].Init();
	regularBlockManager[0].Init();
	regularGameUI[0].Init();
}

void SceneSingleGame::Uninit()
{
	BG_INSTANCE->Uninit();
	regularPlayer[0].Uninit();
	regularBlockManager[0].Uninit();
	regularGameUI[0].Uninit();
}

void SceneSingleGame::Update()
{
	if (PRODUCTION->CheckFlag(GO_SINGLEGAME))
		PRODUCTION->CSOH(SCENE_MANAGER->SINGLE_GAME);

	if (PRODUCTION->CheckFlag(GO_TITLE))
		PRODUCTION->CSOH(SCENE_MANAGER->TITLE);


	BG_INSTANCE->Update();
	regularPlayer[0].Update();
	regularGameUI[0].Update();
	regularBlockManager[0].Update();

	//ProcessOfGameReady();


	if (PRODUCTION->CheckFlag(GO_SINGLEGAME) || PRODUCTION->CheckFlag(GO_TITLE)) return;

	if (GetAsyncKeyState('N') & 1 || pad[0].bYt)
	{
		PRODUCTION->SetOn(GO_TITLE);
		PRODUCTION->Start();
	}
}

void SceneSingleGame::Draw()
{
	BG_INSTANCE->DrawOfSingle();
	regularBlockManager[0].DrawOfSingle();
	regularPlayer[0].DrawOfSingle();
	regularGameUI[0].DrawOfSingle();

	if (PRODUCTION->CheckFlag(GO_SINGLEGAME) || PRODUCTION->CheckFlag(GO_TITLE))
	{
		PRODUCTION->Draw();
	}
}

void SceneSingleGame::ProcessOfGameReady()
{
	if (!isGameReady)return;

	if (++readyTimer >= 180)
	{
		readyTimer = 0;
		isGameReady = false;
	}
}

