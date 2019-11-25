#include "GameMaster.h"
#include "SceneManager.h"


void GameMaster::Init()
{
	// Block
	for (int i = 0; i < sceneGame.gameMode; i++)
	{
		blockManager[i].Init(i);
	}

	// Player
	for (int i = 0; i < sceneGame.gameMode; i++)
	{
		player[i].Init(i);
	}

	// UI
	for (int i = 0; i < sceneGame.gameMode; i++)
	{
		ui[i].Init(i);
	}

	// Effect
	for (int i = 0; i < sceneGame.gameMode; i++)
	{
		effect[i].Init(i);
	}
}

void GameMaster::Update()
{
	// Player
	for (int i = 0; i < sceneGame.gameMode; i++)
	{
		player[i].Update(i);
	}

	// UI
	for (int i = 0; i < sceneGame.gameMode; i++)
	{
		ui[i].Update(i);
	}

	// Block
	for (int i = 0; i < sceneGame.gameMode; i++)
	{
		blockManager[i].Update(i);
	}

	// Effect
	for (int i = 0; i < sceneGame.gameMode; i++)
	{
		effect[i].Update(i);
	}

}

void GameMaster::Draw()
{
	// Block
	for (int i = 0; i < sceneGame.gameMode; i++)
	{
		blockManager[i].Draw(i);
	}

	// Player
	for (int i = 0; i < sceneGame.gameMode; i++)
	{
		player[i].Draw(i);
	}

	// UI
	for (int i = 0; i < sceneGame.gameMode; i++)
	{
		ui[i].Draw(i);
	}

	// Effect
	for (int i = 0; i < sceneGame.gameMode; i++)
	{
		effect[i].Draw(i);
	}

}

void GameMaster::Uninit()
{
	for (int i = 0; i < 2; i++)
	{
		blockManager[i].Uninit();
		player[i].Uninit();
		ui[i].Uninit();
		effect[i].Uninit();
	}
}