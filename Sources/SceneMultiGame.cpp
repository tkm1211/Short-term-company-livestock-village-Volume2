#include "SceneGame.h"

#include <input_device.h>

#include "BG.h"
#include "Production.h"
#include "SceneManager.h"
#include "Player.h"
#include "BlockManager.h"
#include "UI.h"
#include "Effect.h"
#include "PauseScene.h"
#include "Sound.h"

/*--------------------------------------*/
//	Menber function
/*--------------------------------------*/
void SceneMultiGame::Init()
{
	gameMode = SelectGameMode::Multi;


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
	pause.Init();

	isGameReady = true;

	// Play BGM
	pAudio->Play(Sound::Get()->bgmHandle[Sound::BGM::GAME].get(), true);
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
	pause.Uninit();

	// Stop BGM
	pAudio->Stop(Sound::Get()->bgmHandle[Sound::BGM::GAME].get());
	pAudio->DeleteSourceVoice(Sound::Get()->bgmHandle[Sound::BGM::GAME].get());
}

void SceneMultiGame::Update()
{
	if (PRODUCTION->CheckFlag(GO_MULTIGAME))
		PRODUCTION->CSOH(SCENE_MANAGER->MULTI_GAME);

	if (PRODUCTION->CheckFlag(GO_TITLE))
		PRODUCTION->CSOH(SCENE_MANAGER->TITLE);


	pause.Update();
	if (!pause.GetisPauseNow())
	{
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
		//BlockManager::UseImGui();

		for (int i = 0; i < 2; i++)
		{
			regularEffects[i].Update(i);
		}
		//Effect::UseImGui();
	}


	if (PRODUCTION->CheckFlag(GO_MULTIGAME) || PRODUCTION->CheckFlag(GO_TITLE)) return;

#if defined(DEBUG) | defined(_DEBUG)
	if (GetAsyncKeyState('N') & 1 || pad[0].bYt)
	{
		PRODUCTION->SetOn(GO_TITLE);
		PRODUCTION->Start();
	}
#endif
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
	pause.Draw();

	if (PRODUCTION->CheckFlag(GO_MULTIGAME) || PRODUCTION->CheckFlag(GO_TITLE))
	{
		PRODUCTION->Draw();
	}
}
