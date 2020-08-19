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
	gameMode = SelectGameMode::Single;
	readyTimer = 0;
	isGameReady = true;
	isGameover = false;

	BG_INSTANCE->Init();
	regularPlayer[0].Init();
	regularBlockManager[0].Init();
	regularGameUI[0].Init();
	regularEffects[0].Init(0);
	pause.Init();

	// Play BGM
	pAudio->Play(Sound::Get()->bgmHandle[Sound::BGM::GAME2].get(), true);
}

void SceneSingleGame::Uninit()
{
	BG_INSTANCE->Uninit();
	regularPlayer[0].Uninit();
	regularBlockManager[0].Uninit();
	regularGameUI[0].Uninit();
	regularEffects[0].Uninit(0);
	pause.Uninit();

	// Stop BGM
	pAudio->Stop(Sound::Get()->bgmHandle[Sound::BGM::GAME2].get());
	pAudio->DeleteSourceVoice(Sound::Get()->bgmHandle[Sound::BGM::GAME2].get());
}

void SceneSingleGame::Update()
{
	if (PRODUCTION->CheckFlag(GO_SINGLEGAME))
		PRODUCTION->CSOH(SCENE_MANAGER->SINGLE_GAME);

	if (PRODUCTION->CheckFlag(GO_TITLE))
		PRODUCTION->CSOH(SCENE_MANAGER->TITLE);


	pause.Update();
	if (!pause.GetisPauseNow())
	{
		BG_INSTANCE->Update();
		regularPlayer[0].Update();
		regularGameUI[0].Update();
		regularBlockManager[0].Update();
		regularEffects[0].Update(0);
	}


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
	regularEffects[0].DrawOfSingle();
	pause.Draw();

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

