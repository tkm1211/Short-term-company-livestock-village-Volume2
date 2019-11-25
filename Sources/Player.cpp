#include <input_device.h>
#include <Blender.h>
#include "SceneManager.h"
#include "GameMaster.h"
#include "Player.h"
#include "BlockManager.h"
#include "Production.h"
#include "Sound.h"
#include "UI.h"


//--------------------------------------

// PlayerManager

//--------------------------------------
//void PlayerManager::Init()
//{
//	for (int i = 0; i < sceneGame.playStyle + 1;i++)
//	{
//		player[i].Init();
//	}
//}
//
//void PlayerManager::Update()
//{
//	for (int i = 0; i < sceneGame.playStyle + 1;i++)
//	{
//		player[i].Update();
//	}
//	SetDrawBlendMode(BLEND_MODE::ALPHA);
//}
//
//void PlayerManager::Draw()
//{
//	for (int i = 0; i < sceneGame.playStyle + 1; i++)
//	{
//		player[i].Draw();
//	}
//}
//
//void PlayerManager::Uninit()
//{
//	for (int i = 0; i < sceneGame.playStyle + 1; i++)
//	{
//		player[i].Uninit();
//	}
//}


//--------------------------------------

// Player

//--------------------------------------
void Player::Init(int _playerNum)
{
	sprPickel = std::make_unique<SpriteBatch>(L"Data/UI.png", 1, DirectX::XMFLOAT2(114, 114));
	sprPlayer1[0] = std::make_unique<SpriteBatch>(L"Data/character01.png", 2, DirectX::XMFLOAT2(0.0f, 0.0f));
	sprPlayer1[1] = std::make_unique<SpriteBatch>(L"Data/character02.png", 2, DirectX::XMFLOAT2(0.0f, 0.0f));
	sprPlayer1[2] = std::make_unique<SpriteBatch>(L"Data/character03.png", 2, DirectX::XMFLOAT2(0.0f, 0.0f));
	sprPlayer1[3] = std::make_unique<SpriteBatch>(L"Data/character04.png", 2, DirectX::XMFLOAT2(0.0f, 0.0f));
	row = 2;
	column = 4;
	pos = DirectX::XMFLOAT2(static_cast<float>(row * 114), static_cast<float>(column * 114));

	if (!sceneGame.isTwoPlayer)
	{
		characterPos = DirectX::XMFLOAT2(150.0f, 327.0f);
	}
	else
	{
		switch (_playerNum)
		{
		case 0:
			characterPos = DirectX::XMFLOAT2(948.0f - 6.0f, 609.0f);
			break;
		case 1:
			characterPos = DirectX::XMFLOAT2(970.0f + 6.0f, 81.0f);
			break;
		}
	}
	for (auto& it : accelerationCount)
		it = 0;

	animCount = 0;
	animFrame = 0;

	count = 0;
	count2 = 0;
	count3 = 0;

	charTex.tex = DirectX::XMFLOAT2(0.0f, 0.0f);

	charAnimCnt = 0;
	charAnimFrame = 0;

	canMove = true;
	isPlaySoundBreak = false;
	isPlaySoundOjama = false;
}

