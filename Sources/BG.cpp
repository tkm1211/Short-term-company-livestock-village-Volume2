#include "BG.h"
#include "SceneManager.h"
#include "BlockManager.h"
#include "GameMaster.h"

void BG::Init()
{
	sprGrid = std::make_unique<SpriteBatch>(L"Data/frame.png",2);
	sprBackBG = std::make_unique<SpriteBatch>(L"Data/BG.png");
	sprGameBG = std::make_unique<SpriteBatch>(L"Data/gameBG.png");
}

void BG::Update()
{

}

void BG::Draw()
{
	sprBackBG->Begin();
	sprBackBG->Draw(0, 0, 1920, 1080, 0.0f);
	sprBackBG->End();

	if (!sceneGame.isTwoPlayer)
	{
		sprGameBG->Begin();
		sprGameBG->Draw(0, 270, 618, 540, 0.0f);
		sprGameBG->End();
	}

	sprGrid->Begin();
	if (!sceneGame.isTwoPlayer)
	{
		sprGrid->Draw(615, 12 + GAME_MASTER->blockManager[0].shakePos.y, 690, 1032, 0.0f);
	}
	else
	{
		if (sceneGame.isGameOver_one)
		{
			sprGrid->Draw(TWO_PLAY_ONE_ORIJIN_X, 12 + GAME_MASTER->blockManager[0].shakePos.y, 690, 1032, 0.0f);
			sprGrid->Draw(TWO_PLAY_TWO_ORIJIN_X, 12, 690, 1032, 0.0f);
		}
		else if (sceneGame.isGameOver_two)
		{
			sprGrid->Draw(TWO_PLAY_ONE_ORIJIN_X, 12, 690, 1032, 0.0f);
			sprGrid->Draw(TWO_PLAY_TWO_ORIJIN_X, 12 + GAME_MASTER->blockManager[1].shakePos2.y, 690, 1032, 0.0f);
		}
		else
		{
			sprGrid->Draw(TWO_PLAY_ONE_ORIJIN_X, 12, 690, 1032, 0.0f);
			sprGrid->Draw(TWO_PLAY_TWO_ORIJIN_X, 12, 690, 1032, 0.0f);
		}
	}
	sprGrid->End();
}

void BG::Uninit()
{

}