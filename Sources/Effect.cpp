#include "Effect.h"
#include "SceneManager.h"
#include "BlockManager.h"
#include "GameMaster.h"
#include "easing.h"
#include "Sound.h"
#include <Blender.h>


void Effect::Init(int _playerNum)
{
	sprDeadLine = std::make_unique<SpriteBatch>(L"Data/effect.png", 1, DirectX::XMFLOAT2(684, 1032));
	sprParticle = std::make_unique<SpriteBatch>(L"Data/particle.png", 200, DirectX::XMFLOAT2(24, 24));

	alpha = 0;

	isPlus = true;
	isAleat = false;
	isLastAleat = false;
	isDisappearance = false;

	for (auto& it : particle)
	{
		it.pos = DirectX::XMFLOAT2(0.0f, 0.0f);
		it.velocity = DirectX::XMFLOAT2(0.0f, 0.0f);
		it.timer = 0;
		it.particleType = 0;
		it.color = -1;
		it.angle = 0.0f;
		it.alpha = 1.0f;
		it.maxTime = 0.0f;
		it.isEnable = false;
	}
}

void Effect::Update(int _playerNum)
{
	MoveParticle(_playerNum);

	if (sceneGame.isGameOver_one || sceneGame.isGameOver_two)
	{
		alpha = 0.0f;
	}
	if (sceneGame.isGameOver_one || sceneGame.isGameOver_two || sceneGame.isStartInit) return;
	// DeadLine
	if (isAleat)
	{
		switch (isPlus)
		{
		case true:
			alpha += 0.01f;
			if (alpha == 0.01f)
			{
				pAudio->Play(SOUND->seHandle[SOUND->ALEAT]);
			}
			if (alpha >= 0.8f)
			{
				alpha = 0.8f;
				isPlus = false;
			}
			break;
		case false:
			alpha -= 0.01f;
			if (alpha <= 0.2f)
			{
				alpha = 0.2f;
				isPlus = true;
			}
			break;
		}
	}
	else if (!isAleat && isLastAleat)
	{
		isDisappearance = true;
	}

	if (isDisappearance)
	{
		alpha -= 0.02f;
		if (alpha <= 0.0f)
		{
			isDisappearance = false;
		}
	}

}