void Player::Update(int _playerNum)
{
	if (!PRODUCTION->CheckFlag(GO_GAME) || !PRODUCTION->CheckFlag(GO_GAME))
	{
		charTex.tex.y = 0.0f;


		if ((sceneGame.gameMode == GameMode::ONE_PLAYER + 1 && sceneGame.isGameOver_one)
			|| ((!sceneGame.isGameOver_one && _playerNum == 0 && sceneGame.isGameOver_two) 
			|| (!sceneGame.isGameOver_two && _playerNum == 1 && sceneGame.isGameOver_one)))
		{
			if (charTex.tex.y != charTex.tex.y)
			{
				charAnimFrame = 0;
			}
			charTex.tex.y = 416.0f * 3.0f;
		}
		else if ((sceneGame.isGameOver_one && _playerNum == 0 && !sceneGame.isGameOver_two) || (sceneGame.isGameOver_two && _playerNum == 1 && !sceneGame.isGameOver_one))
		{
			if (charTex.tex.y != charTex.tex.y)
			{
				charAnimFrame = 0;
			}
			charTex.tex.y = 416.0f * 4.0f;
		}
		else if (GAME_MASTER->effect[_playerNum].isAleat)
		{
			if (charTex.tex.y != charTex.tex.y)
			{
				charAnimFrame = 0;
			}
			charTex.tex.y = 416.0f * 2.0f;
		}
		else if (GAME_MASTER->blockManager[_playerNum].isChainNow/* || GAME_MASTER->blockManager[_playerNum].isLastChain*/ || (GAME_MASTER->blockManager[_playerNum].pushUp && !GAME_MASTER->blockManager[_playerNum].pushUpT))
		{
			if (charTex.tex.y != charTex.tex.y)
			{
				charAnimFrame = 0;
			}
			charTex.tex.y = 416.0f * 1.0f;
		}
		else
		{
			if (charTex.tex.y != charTex.tex.y)
			{
				charAnimFrame = 0;
			}
			charTex.tex.y = 416.0f * 0.0f;
		}

		charTex.tex.x = 416.0f * charAnimFrame;
		// TODO : debug用の処理
		if (416.0f <= charTex.tex.y)
		{
			charTex.tex.x = 0.0f;
		}

		if (sceneGame.isGameOver_one || sceneGame.isGameOver_two || sceneGame.isStartInit)return;
		Move(_playerNum);

		Break(_playerNum);

		if (charAnimCnt++ % 5 == 0 && charAnimCnt != 0)
		{
			charAnimFrame++;
			if (6 <= charAnimFrame)
			{
				charAnimFrame = 0;
			}
		}
	}
#ifdef USE_IMGUI
//	ImGui::Begin("pos");
//	ImGui::SliderFloat("pos.x", &characterPos.x,0,1920);
//	ImGui::SliderFloat("pos.y", &characterPos.y,0,1080);
	//ImGui::SliderFloat("pos.x", &characterPos2.x, 0, 1920);
	//ImGui::SliderFloat("pos.y", &characterPos2.y, 0, 1080);
//	ImGui::End();
#endif
}

void Player::Draw(int _playerNum)
{
	// ピッケル
	if (!sceneGame.isTwoPlayer)
	{
		sprPickel->Begin();
		sprPickel->Draw(pos.x + ADJUST + ORIJIN_X, pos.y + ADJUST + ORIJIN_Y, 114, 114, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, animFrame);
		sprPickel->End();

		//キャラクター
		sprPlayer1[sceneGame.charNum[_playerNum]]->Begin();
		sprPlayer1[sceneGame.charNum[_playerNum]]->Draw(characterPos.x, characterPos.y, 320, 416, charTex.tex.x, charTex.tex.y, 320, 416, 0, 0, 0.0f);
		sprPlayer1[sceneGame.charNum[_playerNum]]->End();
	}
	else
	{
		TwoPlayDraw(_playerNum);
	}
}

void Player::Uninit()
{

}

