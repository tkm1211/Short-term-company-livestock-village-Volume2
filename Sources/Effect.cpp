#include "Blender.h"

#include "SceneManager.h"
#include "Effect.h"
#include "BlockManager.h"
#include "easing.h"
#include "Block.h"
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
	sprDeadLine = std::make_unique<SpriteBatch>(L"Data/Image/Effect/effect.png", 1, DirectX::XMFLOAT2(684, 1032));
	sprParticle = std::make_unique<SpriteBatch>(L"Data/Image/Effect/particle.png", 200, DirectX::XMFLOAT2(24, 24));
	sprSmokeS = std::make_unique<SpriteBatch>(L"Data/Image/Effect/SmokeS.png", 10, DirectX::XMFLOAT2(276, 114));
	sprSmokeL = std::make_unique<SpriteBatch>(L"Data/Image/Effect/SmokeL.png", 10, DirectX::XMFLOAT2(582, 114));
	sprMiniChar = RESOURCE->GetSpriteData(Resource::Texture::MiniCharacter);
	sprMiniBlock = RESOURCE->GetSpriteData(Resource::Texture::MiniBlock);
#if 0
	sprStack = RESOURCE->GetSpriteData(Resource::Texture::StackBlock);
#else
	sprStack = RESOURCE->GetSpriteData(Resource::Texture::StackNum);
#endif
	
	isPlus = true;
	isAleat = false;
	isLastAleat = false;
	isDisappearance = false;

	stackObstacleNum = 0;

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
	isShowMiniBlock = false;
	isShowStackNum = false;
}

void Effect::Uninit(int _pn)
{
}

void Effect::Update(int _pn)
{
	MoveParticle(_pn);
	UpdateOfSmoke();
	UpdateOfSmokeL(_pn);
	UpdateOfMiniChar();
	UpdateOfMiniBlock(_pn);
}

void Effect::Draw(int _pn)
{
	SetDrawBlendMode(BLEND_MODE::ADD);

	// Draw Aleat.
	if (!(sceneSelect.gameMode == SelectGameMode::Single))
	{

	}
	else
	{

	}

	SetDrawBlendMode(BLEND_MODE::ALPHA);

	// Particle
	sprParticle->Begin();

	if (sceneSelect.gameMode == SelectGameMode::Single)
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


	// Draw MiniCharacter
	if (isShowMiniChar)
	{
		DrawMiniChar(_pn);
	}


	// Draw MiniBlock
	if (isShowMiniBlock)
	{
		DrawMiniBlock(_pn);
	}

	// Draw StackObstacle
	//if (isShowStackNum)
	{
		DrawStackObstacle(_pn);
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

	// smokeLの大きさ調整用の変数
	const float scale = 3.0f;

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
		smokeL.pos = DirectX::XMFLOAT2(60, 800);
	}
	else
	{
		smokeL.pos = DirectX::XMFLOAT2(60, 800); //仮でずらしてる
	}
	smokeL.anmCnt = 0;
	smokeL.anmFrm = 0;
	smokeL.isEnable = true;

	SetIsShowSmokeL(true);
}




void Effect::GenerateMiniChar(int _row, int _column, int _pn)
{
	miniChar.pos = DirectX::XMFLOAT2(114.0f * _row + 150.0f, 114.0f * _column - 100.0f);
	miniChar.anmCnt = 0;
	miniChar.anmFrm = 0;
	miniChar.isEnable = true;
	isShowMiniChar = true;
	
	GenerateMiniBlock(_row, _column, _pn);
}

void Effect::UpdateOfMiniChar()
{
	auto End = [&]()
	{
		miniChar.isEnable = false;
		isShowMiniChar = false;
		miniChar.pos = DirectX::XMFLOAT2(0.0f, 0.0f);
		miniChar.anmCnt = 0;
		miniChar.anmFrm = 0;
	};

	if (isShowMiniChar)
	{
		if (miniChar.isEnable)
		{
			if (++miniChar.anmCnt % 5 == 0)
			{
				if (++miniChar.anmFrm > 3)
				{
					miniChar.anmFrm = 3;
				}
				if (miniChar.anmCnt >= 60 && miniChar.anmFrm == 3)
				{
					// end
					End();
				}
			}
		}
	}
}

