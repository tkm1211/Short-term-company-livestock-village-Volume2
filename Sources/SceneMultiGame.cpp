#include "SceneGame.h"

#include <input_device.h>

#include "BG.h"
#include "Production.h"
#include "SceneManager.h"
#include "Player.h"
#include "BlockManager.h"
#include "UI.h"

/*--------------------------------------*/
//	Menber function
/*--------------------------------------*/
void SceneMultiGame::Init()
{
	BG_INSTANCE->Init();
}

void SceneMultiGame::Uninit()
{
	BG_INSTANCE->Uninit();
}

void SceneMultiGame::Update()
{
	if (PRODUCTION->CheckFlag(GO_MULTIGAME))
		PRODUCTION->CSOH(SCENE_MANAGER->MULTI_GAME);

	if (PRODUCTION->CheckFlag(GO_TITLE))
		PRODUCTION->CSOH(SCENE_MANAGER->TITLE);


	BG_INSTANCE->Update();


	if (PRODUCTION->CheckFlag(GO_MULTIGAME) || PRODUCTION->CheckFlag(GO_TITLE)) return;

	if (GetAsyncKeyState('N') & 1 || pad[0].bYt)
	{
		PRODUCTION->SetOn(GO_TITLE);
		PRODUCTION->Start();
	}

}

void SceneMultiGame::Draw()
{
	BG_INSTANCE->Draw();

	if (PRODUCTION->CheckFlag(GO_MULTIGAME) || PRODUCTION->CheckFlag(GO_TITLE))
	{
		PRODUCTION->Draw();
	}

}