/*************************************************
	移動関数
*************************************************/
void Player::Move(int _playerNum)
{
	if (pad[_playerNum].sY < 0)
	{
		if (++accelerationCount[0] == 1)
		{
			--column;
			pos.y -= Player::MOVING_WIDTH;
			pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
		}
		if (accelerationCount[0] >= ACCEL_START)
		{
			if (accelerationCount[0] % ACCEL_SPEED_PAR == 0)
			{
				--column;
				pos.y -= Player::MOVING_WIDTH;
				pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
			}
		}
	}
	else
	{
		accelerationCount[0] = 0;
	}

	if (0 < pad[_playerNum].sX)
	{
		if (++accelerationCount[1] == 1)
		{
			++row;
			pos.x += Player::MOVING_WIDTH;
			pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
		}
		if (accelerationCount[1] >= ACCEL_START)
		{
			if (accelerationCount[1] % ACCEL_SPEED_PAR == 0)
			{
				++row;
				pos.x += Player::MOVING_WIDTH;
				pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
			}
		}
	}
	else
	{
		accelerationCount[1] = 0;
	}

	if (0 < pad[_playerNum].sY)
	{
		if (++accelerationCount[2] == 1)
		{
			++column;
			pos.y += Player::MOVING_WIDTH;
			pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
		}
		if (accelerationCount[2] >= ACCEL_START)
		{
			if (accelerationCount[2] % ACCEL_SPEED_PAR == 0)
			{
				++column;
				pos.y += Player::MOVING_WIDTH;
				pAudio->Play(SOUND->seHandle[SOUND->MOVE]);

			}
		}
	}
	else
	{
		accelerationCount[2] = 0;
	}

	if (pad[_playerNum].sX < 0)
	{
		if (++accelerationCount[3] == 1)
		{
			--row;
			pos.x -= Player::MOVING_WIDTH;
			pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
		}
		if (accelerationCount[3] >= ACCEL_START)
		{
			if (accelerationCount[3] % ACCEL_SPEED_PAR == 0)
			{
				--row;
				pos.x -= Player::MOVING_WIDTH;
				pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
			}
		}
	}
	else
	{
		accelerationCount[3] = 0;
	}

	// Cross Button
	if (pad[_playerNum].bUPs)
	{
		if (++accelerationCount[0 + 4] == 1)
		{
			--column;
			pos.y -= Player::MOVING_WIDTH;
			pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
		}
		if (accelerationCount[0 + 4] >= ACCEL_START)
		{
			if (accelerationCount[0 + 4] % ACCEL_SPEED_PAR == 0)
			{
				--column;
				pos.y -= Player::MOVING_WIDTH;
				pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
			}
		}
	}
	else
	{
		accelerationCount[0 + 4] = 0;
	}

	if (pad[_playerNum].bRIGHTs)
	{
		if (++accelerationCount[1 + 4] == 1)
		{
			++row;
			pos.x += Player::MOVING_WIDTH;
			pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
		}
		if (accelerationCount[1 + 4] >= ACCEL_START)
		{
			if (accelerationCount[1 + 4] % ACCEL_SPEED_PAR == 0)
			{
				++row;
				pos.x += Player::MOVING_WIDTH;
				pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
			}
		}
	}
	else
	{
		accelerationCount[1 + 4] = 0;
	}

	if (pad[_playerNum].bDOWNs)
	{
		if (++accelerationCount[2 + 4] == 1)
		{
			++column;
			pos.y += Player::MOVING_WIDTH;
			pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
		}
		if (accelerationCount[2 + 4] >= ACCEL_START)
		{
			if (accelerationCount[2 + 4] % ACCEL_SPEED_PAR == 0)
			{
				++column;
				pos.y += Player::MOVING_WIDTH;
				pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
			}
		}
	}
	else
	{
		accelerationCount[2 + 4] = 0;
	}

	if (pad[_playerNum].bLEFTs)
	{
		if (++accelerationCount[3 + 4] == 1)
		{
			--row;
			pos.x -= Player::MOVING_WIDTH;
			pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
		}
		if (accelerationCount[3 + 4] >= ACCEL_START)
		{
			if (accelerationCount[3 + 4] % ACCEL_SPEED_PAR == 0)
			{
				--row;
				pos.x -= Player::MOVING_WIDTH;
				pAudio->Play(SOUND->seHandle[SOUND->MOVE]);
			}
		}
	}
	else
	{
		accelerationCount[3 + 4] = 0;
	}

	Correction(_playerNum);
}

void Player::Correction(int _playerNum)
{
	if (row > GAME_MASTER->blockManager[_playerNum].ROW_MAX - 1)
	{
		row = GAME_MASTER->blockManager[_playerNum].ROW_MAX - 1;
		pos.x -= Player::MOVING_WIDTH;
	}
	else if (row < 0)
	{
		row = 0;
		pos.x += Player::MOVING_WIDTH;
	}

	if (column > GAME_MASTER->blockManager[_playerNum].COLUMN_MAX - 1)
	{
		column = GAME_MASTER->blockManager[_playerNum].COLUMN_MAX - 1;
		pos.y -= Player::MOVING_WIDTH;
	}
	else if (column < 0)
	{
		column = 0;
		pos.y += Player::MOVING_WIDTH;
	}
}

