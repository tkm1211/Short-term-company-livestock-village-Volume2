#include "SceneGame.h"

#include "BG.h"
#include "Production.h"
#include "SceneManager.h"
#include "Player.h"
#include "BlockManager.h"

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
	BG_INSTANCE->Init();
	provisionalPlayer.Init();
	provisionalBlockManager.Init();
}

void SceneSingleGame::Uninit()
{
	BG_INSTANCE->Uninit();
	provisionalPlayer.Uninit();
	provisionalBlockManager.Uninit();
}

void SceneSingleGame::Update()
{
	if (PRODUCTION->CheckFlag(GO_SINGLEGAME))
		PRODUCTION->CSOH(SCENE_MANAGER->SINGLE_GAME);

	if (PRODUCTION->CheckFlag(GO_TITLE))
		PRODUCTION->CSOH(SCENE_MANAGER->TITLE);

	if (PRODUCTION->CheckFlag(GO_SINGLEGAME) || PRODUCTION->CheckFlag(GO_TITLE)) return;

	BG_INSTANCE->Update();
	provisionalPlayer.Update();
	provisionalBlockManager.Update();
}

void SceneSingleGame::Draw()
{
	BG_INSTANCE->Draw();
	provisionalBlockManager.Draw();
	provisionalPlayer.Draw();

	if (PRODUCTION->CheckFlag(GO_SINGLEGAME) || PRODUCTION->CheckFlag(GO_TITLE))
	{
		PRODUCTION->Draw();
	}
}

