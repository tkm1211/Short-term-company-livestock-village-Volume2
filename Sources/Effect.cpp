#include "Blender.h"

#include "SceneManager.h"
#include "Effect.h"
#include "easing.h"
#include "UI.h"

/*--------------------------------------*/
//	Global area
/*--------------------------------------*/
std::array<Effect, 2> regularEffects;

/*--------------------------------------*/
//	Menber function
/*--------------------------------------*/
void Effect::Init(int _pn)
{
	sprDeadLine = std::make_unique<SpriteBatch>(L"Data/effect.png", 1, DirectX::XMFLOAT2(684, 1032));
	sprParticle = std::make_unique<SpriteBatch>(L"Data/particle.png", 200, DirectX::XMFLOAT2(24, 24));
	sprSmokeS = std::make_unique<SpriteBatch>(L"Data/SmokeS.png", 10, DirectX::XMFLOAT2(276, 114));
	sprSmokeL = std::make_unique<SpriteBatch>(L"Data/SmokeL.png", 10, DirectX::XMFLOAT2(582, 114));

	
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

	for (auto& it : smokeS)
	{
		it.anmCnt = 0;
		it.anmFrm = 0;
		it.isEnable = false;
		it.pos = DirectX::XMFLOAT2(0.0f, 0.0f);
	}

	isShowSmoke = false;
	isShowSmokeL = false;
}

void Effect::Uninit(int _pn)
{
}

void Effect::Update(int _pn)
{
	MoveParticle(_pn);
	UpdateOfSmoke();
	UpdateOfSmokeL(_pn);
}

void Effect::Draw(int _pn)
{
	SetDrawBlendMode(BLEND_MODE::ADD);

	// Draw Aleat.
	if (!sceneSelect.gameMode == SceneSelect::SelectGameMode::Single)
	{

	}
	else
	{

	}

	SetDrawBlendMode(BLEND_MODE::ALPHA);

	// Particle
	sprParticle->Begin();

	if (sceneSelect.gameMode == SceneSelect::SelectGameMode::Single)
	{
		for (auto& it : particle)
		{
			if (!it.isEnable)continue;

			sprParticle->Draw(it.pos.x + GameUI::SINGLE_CORRECTION_X + GameUI::ADJUST, it.pos.y + GameUI::SINGLE_CORRECTION_Y + GameUI::ADJUST, 48 * it.alpha, 48 * it.alpha, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, it.particleType);
		}
	}
	else
	{
		if (_pn == 0)
		{
			for (auto& it : particle)
			{
				if (!it.isEnable)continue;

				sprParticle->Draw(it.pos.x + GameUI::MULTIPLAY_ONE_ORIJIN_X + GameUI::ADJUST, it.pos.y + GameUI::MULTI_CORRECTION_Y + GameUI::ADJUST, 48, 48, 0.0f, 1.0f, 1.0f, 1.0f, it.alpha, it.particleType);
			}
		}
		else
		{
			for (auto& it : particle)
			{
				if (!it.isEnable)continue;

				sprParticle->Draw(it.pos.x + GameUI::MULTIPLAY_TWO_ORIJIN_X + GameUI::ADJUST, it.pos.y + GameUI::MULTI_CORRECTION_Y + GameUI::ADJUST, 48, 48, 0.0f, 1.0f, 1.0f, 1.0f, it.alpha, it.particleType);
			}
		}
	}

	sprParticle->End();
}


void Effect::DrawOfSingle()
{
	// Draw Aleat
	SetDrawBlendMode(BLEND_MODE::ADD);
	{

	}
	SetDrawBlendMode(BLEND_MODE::ALPHA);


	// Draw Particle
	{
		sprParticle->Begin();

		for (auto& it : particle)
		{
			if (!it.isEnable)continue;

			sprParticle->Draw(it.pos.x + GameUI::SINGLE_CORRECTION_X + GameUI::ADJUST, it.pos.y + GameUI::SINGLE_CORRECTION_Y + GameUI::ADJUST, 48 * it.alpha, 48 * it.alpha, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, it.particleType);
		}

		sprParticle->End();
	}
}

void Effect::DrawOfMulti(int _pn)
{
	//sprSmokeS->Begin();
	//sprSmokeS->Draw(0 + GameUI::MULTIPLAY_ONE_ORIJIN_X - 80, 114 * 5 + GameUI::MULTI_CORRECTION_Y + GameUI::ADJUST, 276, 114, 0);
	//sprSmokeS->End();


	// Draw Particle.
	sprParticle->Begin();

	if (_pn == 0)
	{
		for (auto& it : particle)
		{
			if (!it.isEnable)continue;

			sprParticle->Draw(it.pos.x + GameUI::MULTIPLAY_ONE_ORIJIN_X - 80, it.pos.y + GameUI::MULTI_CORRECTION_Y + GameUI::ADJUST, 48, 48, 0.0f, 1.0f, 1.0f, 1.0f, it.alpha, it.particleType);
		}

	}
	else
	{
		for (auto& it : particle)
		{
			if (!it.isEnable)continue;

			sprParticle->Draw(it.pos.x + GameUI::MULTIPLAY_TWO_ORIJIN_X - 80, it.pos.y + GameUI::MULTI_CORRECTION_Y + GameUI::ADJUST, 48, 48, 0.0f, 1.0f, 1.0f, 1.0f, it.alpha, it.particleType);
		}


	}

	sprParticle->End();


	// Draw Smoke
	if (isShowSmoke)
	{
		DrawSmokeS(_pn);
	}


	// Draw SmokeL
	if (isShowSmokeL)
	{
		DrawSmokeL(_pn);
	}

}



