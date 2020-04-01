#include "Player.h"

#include <input_device.h>

#include "SceneManager.h"
#include "Resource.h"
#include "BlockManager.h"
#include "UI.h"

/*--------------------------------------*/
//	Global area
/*--------------------------------------*/

std::array<Player, 2> regularPlayer;


/*--------------------------------------*/
//	Menber function
/*--------------------------------------*/

void Player::Init()
{
	sprPickel = RESOURCE->GetSpriteData(Resource::Texture::UI);

	row = 2;
	column = 4;
	animFrame = 0;
	pos = DirectX::XMFLOAT2(static_cast<float>(row * 114), static_cast<float>(column * 114));

}

void Player::Uninit()
{
}

void Player::Update(int _pn)
{
	if (sceneSelect.gameMode == SceneSelect::SelectGameMode::Single && sceneSingleGame.GetIsGameReady()) return;
	if (sceneSelect.gameMode == SceneSelect::SelectGameMode::Multi && sceneMultiGame.GetIsGameReady()) return;
	if (sceneSelect.gameMode == SceneSelect::SelectGameMode::CPU && sceneCPUGame.GetIsGameReady()) return;

	OperatePlayer(_pn);

	SetBreakBlock(_pn);
}

void Player::Draw()
{
	sprPickel->Begin();
	sprPickel->Draw(pos.x + GameUI::ADJUST + GameUI::SINGLE_CORRECTION_X, pos.y + GameUI::ADJUST + GameUI::SINGLE_CORRECTION_Y, 114, 114, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, animFrame);
	sprPickel->End();
}

void Player::DrawOfSingle()
{
	sprPickel->Begin();
	sprPickel->Draw(pos.x + GameUI::ADJUST + GameUI::SINGLE_CORRECTION_X, pos.y + GameUI::ADJUST + GameUI::SINGLE_CORRECTION_Y, 114, 114, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, animFrame);
	sprPickel->End();
}