void Player::Break(int _playerNum)
{
	if (!pad[_playerNum].bAt && pad[_playerNum].isButtomA)
	{
		if (10 <= pad[_playerNum].buttomAStopCnt++)
		{
			pad[_playerNum].isButtomA = false;
			pad[_playerNum].buttomAStopCnt = 0;
		}
	}
	if (pad[_playerNum].bAt || pad[_playerNum].bBt || pad[_playerNum].bXt || pad[_playerNum].bYt)
	{
		if (pad[_playerNum].bAt && !pad[_playerNum].isButtomA)
		{
			//pad[_playerNum].isButtomA = true;
			if (GAME_MASTER->blockManager[_playerNum].isChainNow || GAME_MASTER->blockManager[_playerNum].isChainNow)return;
			if (!isPlaySoundBreak)
			{
				pAudio->Play(SOUND->seHandle[SOUND->BREAK]);
				isPlaySoundBreak = true;
			}
			animFrame = 1;
			count = 0;
		}

		if (pad[_playerNum].bBt || pad[_playerNum].bXt || pad[_playerNum].bYt)
		{
			if (GAME_MASTER->blockManager[_playerNum].isChainNow || GAME_MASTER->blockManager[_playerNum].isChainNow)return;
			if (!isPlaySoundBreak)
			{
				pAudio->Play(SOUND->seHandle[SOUND->BREAK]);
				isPlaySoundBreak = true;
			}
			animFrame = 1;
			count = 0;
		}
	}
	else
	{
		if (++count >= 10)
		{
			isPlaySoundBreak = false;
			animFrame = 0;
			count = 0;
			isPlaySoundOjama = false;
		}
	}
	if (GAME_MASTER->blockManager[_playerNum].isBreak || GAME_MASTER->blockManager[_playerNum].isChain || GAME_MASTER->blockManager[_playerNum].isLastChain || GAME_MASTER->blockManager[_playerNum].pushUp)
	{
		animFrame = 2;
		return;
	}
	else
	{
		if (++count2 >= 10)
		{
			isPlaySoundBreak = false;
			animFrame = 0;
			count2 = 0;
			isPlaySoundOjama = false;
		}
	}

	if (pad[_playerNum].bAt || pad[_playerNum].bBt || pad[_playerNum].bXt || pad[_playerNum].bYt)
	{
		if (pad[_playerNum].bAt && !pad[_playerNum].isButtomA)
		{
			pad[_playerNum].isButtomA = true;
			//		animFrame = 1;
			for (size_t i = 0; i < GAME_MASTER->blockManager[_playerNum].block.size(); i++)
			{
				if (GAME_MASTER->blockManager[_playerNum].block[i].row == row && GAME_MASTER->blockManager[_playerNum].block[i].column == column)
				{
					if (GAME_MASTER->blockManager[_playerNum].block[i].color == 7)
					{
						if (!isPlaySoundOjama)
						{
							pAudio->Play(SOUND->seHandle[SOUND->OJAMA]);
							isPlaySoundOjama = true;
						}
						else
						{
							if (++count3 >= 10)
							{
								count3 = 0;
								isPlaySoundOjama = false;
							}
						}
					}
					else
					{
						GAME_MASTER->effect[_playerNum].GenerateParticle(GAME_MASTER->blockManager[_playerNum].block[i].row, GAME_MASTER->blockManager[_playerNum].block[i].column, GAME_MASTER->blockManager[_playerNum].block[i].color);
						int row_ = row;
						int column_ = column;
						GAME_MASTER->blockManager[_playerNum].isBreak = true;
						GAME_MASTER->ui[_playerNum].isTimerStop = true;
						GAME_MASTER->blockManager[_playerNum].block.erase(GAME_MASTER->blockManager[_playerNum].block.begin() + i);
						//	SetXInputVibration(45000, 45000, 10, _playerNum);
						pAudio->Play(SOUND->seHandle[SOUND->DESTRUCTION1]);
						//				BLOCK_MASTER->blockManager[0].block[i].isErase = true;
						//				BLOCK_MASTER->blockManager[0].block[i].eraseNum = i;

						GAME_MASTER->blockManager[_playerNum].RagisterFallBlock(row_, column_, _playerNum);
					}
				}
			}
		}
		if (pad[_playerNum].bBt || pad[_playerNum].bXt || pad[_playerNum].bYt)
		{
			for (size_t i = 0; i < GAME_MASTER->blockManager[_playerNum].block.size(); i++)
			{
				if (GAME_MASTER->blockManager[_playerNum].block[i].row == row && GAME_MASTER->blockManager[_playerNum].block[i].column == column)
				{
					if (GAME_MASTER->blockManager[_playerNum].block[i].color == 7)
					{
						if (!isPlaySoundOjama)
						{
							pAudio->Play(SOUND->seHandle[SOUND->OJAMA]);
							isPlaySoundOjama = true;
						}
						else
						{
							if (++count3 >= 10)
							{
								count3 = 0;
								isPlaySoundOjama = false;
							}
						}
					}
					else
					{
						GAME_MASTER->effect[_playerNum].GenerateParticle(GAME_MASTER->blockManager[_playerNum].block[i].row, GAME_MASTER->blockManager[_playerNum].block[i].column, GAME_MASTER->blockManager[_playerNum].block[i].color);
						int row_ = row;
						int column_ = column;
						GAME_MASTER->blockManager[_playerNum].isBreak = true;
						GAME_MASTER->ui[_playerNum].isTimerStop = true;
						GAME_MASTER->blockManager[_playerNum].block.erase(GAME_MASTER->blockManager[_playerNum].block.begin() + i);
						//	SetXInputVibration(45000, 45000, 10, _playerNum);
						pAudio->Play(SOUND->seHandle[SOUND->DESTRUCTION1]);
						//				BLOCK_MASTER->blockManager[0].block[i].isErase = true;
						//				BLOCK_MASTER->blockManager[0].block[i].eraseNum = i;

						GAME_MASTER->blockManager[_playerNum].RagisterFallBlock(row_, column_, _playerNum);
					}
				}
			}
		}
	}
//	else
//	{
//		animFrame = 0;
//	}
}

