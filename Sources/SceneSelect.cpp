#include "SceneSelect.h"

#include <input_device.h>

#include "Resource.h"
#include "SceneManager.h"
#include "Production.h"

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
}

void SceneSelect::Update()
{
	if (PRODUCTION->CheckFlag(GO_SINGLEGAME))
		PRODUCTION->CSOH(SCENE_MANAGER->SINGLE_GAME);
	if (PRODUCTION->CheckFlag(GO_PLAYERSTILE))
		PRODUCTION->CSOH(SCENE_MANAGER->SCENE_SELECT);
	if (PRODUCTION->CheckFlag(GO_MULTIGAME))
		PRODUCTION->CSOH(SCENE_MANAGER->MULTI_GAME);


	if (PRODUCTION->CheckFlag(GO_PLAYERSTILE) || PRODUCTION->CheckFlag(GO_SINGLEGAME) || PRODUCTION->CheckFlag(GO_MULTIGAME)) return;

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
}

void SceneSelect::Draw()
{
	sprSelect->Begin();
	sprSelect->Draw(0, 0, 1440, 160, 0, 0, 1440, 160, 0, 0, 0.0f);
	sprSelect->End();

	if (PRODUCTION->CheckFlag(GO_PLAYERSTILE) || PRODUCTION->CheckFlag(GO_SINGLEGAME) || PRODUCTION->CheckFlag(GO_MULTIGAME))
	{
		PRODUCTION->Draw();
	}

}


void SceneSelect::Uninit()
{

}


bool SceneSelect::JudgeGameMode(SelectGameMode _b)
{
	if (gameMode == _b)return true;

	return false;
}
