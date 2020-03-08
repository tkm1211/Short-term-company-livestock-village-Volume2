#include "SceneGame.h"

#include <input_device.h>

#include "BG.h"
#include "Production.h"
#include "SceneManager.h"
#include "Player.h"
#include "BlockManager.h"
#include "UI.h"
#include "Effect.h"

/*--------------------------------------*/
//	Menber function
/*--------------------------------------*/
void SceneMultiGame::Init()
{
	gameMode = SceneSelect::SelectGameMode::Multi;


	BG_INSTANCE->Init();
	for (int i = 0; i < 2; i++)
	{
		regularPlayer[i].Init();
	}
	for (int i = 0; i < 2; i++)
	{
		regularBlockManager[i].Init(i);
	}
	for (int i = 0; i < 2; i++)
	{
		regularGameUI[i].Init(i);
	}
	for (int i = 0; i < 2; i++)
	{
		regularEffects[i].Init(i);
	}


	isGameReady = true;
}

void SceneMultiGame::Uninit()
{
	BG_INSTANCE->Uninit();
	for (int i = 0; i < 2; i++)
	{
		regularPlayer[i].Uninit();
		regularBlockManager[i].Uninit();
		regularGameUI[i].Uninit();
		regularEffects[i].Uninit(i);
	}
}

void SceneMultiGame::Update()
{
	if (PRODUCTION->CheckFlag(GO_MULTIGAME))
		PRODUCTION->CSOH(SCENE_MANAGER->MULTI_GAME);

	if (PRODUCTION->CheckFlag(GO_TITLE))
		PRODUCTION->CSOH(SCENE_MANAGER->TITLE);


	BG_INSTANCE->Update();
	for (int i = 0; i < 2; i++)
	{
		regularGameUI[i].Update(i);
	}
	for (int i = 0; i < 2; i++)
	{
		regularPlayer[i].Update(i);
	}
	for (int i = 0; i < 2; i++)
	{
		regularBlockManager[i].Update(i);
	}
	for (int i = 0; i < 2; i++)
	{
		regularEffects[i].Update(i);
	}


	if (PRODUCTION->CheckFlag(GO_MULTIGAME) || PRODUCTION->CheckFlag(GO_TITLE)) return;

	if (GetAsyncKeyState('N') & 1 || pad[0].bYt)
	{
		PRODUCTION->SetOn(GO_TITLE);
		PRODUCTION->Start();
	}

}

void SceneMultiGame::Draw()
{
	BG_INSTANCE->DrawOfMulti();
	for (int i = 0; i < 2; i++)
	{
		regularBlockManager[i].DrawOfMulti(i);
	}
	for (int i = 0; i < 2; i++)
	{
		regularPlayer[i].DrawOfMulti(i);
	}
	for (int i = 0; i < 2; i++)
	{
		regularGameUI[i].DrawOfMulti(i);
	}
	for (int i = 0; i < 2; i++)
	{
		regularEffects[i].DrawOfMulti(i);
	}

	if (PRODUCTION->CheckFlag(GO_MULTIGAME) || PRODUCTION->CheckFlag(GO_TITLE))
	{
		PRODUCTION->Draw();
	}
}
