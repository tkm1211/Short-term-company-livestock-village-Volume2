#include "SceneGame.h"
#include "BG.h"
#include "Player.h"
#include "CPU.h"
#include "BlockManager.h"
#include "UI.h"
#include "Effect.h"
#include "Production.h"
#include "SceneManager.h"
#include "PauseScene.h"
#include "input_device.h"
#include "Sound.h"


/*--------------------------------------*/
//	Menber function
/*--------------------------------------*/
void SceneCPUGame::Init()
{
	gameMode = SelectGameMode::CPU;


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
	pause.Init();

	sceneMultiGame.isGameover[0] = sceneMultiGame.isGameover[1] = false;
	isGameReady = true;

	// Play BGM
	bgmRnd = rand() % Sound::GAME_BGM_MAX;
	switch (bgmRnd)
	{
	case 0:
		pAudio->Play(Sound::Get()->bgmHandle[Sound::BGM::GAME].get(), true);
		break;
	case 1:
		pAudio->Play(Sound::Get()->bgmHandle[Sound::BGM::GAME2].get(), true);
		break;
	case 2:
		pAudio->Play(Sound::Get()->bgmHandle[Sound::BGM::GAME3].get(), true);
		break;
	}

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
	pause.Uninit();

	// Stop BGM
	switch (bgmRnd)
	{
	case 0:
		pAudio->Stop(Sound::Get()->bgmHandle[Sound::BGM::GAME].get());
		pAudio->DeleteSourceVoice(Sound::Get()->bgmHandle[Sound::BGM::GAME].get());
		break;
	case 1:
		pAudio->Stop(Sound::Get()->bgmHandle[Sound::BGM::GAME2].get());
		pAudio->DeleteSourceVoice(Sound::Get()->bgmHandle[Sound::BGM::GAME2].get());
		break;
	case 2:
		pAudio->Stop(Sound::Get()->bgmHandle[Sound::BGM::GAME3].get());
		pAudio->DeleteSourceVoice(Sound::Get()->bgmHandle[Sound::BGM::GAME3].get());
		break;
	}
}

void SceneCPUGame::Update()
{
	if (PRODUCTION->CheckFlag(GO_CPUGAME))
		PRODUCTION->CSOH(SCENE_MANAGER->CPU_GAME);

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
	}


	if (PRODUCTION->CheckFlag(GO_CPUGAME) || PRODUCTION->CheckFlag(GO_TITLE)) return;

#if defined(DEBUG) | defined(_DEBUG)
	if (GetAsyncKeyState('N') & 1 || pad[0].bYt)
	{
		PRODUCTION->SetOn(GO_TITLE);
		PRODUCTION->Start();
	}
#endif
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

	pause.Draw();

	if (PRODUCTION->CheckFlag(GO_CPUGAME) || PRODUCTION->CheckFlag(GO_TITLE))
	{
		PRODUCTION->Draw();
	}
}