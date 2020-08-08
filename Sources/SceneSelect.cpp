#include "SceneSelect.h"

#include <input_device.h>

#include "Resource.h"
#include "SceneManager.h"
#include "Production.h"
#include "Sound.h"

/*--------------------------------------*/
//	Global area
/*--------------------------------------*/

// unknown


/*--------------------------------------*/
//	Menber function
/*--------------------------------------*/
void SceneSelect::Init()
{
	sprSelect = RESOURCE->GetSpriteData(Resource::Texture::Select);
	gameMode = SelectGameMode::Single;

	// Play BGM
	pAudio->Play(Sound::Get()->bgmHandle[Sound::BGM::SELECT].get(), true);
}

void SceneSelect::Update()
{
	if (PRODUCTION->CheckFlag(GO_SINGLEGAME))
		PRODUCTION->CSOH(SCENE_MANAGER->SINGLE_GAME, true);
	if (PRODUCTION->CheckFlag(GO_PLAYERSTILE))
		PRODUCTION->CSOH(SCENE_MANAGER->SCENE_SELECT);
	if (PRODUCTION->CheckFlag(GO_MULTIGAME))
		PRODUCTION->CSOH(SCENE_MANAGER->MULTI_GAME, true);
	if (PRODUCTION->CheckFlag(GO_CPUGAME))
		PRODUCTION->CSOH(SCENE_MANAGER->CPU_GAME);


	if (PRODUCTION->CheckFlag(GO_PLAYERSTILE) || PRODUCTION->CheckFlag(GO_SINGLEGAME) || PRODUCTION->CheckFlag(GO_MULTIGAME) || PRODUCTION->CheckFlag(GO_CPUGAME)) return;

	if (GetAsyncKeyState('N') & 1 || pad[0].bAt)
	{
		PRODUCTION->SetOn(GO_SINGLEGAME);
		PRODUCTION->Start();
		gameMode = SelectGameMode::Single;
	}
	if (GetAsyncKeyState('M') & 1 || pad[0].bYt)
	{
		PRODUCTION->SetOn(GO_MULTIGAME);
		PRODUCTION->Start();
		gameMode = SelectGameMode::Multi;
	}
	if (GetAsyncKeyState('C') & 1 || pad[0].bXt)
	{
		PRODUCTION->SetOn(GO_CPUGAME);
		PRODUCTION->Start();
		gameMode = SelectGameMode::CPU;
	}
}

void SceneSelect::Draw()
{
	sprSelect->Begin();
	sprSelect->Draw(0, 0, 1440, 160, 0, 0, 1440, 160, 0, 0, 0.0f);
	sprSelect->End();

	if (PRODUCTION->CheckFlag(GO_PLAYERSTILE) || PRODUCTION->CheckFlag(GO_SINGLEGAME) || PRODUCTION->CheckFlag(GO_MULTIGAME) || PRODUCTION->CheckFlag(GO_CPUGAME))
	{
		PRODUCTION->Draw();
	}

}


void SceneSelect::Uninit()
{
	// Play BGM
	pAudio->Stop(Sound::Get()->bgmHandle[Sound::BGM::SELECT].get());
	pAudio->DeleteSourceVoice(Sound::Get()->bgmHandle[Sound::BGM::SELECT].get());
}


bool SceneSelect::JudgeGameMode(SelectGameMode _b)
{
	if (gameMode == _b)return true;

	return false;
}
