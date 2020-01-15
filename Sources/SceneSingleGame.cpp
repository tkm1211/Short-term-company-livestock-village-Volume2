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
	readyTimer = 0;
	isGameReady = true;

	BG_INSTANCE->Init();
	provisionalPlayer.Init();
	provisionalBlockManager.Init();
	provisionalGameUI.Init();
}

void SceneSingleGame::Uninit()
{
	BG_INSTANCE->Uninit();
	provisionalPlayer.Uninit();
	provisionalBlockManager.Uninit();
	provisionalGameUI.Uninit();
}

void SceneSingleGame::Update()
{
	if (PRODUCTION->CheckFlag(GO_SINGLEGAME))
		PRODUCTION->CSOH(SCENE_MANAGER->SINGLE_GAME);

	if (PRODUCTION->CheckFlag(GO_TITLE))
		PRODUCTION->CSOH(SCENE_MANAGER->TITLE);


	BG_INSTANCE->Update();
	provisionalPlayer.Update();
	provisionalGameUI.Update();
	provisionalBlockManager.Update();

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
	BG_INSTANCE->Draw();
	provisionalBlockManager.Draw();
	provisionalPlayer.Draw();
	provisionalGameUI.Draw();

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