void Player::TwoPlayDraw(int _playerNum)
{
	switch (_playerNum)
	{
	case 0:
		sprPickel->Begin();
		sprPickel->Draw(pos.x + ADJUST + TWO_PLAY_ONE_ORIJIN_X, pos.y + ADJUST + ORIJIN_Y, 114, 114, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, animFrame);
		sprPickel->End();
		sprPlayer1[sceneGame.charNum[_playerNum]]->Begin();
		sprPlayer1[sceneGame.charNum[_playerNum]]->Draw(characterPos.x, characterPos.y, -160, 208, charTex.tex.x, charTex.tex.y, 320, 416, 0, 0, 0.0f);
		sprPlayer1[sceneGame.charNum[_playerNum]]->End();
		break;
	case 1:
		sprPickel->Begin();
		sprPickel->Draw(pos.x + ADJUST + TWO_PLAY_TWO_ORIJIN_X, pos.y + ADJUST + ORIJIN_Y, 114.0f, 114.0f, 0.0f + animFrame * 114.0f, 114.0f, 114.0f, 114.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		sprPickel->End();
		sprPlayer1[sceneGame.charNum[_playerNum]]->Begin();
		sprPlayer1[sceneGame.charNum[_playerNum]]->Draw(characterPos.x, characterPos.y, 160, 208, charTex.tex.x, charTex.tex.y, 320, 416, 0, 0, 0.0f);
		sprPlayer1[sceneGame.charNum[_playerNum]]->End();
		break;
	}
}