void Effect::DrawMiniChar(int _pn)
{
	if (!isShowMiniChar)return;

	constexpr float preExRate = 3.0f;

	int charNum = regularGameUI[_pn].GetCharNum();

	sprMiniChar->Begin();
	switch (_pn)
	{
	case 0:
		if (!miniChar.isEnable)break;
		sprMiniChar->Draw(
			miniChar.pos.x + GameUI::MULTIPLAY_ONE_ORIJIN_X, miniChar.pos.y + GameUI::MULTI_CORRECTION_Y, 
			32 * preExRate, 80 * preExRate, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f, charNum * 4 + miniChar.anmFrm
		);
		break;

	case 1:
		if (!miniChar.isEnable)break;
		sprMiniChar->Draw(
			miniChar.pos.x + GameUI::MULTIPLAY_TWO_ORIJIN_X, miniChar.pos.y + GameUI::MULTI_CORRECTION_Y,
			32.0f * preExRate, 80.0f * preExRate, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f, charNum * 4 + miniChar.anmFrm
		);
		break;
	}
	sprMiniChar->End();
}




void Effect::GenerateMiniBlock(int _row, int _column, int _pn)
{
	for (auto& it : miniBlock)
	{
		if (it.isEnable)continue;


		it.pos = DirectX::XMFLOAT2(114.0f * _row + 150.0f, 114.0f * _column - 100.0f);
		it.firstPos = it.pos;
		it.anmCnt = 0;
		it.anmFrm = 0;
		it.flyingCnt = 0;
		it.attackerNum = _pn;
		it.isEnable = true;
		it.nowFlying = false;
		it.nowFalling = false;
		isShowMiniBlock = true;

		return;
	}
}

void Effect::UpdateOfMiniBlock(int _pn)
{
	auto FirstEnd = [&](MiniBlock& i)
	{
		i.nowFlying = false;
		i.nowFalling = true;
		i.anmCnt = 0;
		i.anmFrm = 3;
		i.flyingCnt = 0;

		if (i.attackerNum == 0)
		{
			i.pos.x = 1090.0f;
		}
		else
		{
			i.pos.x = -490.0f;
		}
		i.pos.y = -Block::SIZE_Y;
	};

	auto FinalEnd = [&](MiniBlock& i)
	{
		i.isEnable = false;
		i.nowFlying = false;
		i.nowFalling = false;
		i.pos = DirectX::XMFLOAT2(0.0f, 0.0f);
		i.anmCnt = 0;
		i.anmFrm = 0;
		i.flyingCnt = 0;

		// 描画判定
		int cnt = 0;
		for (auto& it : miniBlock)
		{
			if (it.isEnable)
			{
				cnt++;
				continue;
			}
		}
		if (cnt == 0)
		{
			isShowMiniBlock = false;
			return;
		}
	};


	// 更新処理
	if (isShowMiniBlock)
	{
		for (auto& it : miniBlock)
		{
			if (!it.isEnable)continue;

			if (++it.anmCnt % 5 == 0 && !it.nowFalling)
			{
				if (++it.anmFrm > 3)
				{
					it.anmFrm = 3;
					it.nowFlying = true;
				}
			}
			// 上昇
			if (it.nowFlying)
			{
				if (++it.flyingCnt >= 40)
				{
					//FirstEnd(it);
					FinalEnd(it);
					SetStackObstacleNum(_pn);
				}
				else
				{
#if 0
					it.pos.y += FORCE_OF_MINIBLOCK;
#else
					it.pos.y = easing::OutQuad(it.flyingCnt, 40, -Block::SIZE_Y*2, it.firstPos.y);
#endif
				}
			}
			// お邪魔置きに落とす
			if (it.nowFalling)
			{
				if (++it.flyingCnt >= 25)
				{
					FinalEnd(it);
					SetStackObstacleNum(_pn);
				}
				else
				{
					it.pos.y += -FORCE_OF_MINIBLOCK;
				}
			}
		}
	}
}