void Effect::Draw(int _playerNum)
{
	SetDrawBlendMode(BLEND_MODE::ADD);

	if (!sceneGame.isTwoPlayer)
	{
		if (alpha != 0.0f)
		{
			sprDeadLine->Begin();
			//		sprDeadLine->Draw(615 + 3, 12, 684, 1032, 0.0f);
			sprDeadLine->Draw(615 + 3, 12, 684, 1032, 0, 0, 684, 1032, 0, 0, 0.0f, 1.0f, 1.0f, 1.0f, alpha);
			sprDeadLine->End();
		}
	}
	else
	{
		if (alpha != 0.0f)
		{
			sprDeadLine->Begin();
			if (_playerNum == 0)
			{
				//			sprDeadLine->Draw(TWO_PLAY_ONE_ORIJIN_X + ADJUST, 12, 684, 1032, 0.0f, 1.0f, 1.0f, 1.0f, alpha);
				sprDeadLine->Draw(TWO_PLAY_ONE_ORIJIN_X + ADJUST, 12, 684, 1032, 0, 0, 684, 1032, 0, 0, 0.0f, 1.0f, 1.0f, 1.0f, alpha);
				//			sprDeadLine->Draw(TWO_PLAY_ONE_ORIJIN_X + ADJUST, 12, 684, 1032, 0.0f);
			}
			else
			{
				//			sprDeadLine->Draw(TWO_PLAY_TWO_ORIJIN_X + ADJUST, 12, 684, 032, 0.0f, 1.0f, 1.0f, 1.0f, alpha);
				sprDeadLine->Draw(TWO_PLAY_TWO_ORIJIN_X + ADJUST, 12, 684, 1032, 0, 0, 684, 1032, 0, 0, 0.0f, 1.0f, 1.0f, 1.0f, alpha);
				//			sprDeadLine->Draw(TWO_PLAY_TWO_ORIJIN_X + ADJUST, 12, 684, 1032, 0.0f);

			}
			sprDeadLine->End();
		}
	}
	SetDrawBlendMode(BLEND_MODE::ALPHA);

	// Particle
	sprParticle->Begin();
	if (!sceneGame.isTwoPlayer)
	{
		for (auto& it : particle)
		{
			if (!it.isEnable) continue;

			//sprParticle->Draw(it.pos, DirectX::XMFLOAT2(24, 24), DirectX::XMFLOAT2(24 * it.animFrame), DirectX::XMFLOAT2(24, 24), DirectX::XMFLOAT2(0.5f, 0.5f), it.angle, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			sprParticle->Draw(it.pos.x + ORIJIN_X + ADJUST, it.pos.y + ORIJIN_Y + ADJUST, 48 * it.alpha, 48*it.alpha, 0.0f/*it.angle*/, 1.0f, 1.0f, 1.0f, 1.0f, it.particleType/*it.animFrame + 10 * it.color*/);
		}
	}
	else
	{
		if (_playerNum == 0)
		{
			for (auto& it : GAME_MASTER->effect[0].particle)
			{
				if (!it.isEnable) continue;

				//sprParticle->Draw(it.pos, DirectX::XMFLOAT2(24, 24), DirectX::XMFLOAT2(24 * it.animFrame), DirectX::XMFLOAT2(24, 24), DirectX::XMFLOAT2(0.5f, 0.5f), it.angle, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
				sprParticle->Draw(it.pos.x + TWO_PLAY_ONE_ORIJIN_X + ADJUST, it.pos.y + ORIJIN_Y + ADJUST, 48, 48, 0.0f/*it.angle*/, 1.0f, 1.0f, 1.0f, it.alpha, it.particleType);
			}

		}
		else
		{
			for (auto& it : GAME_MASTER->effect[1].particle)
			{
				if (!it.isEnable) continue;

				//sprParticle->Draw(it.pos, DirectX::XMFLOAT2(24, 24), DirectX::XMFLOAT2(24 * it.animFrame), DirectX::XMFLOAT2(24, 24), DirectX::XMFLOAT2(0.5f, 0.5f), it.angle, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
				sprParticle->Draw(it.pos.x + TWO_PLAY_TWO_ORIJIN_X + ADJUST, it.pos.y + ORIJIN_Y + ADJUST, 48, 48, 0.0f/*it.angle*/, 1.0f, 1.0f, 1.0f, it.alpha, it.particleType);
			}

		}
	}
	sprParticle->End();
}

void Effect::Uninit()
{

}


void Effect::GenerateParticle(int _row, int _column, int _color)
{
	for (int j = 0; j < 15; j++)
	{
		for (int i = 0; i < PARTICLE_MAX; i++)
		{
			if (particle[i].isEnable) continue;

			int rnd = rand() % 180;
			int typeRnd = rand() % 4 + 6;
			float speedRnd = (float)(rand() % 3 + 3);

			particle[i].maxTime = (float)(rand() % 30 + 15);

			particle[i].pos = DirectX::XMFLOAT2(114.0f * _row + 57.0f, 114.0f * _column + 57.0f);
			particle[i].angle = static_cast<float>(rnd);
			particle[i].color = _color;
			particle[i].particleType = particle[i].color * 10 + typeRnd;
			particle[i].timer = 0;
			particle[i].velocity = DirectX::XMFLOAT2(cosf(particle[i].angle)*speedRnd, sinf(particle[i].angle)*speedRnd);
			particle[i].isEnable = true;
			break;
		}
	}
}

void Effect::MoveParticle(int _playerNum)
{
	if (_playerNum == 0)
	{
		for (auto& it : particle)
		{
			if (!it.isEnable) continue;
			it.velocity.y += 0.6f;

			it.pos.x += it.velocity.x;
			it.pos.y += it.velocity.y;
			it.angle += 5.0f;

			++it.timer;

			it.alpha = easing::InExp((float)it.timer,it.maxTime,0.0f,1.0f);

			if (it.timer >= it.maxTime)
			{
				it.isEnable = false;
			}
		}
	}
	else
	{
		for (auto& it : particle)
		{
			if (!it.isEnable) continue;
			it.velocity.y += 0.6f;

			it.pos.x += it.velocity.x;
			it.pos.y += it.velocity.y;
			it.angle += 5.0f;

			++it.timer;

			it.alpha = easing::InExp((float)it.timer, it.maxTime, 0.0f, 1.0f);

			if (it.timer >= it.maxTime)
			{
				it.isEnable = false;
			}
		}
	}
}