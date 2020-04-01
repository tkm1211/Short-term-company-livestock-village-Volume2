#include "SceneGame.h"
#include "BG.h"
#include "Player.h"
#include "CPU.h"
#include "BlockManager.h"
#include "UI.h"
#include "Effect.h"
#include "Production.h"
#include "SceneManager.h"
#include "input_device.h"


/*--------------------------------------*/
//	Menber function
/*--------------------------------------*/
void SceneCPUGame::Init()
{
	gameMode = SceneSelect::SelectGameMode::CPU;


	BG_INSTANCE->Init();
	regularPlayer[0].Init();
	regularCPU.Init();
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

void SceneCPUGame::Uninit()
{
	BG_INSTANCE->Uninit();
	regularPlayer[0].Uninit();
	regularCPU.UnInit();
	for (int i = 0; i < 2; i++)
	{
		regularBlockManager[i].Uninit();
		regularGameUI[i].Uninit();
		regularEffects[i].Uninit(i);
	}
}

void SceneCPUGame::Update()
{
	if (PRODUCTION->CheckFlag(GO_CPUGAME))
		PRODUCTION->CSOH(SCENE_MANAGER->CPU_GAME);

	if (PRODUCTION->CheckFlag(GO_TITLE))
		PRODUCTION->CSOH(SCENE_MANAGER->TITLE);


	BG_INSTANCE->Update();
	for (int i = 0; i < 2; i++)
	{
		regularGameUI[i].Update(i);
	}

	regularPlayer[0].Update(0);
	regularCPU.Update();

	for (int i = 0; i < 2; i++)
	{
		regularBlockManager[i].Update(i);
	}
	for (int i = 0; i < 2; i++)
	{
		regularEffects[i].Update(i);
	}


	if (PRODUCTION->CheckFlag(GO_CPUGAME) || PRODUCTION->CheckFlag(GO_TITLE)) return;

	if (GetAsyncKeyState('N') & 1 || pad[0].bYt)
	{
		PRODUCTION->SetOn(GO_TITLE);
		PRODUCTION->Start();
	}
}

void SceneCPUGame::Draw()
{
	BG_INSTANCE->DrawOfMulti();
	for (int i = 0; i < 2; i++)
	{
		regularBlockManager[i].DrawOfMulti(i);
	}

	regularPlayer[0].DrawOfMulti(0);
	regularCPU.Draw();

	for (int i = 0; i < 2; i++)
	{
		regularGameUI[i].DrawOfMulti(i);
	}
	for (int i = 0; i < 2; i++)
	{
		regularEffects[i].DrawOfMulti(i);
	}

	if (PRODUCTION->CheckFlag(GO_CPUGAME) || PRODUCTION->CheckFlag(GO_TITLE))
	{
		PRODUCTION->Draw();
	}
}