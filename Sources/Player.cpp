#include "Player.h"

#include <input_device.h>

#include "Resource.h"
#include "BlockManager.h"
#include "UI.h"

/*--------------------------------------*/
//	Global area
/*--------------------------------------*/

// unknown


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

void Player::Update()
{
	OperatePlayer();
	SetBreakBlock(0);
}

void Player::Draw()
{
	sprPickel->Begin();
	sprPickel->Draw(pos.x + GameUI::ADJUST + GameUI::SINGLE_CORRECTION_X, pos.y + GameUI::ADJUST + GameUI::SINGLE_CORRECTION_Y, 114, 114, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, animFrame);
	sprPickel->End();
}


void Player::OperatePlayer()
{
	if (pad[0].sY < 0)
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

	if (0 < pad[0].sX)
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

	if (0 < pad[0].sY)
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

	if (pad[0].sX < 0)
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
	if (pad[0].bUPs)
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

	if (pad[0].bRIGHTs)
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

	if (pad[0].bDOWNs)
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

	if (pad[0].bLEFTs)
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

void Player::SetBreakBlock(int _playerNum)
{
	if (provisionalBlockManager.GetStatus() != BlockManager::State::Wait) return;

	if (pad[0].bAt || pad[0].bBt || pad[0].bXt || pad[0].bYt)
	{
			// TODO : Correspond Multi Player
			{
				provisionalBlockManager.BreakBlock(row, column);
				provisionalGameUI.SetIsTimerStop(true);
			}
	}
}