void Player::DrawOfMulti(int _pn)
{
	switch (_pn)
	{
	case 0:
		sprPickel->Begin();
		sprPickel->Draw(pos.x + GameUI::ADJUST + GameUI::MULTIPLAY_ONE_ORIJIN_X, pos.y + GameUI::ADJUST + GameUI::MULTI_CORRECTION_Y, 114.0f, 114.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, animFrame);
		sprPickel->End();
		break;

	case 1:
		sprPickel->Begin();
		sprPickel->Draw(pos.x + GameUI::ADJUST + GameUI::MULTIPLAY_TWO_ORIJIN_X, pos.y + GameUI::ADJUST + GameUI::MULTI_CORRECTION_Y, 114.0f, 114.0f, 0.0f + animFrame * 114.0f, 114.0f, 114.0f, 114.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		sprPickel->End();
		break;
	}
}


void Player::OperatePlayer(int _pn)
{
	if (pad[_pn].sLY < 0)
	{
		if (++accelerationCount[0] == 1)
		{
			--column;
			pos.y -= Player::MOVING_WIDTH;
		}
		if (accelerationCount[0] >= ACCEL_START)
		{
			if (accelerationCount[0] % ACCEL_MOVE_PER == 0)
			{
				--column;
				pos.y -= Player::MOVING_WIDTH;
			}
		}
	}
	else
	{
		accelerationCount[0] = 0;
	}

	if (0 < pad[_pn].sLX)
	{
		if (++accelerationCount[1] == 1)
		{
			++row;
			pos.x += Player::MOVING_WIDTH;
		}
		if (accelerationCount[1] >= ACCEL_START)
		{
			if (accelerationCount[1] % ACCEL_MOVE_PER == 0)
			{
				++row;
				pos.x += Player::MOVING_WIDTH;
			}
		}
	}
	else
	{
		accelerationCount[1] = 0;
	}

	if (0 < pad[_pn].sLY)
	{
		if (++accelerationCount[2] == 1)
		{
			++column;
			pos.y += Player::MOVING_WIDTH;
		}
		if (accelerationCount[2] >= ACCEL_START)
		{
			if (accelerationCount[2] % ACCEL_MOVE_PER == 0)
			{
				++column;
				pos.y += Player::MOVING_WIDTH;

			}
		}
	}
	else
	{
		accelerationCount[2] = 0;
	}

	if (pad[_pn].sLX < 0)
	{
		if (++accelerationCount[3] == 1)
		{
			--row;
			pos.x -= Player::MOVING_WIDTH;
		}
		if (accelerationCount[3] >= ACCEL_START)
		{
			if (accelerationCount[3] % ACCEL_MOVE_PER == 0)
			{
				--row;
				pos.x -= Player::MOVING_WIDTH;
			}
		}
	}
	else
	{
		accelerationCount[3] = 0;
	}

	// Cross Button
	if (pad[_pn].bUPs)
	{
		if (++accelerationCount[0 + 4] == 1)
		{
			--column;
			pos.y -= Player::MOVING_WIDTH;
		}
		if (accelerationCount[0 + 4] >= ACCEL_START)
		{
			if (accelerationCount[0 + 4] % ACCEL_MOVE_PER == 0)
			{
				--column;
				pos.y -= Player::MOVING_WIDTH;
			}
		}
	}
	else
	{
		accelerationCount[0 + 4] = 0;
	}

	if (pad[_pn].bRIGHTs)
	{
		if (++accelerationCount[1 + 4] == 1)
		{
			++row;
			pos.x += Player::MOVING_WIDTH;
		}
		if (accelerationCount[1 + 4] >= ACCEL_START)
		{
			if (accelerationCount[1 + 4] % ACCEL_MOVE_PER == 0)
			{
				++row;
				pos.x += Player::MOVING_WIDTH;
			}
		}
	}
	else
	{
		accelerationCount[1 + 4] = 0;
	}

	if (pad[_pn].bDOWNs)
	{
		if (++accelerationCount[2 + 4] == 1)
		{
			++column;
			pos.y += Player::MOVING_WIDTH;
		}
		if (accelerationCount[2 + 4] >= ACCEL_START)
		{
			if (accelerationCount[2 + 4] % ACCEL_MOVE_PER == 0)
			{
				++column;
				pos.y += Player::MOVING_WIDTH;
			}
		}
	}
	else
	{
		accelerationCount[2 + 4] = 0;
	}

	if (pad[_pn].bLEFTs)
	{
		if (++accelerationCount[3 + 4] == 1)
		{
			--row;
			pos.x -= Player::MOVING_WIDTH;
		}
		if (accelerationCount[3 + 4] >= ACCEL_START)
		{
			if (accelerationCount[3 + 4] % ACCEL_MOVE_PER == 0)
			{
				--row;
				pos.x -= Player::MOVING_WIDTH;
			}
		}
	}
	else
	{
		accelerationCount[3 + 4] = 0;
	}

	PositionCorreciton();
}

void Player::PushUpPlayer(int _pn)
{

}

void Player::PositionCorreciton()
{
	if (row > 6 - 1)
	{
		row = 6 - 1;
		pos.x -= Player::MOVING_WIDTH;
	}
	else if (row < 0)
	{
		row = 0;
		pos.x += Player::MOVING_WIDTH;
	}

	if (column > 9 - 1)
	{
		column = 9 - 1;
		pos.y -= Player::MOVING_WIDTH;
	}
	else if (column < 0)
	{
		column = 0;
		pos.y += Player::MOVING_WIDTH;
	}
}

void Player::SetBreakBlock(int _pn)
{
	if (regularBlockManager[_pn].GetStatus() != BlockManager::State::Wait) return;

	if (pad[_pn].bAt || pad[_pn].bBt || pad[_pn].bXt || pad[_pn].bYt)
	{
			// TODO : Correspond Multi Player
		{
			if (regularBlockManager[_pn].BreakBlock(row, column))
			{
				regularGameUI[_pn].SetIsTimerStop(true);
			}
		}
	}
}