void Effect::DrawMiniBlock(int _pn)
{
	if (!isShowMiniBlock)return;

	constexpr float preExRate = 3.0f;

	sprMiniBlock->Begin();
	for (auto& it : miniBlock)
	{
		switch (_pn)
		{
		case 0:
			if (!it.isEnable)break;
			sprMiniBlock->Draw(
				it.pos.x + GameUI::MULTIPLAY_ONE_ORIJIN_X, it.pos.y + GameUI::MULTI_CORRECTION_Y,
				32.0f * preExRate, 80.0f * preExRate, 0.0f,
				1.0f, 1.0f, 1.0f, 1.0f, it.anmFrm
			);
			break;

		case 1:
			if (!it.isEnable)break;
			sprMiniBlock->Draw(
				it.pos.x + GameUI::MULTIPLAY_TWO_ORIJIN_X, it.pos.y + GameUI::MULTI_CORRECTION_Y,
				32.0f * preExRate, 80.0f * preExRate, 0.0f,
				1.0f, 1.0f, 1.0f, 1.0f, it.anmFrm
			);
			break;
		}
	}
	sprMiniBlock->End();
}

#if 0
void Effect::DrawStackObstacle(int _pn)
{
	struct StackBlock
	{
		bool isEnable = false;
		int blockType = 0;	// 1 is 5 stacks, 0 is 1 stacks.
	};


	constexpr int ROTATE_MAX = 100;
	std::array<StackBlock, ROTATE_MAX> mini;

	int stackNum = stackObstacleNum;
	int perFive = stackNum / 5;
	int remainder = stackNum - 5*perFive;

	for (int i = 0; i < perFive; i++)
	{
		mini[i].isEnable = true;
		mini[i].blockType = 0;
	}

	int cnt = 0;
	for (int i = 0; i < ROTATE_MAX; i++)
	{
		if (cnt >= remainder)break;
		if (mini[i].isEnable)continue;


		mini[i].isEnable = true;
		mini[i].blockType = 1;
		cnt++;
	}


	sprStack->Begin();

	for (int i = 0; i < ROTATE_MAX; i++)
	{
		if (!mini[i].isEnable)continue;
		switch (_pn)
		{
		case 0:
			sprStack->Draw(
				GameUI::MULTIPLAY_TWO_ORIJIN_X - 60.0f, -40.0f * i +300 + GameUI::MULTI_CORRECTION_Y,
				36.0f, 36.0f, 0.0f,
				1.0f, 1.0f, 1.0f, 1.0f, mini[i].blockType
			);
			break;
		case 1:
			sprStack->Draw(
				GameUI::MULTIPLAY_ONE_ORIJIN_X, 36.0f * i +200 +  GameUI::MULTI_CORRECTION_Y,
				198.0f, 198.0f, 0.0f,
				1.0f, 1.0f, 1.0f, 1.0f, mini[i].blockType
			);

			break;
		}
	}

	sprStack->End();
}
#else
void Effect::DrawStackObstacle(int _pn)
{
	int stackNum = stackObstacleNum;

	int ten = stackNum / 10;
	int one = stackNum % 10;

	TexData data;
	{
		if (_pn == 0) {
			data.pos = DirectX::XMFLOAT2(750.0f, 30.0f);
		}
		else {
			data.pos = DirectX::XMFLOAT2(1030.0f, 30.0f);
		}
		data.size = DirectX::XMFLOAT2(60.0f, 42.0f);
		data.tex = DirectX::XMFLOAT2(0.0f, 60.f);
	}
	
	sprStack->Begin();

	// ブロック×の部分
	sprStack->Draw(data);
	
	if (_pn == 1)
	{
		sprStack->Draw(820.0f, 30.0f, 36.0f, 60.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, ten);
		sprStack->Draw(850.0f, 30.0f, 36.0f, 60.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, one);
	}
	else
	{
		sprStack->Draw(1100.0f, 30.0f, 36.0f, 60.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, ten);
		sprStack->Draw(1130.0f, 30.0f, 36.0f, 60.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, one);
	}

	sprStack->End();
}
#endif

void Effect::SetStackObstacleNum(int _pn)
{
	if (_pn == 0)
	{
		stackObstacleNum = regularBlockManager[1].GetFallObstacleNum();
	}
	else
	{
		stackObstacleNum = regularBlockManager[0].GetFallObstacleNum();
	}

	isShowStackNum = true;
}

void Effect::UseImGui()
{
	//ImGui::Begin("stackObstacleNum");
	//ImGui::DragInt("p", &regularEffects[0].stackObstacleNum);
	//ImGui::End();

	//ImGui::Begin("2");
	//ImGui::DragInt("q", &regularEffects[1].stackObstacleNum);
	//ImGui::End();

}