void Effect::GenerateParticle(int _row, int _column, int _color)
{
	for (int j = 0; j < 15; j++)
	{
		for (auto& it : particle)
		{
			if (it.isEnable)continue;

			int rnd = rand() % 180;
			int typeRnd = rand() % 4 + 6;
			float speedRnd = (float)(rand() % 3 + 3);

			it.maxTime = (float)(rand() % 30 + 15);

			it.pos = DirectX::XMFLOAT2(114.0f * _row + 57.0f, 114.0f * _column + 57.0f);
			it.angle = static_cast<float>(rnd);
			it.color = _color;
			it.particleType = it.color * 10 + typeRnd;
			it.timer = 0;
			it.velocity = DirectX::XMFLOAT2(cosf(it.angle) * speedRnd, sinf(it.angle) * speedRnd);
			it.isEnable = true;
			break;
		}
	}
}

void Effect::MoveParticle(int _pn)
{
	for (auto& it : particle)
	{
		if (!it.isEnable)continue;

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



bool Effect::JudgeIsShowingSmoke()
{
	for (auto& it : smokeS)
	{
		if (it.isEnable)return false;
	}

	return true;
}

void Effect::UpdateOfSmoke()
{
	if (!isShowSmoke)return;

	for (auto& it : smokeS)
	{
		if (++it.anmCnt % 6 == 0)
		{
			if (++it.anmFrm >= 6)
			{
				it.isEnable = false;
			}
		}
	}

	JudgeIsShowingSmoke();
}

void Effect::UpdateOfSmokeL(int _pn)
{
	if (!isShowSmokeL) return;

	if (++smokeL.anmCnt % 6 == 0)
	{
		if (++smokeL.anmFrm >= 9)
		{
			smokeL.isEnable = false;
			SetIsShowSmokeL(false);
			SetIsReadySmokeL(false);
		}
	}
}

void Effect::DrawSmokeS(int _pn)
{
	sprSmokeS->Begin();
	if (_pn == 0)
	{
		for (auto& it : smokeS)
		{
			if (!it.isEnable)continue;
			sprSmokeS->Draw(it.pos.x + GameUI::MULTIPLAY_ONE_ORIJIN_X - 128, it.pos.y + GameUI::MULTI_CORRECTION_Y - 50, 276.0f, 114.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, it.anmFrm);
		}
	}
	else
	{
		for (auto& it : smokeS)
		{
			if (!it.isEnable)continue;
			sprSmokeS->Draw(it.pos.x + GameUI::MULTIPLAY_TWO_ORIJIN_X - 128, it.pos.y + GameUI::MULTI_CORRECTION_Y - 50, 276.0f, 114.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, it.anmFrm);
		}
	}

	sprSmokeS->End();
}

void Effect::DrawSmokeL(int _pn)
{
	sprSmokeL->Begin();

	// smokeL‚Ì‘å‚«‚³’²®—p‚Ì•Ï”
	const float scale = 5.0f;

	if (_pn == 0)
	{
		if (!smokeL.isEnable)return;
		sprSmokeL->Draw(smokeL.pos.x + GameUI::MULTIPLAY_ONE_ORIJIN_X - 128, smokeL.pos.y + GameUI::MULTI_CORRECTION_Y - 50, 276.0f * scale, 114.0f * scale, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, smokeL.anmFrm);
	}
	else
	{
		if (!smokeL.isEnable)return;
		sprSmokeL->Draw(smokeL.pos.x + GameUI::MULTIPLAY_TWO_ORIJIN_X - 128, smokeL.pos.y + GameUI::MULTI_CORRECTION_Y - 50, 276.0f * scale, 114.0f * scale, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, smokeL.anmFrm);
	}

	sprSmokeL->End();

}

void Effect::GenerateSmoke(int _row, int _column)
{

	for (auto& it : smokeS)
	{
		if (it.isEnable)continue;

		it.pos = DirectX::XMFLOAT2(114.0f * _row + 57.0f, 114.0f * _column + 57.0f);
		it.anmCnt = 0;
		it.anmFrm = 0;
		it.isEnable = true;
		break;
	}

	SetIsShowSmoke(true);
}

void Effect::GenerateSmokeL(int _pn)
{
	if (_pn == 0)
	{
		smokeL.pos = DirectX::XMFLOAT2(0, 0);
	}
	else
	{
		smokeL.pos = DirectX::XMFLOAT2(0, 0); //‰¼‚Å‚¸‚ç‚µ‚Ä‚é
	}
	smokeL.anmCnt = 0;
	smokeL.anmFrm = 0;
	smokeL.isEnable = true;

	SetIsShowSmokeL(true);
}


