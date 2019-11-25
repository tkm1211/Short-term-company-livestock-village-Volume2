#include "BlockMaster.h"
#include "Player.h"
#include <GamePad.h>


void BlockMaster::Init()
{
	for (auto& it : blockManager)
	{
		it.Init();
	}
}

void BlockMaster::Update()
{
	for (auto& it : blockManager)
	{
		it.Update();
		if (!PLAYER_MANAGER->playTwoPlayerMode)return;
	}

}

void BlockMaster::Draw()
{
	for (auto& it : blockManager)
	{
		it.Draw();
		if (!PLAYER_MANAGER->playTwoPlayerMode)return;
	}
}

void BlockMaster::Uninit()
{
	for (auto& it : blockManager)
	{
		it.Uninit();
	}
}

