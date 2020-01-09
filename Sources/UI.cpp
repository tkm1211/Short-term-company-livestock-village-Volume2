#include "UI.h"

#include "BlockManager.h"
#include "Resource.h"
#include "easing.h"
#include "ConvenientMath.h"
#include "Constant.h"

/*--------------------------------------*/
//	Global area
/*--------------------------------------*/

GameUI provisionalGameUI;

/*--------------------------------------*/
//	Menber function
/*--------------------------------------*/

void GameUI::Init()
{
	sprGameUI = RESOURCE->GetSpriteData(Resource::Texture::UI);

	// �Z���N�g���x���ɉ����Ēl��ς���
	{
		//TODO:���Œu��
		maxSecond = scast_f(6.5);
	}

	// ���ɏo�Ă���u���b�N�̐F
	{
		ConvMath::InitializeArray(&nextBlockColors[0], scast_i(nextBlockColors.size()), -1);
		SetNextBlockColors();

		for (int i = 0; i < BlockManager::ROW_MAX; i++)
		{
			using namespace DirectX;
			auto& it = drawBlockStatus[i];
			it.pos = XMFLOAT2(3.0f + 615.0f + 114.0f * i, 12.0f + 1032.0f);
			it.size = XMFLOAT2(114.0f, 228.0f);
		}
		for (int i = 0; i < provisionalBlockManager.GetColorMax(); i++)
		{
			drawBlockStatus[i].tex = DirectX::XMFLOAT2(114.0f * (i + 1), 228.0f);
		}
	}

	// ���[�^�[
	{
		meterBack.pos = DirectX::XMFLOAT2(1302.0f + ADJUST, 510.0f);
		meterBack.tex = DirectX::XMFLOAT2(36.0f, 936.0f);
		meterBack.size = DirectX::XMFLOAT2(36.0f, 570.0f);

		meter.pos = DirectX::XMFLOAT2(1302.0f + ADJUST, 510.0f);
		meter.tex = DirectX::XMFLOAT2(0.0f, 936.0f);
		meter.size = DirectX::XMFLOAT2(36.0f, 570.0f);

		gaugeTexPosY = meter.tex.y + (maxSecond - second) / maxSecond * meter.size.y;
		if (gaugeTexPosY < 951.0f)
		{
			gaugeTexPosY = 936.0f + 570.0f;
			//if (_playerNum == 1) meter.tex.x = 36.0f;
			meter.tex.y = 936.0f + 570.0f;
		}
		else if (maxSecond <= second && gaugeTexPosY <= 936.0f + 570.0f)
		{
			gaugeTexPosY = 936.0f + 570.0f;
		}
		gaugeTexSizeY = meter.size.y - (gaugeTexPosY - meter.tex.y);

		gaugePosY = meter.pos.y + (meter.size.y - gaugeTexSizeY);
	}

	// �`�F�C���\���n
	{
		showChainNumPos = DirectX::XMFLOAT2(0.0f, 0.0f);
		showChainNumFirstPos = 0.0f;
		showChainAlpha = 0.0f;
		showChainTimer = 0;
		nowChainNum = 0;
		isShowChainNum = false;
	}

	// �X�R�A�{�[�h�n
	{
		totalScore = 0;
	}
}

void GameUI::Uninit()
{

}

void GameUI::Update()
{
	ShowChainNumUpdate();
	SetNextBlockColors();
	UpdateOfGauge();
}

void GameUI::Draw()
{
	DirectX::XMFLOAT2 basicSize(36, 60);
	DirectX::XMFLOAT2 basicTex(0, 696);
	DirectX::XMFLOAT2 basicCenter(0.0f, 0.0f);
	DirectX::XMFLOAT4 basicColor(1.0f, 1.0f, 1.0f, 1.0f);


	sprGameUI->Begin();

	// ���[�^�[
	{
		sprGameUI->Draw(meterBack.pos, meterBack.size, meterBack.tex, meterBack.size,
			DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		sprGameUI->Draw(DirectX::XMFLOAT2(meter.pos.x, gaugePosY), DirectX::XMFLOAT2(meter.size.x, gaugeTexSizeY),
			DirectX::XMFLOAT2(meter.tex.x, gaugeTexPosY), DirectX::XMFLOAT2(meter.size.x, gaugeTexSizeY),
			DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// ���ɏo�Ă���u���b�N
	{
		for (int i = 0; i < scast_i(nextBlockColors.size()); i++)
		{
			using namespace DirectX;
			auto it = drawBlockStatus[i];
			sprGameUI->Draw(
				XMFLOAT2(it.pos.x, it.pos.y), it.size,
				drawBlockStatus[nextBlockColors[i]].tex, it.size,
				XMFLOAT2(0.0f, 0.0f), 0.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}	

	// �X�R�A
	{
		DirectX::XMFLOAT2 scorePos = DirectX::XMFLOAT2(1463.0f, 879.0f);
		TexData scoreBoard{};
		scoreBoard.pos = DirectX::XMFLOAT2(1416.0f, 774.0f);
		scoreBoard.tex = DirectX::XMFLOAT2(0.0f, 312.0f);
		scoreBoard.size = DirectX::XMFLOAT2(312.0f, 192.0f);


		int hundredThousand = totalScore % 1000000 / 100000;
		int tenThousand = totalScore % 100000 / 10000;
		int thousand = totalScore % 10000 / 1000;
		int hundred = totalScore % 1000 / 100;
		int ten = totalScore % 100 / 10;
		int one = totalScore % 10 / 1;

		// Board
		sprGameUI->Draw(scoreBoard.pos, scoreBoard.size, scoreBoard.tex, scoreBoard.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, basicColor);

		// �\��
		sprGameUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 0, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(basicTex.x + 36 * hundredThousand/*�����Ɍ������*/, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
		// ��
		sprGameUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 1, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(basicTex.x + 36 * tenThousand/*�����Ɍ������*/, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
		// ��
		sprGameUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 2, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(basicTex.x + 36 * thousand/*�����Ɍ������*/, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
		// �S
		sprGameUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 3, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(basicTex.x + 36 * hundred/*�����Ɍ������*/, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
		// �\
		sprGameUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 4, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(basicTex.x + 36 * ten/*�����Ɍ������*/, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
		// ��
		sprGameUI->Draw(DirectX::XMFLOAT2(scorePos.x + 36 * 5, scorePos.y), DirectX::XMFLOAT2(36, 60), DirectX::XMFLOAT2(basicTex.x + 36 * one/*�����Ɍ������*/, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
	}

	// �`�F�C����
	{
		if (isShowChainNum)
		{
			//nowChainNum = 10;// TODO : test�p�̉��ϐ�
			basicTex = DirectX::XMFLOAT2(0, 816);
			basicColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, showChainAlpha);

			if (nowChainNum < 10)
			{
				// �R���{�����񌅖���
				// ��
				sprGameUI->Draw(DirectX::XMFLOAT2(showChainNumPos.x + ADJUST + SINGLE_CORRECTION_X, showChainNumPos.y), DirectX::XMFLOAT2(basicSize.x * 2, basicSize.y * 2), DirectX::XMFLOAT2(basicTex.x + 36 * nowChainNum, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
				// �A�I
				sprGameUI->Draw(DirectX::XMFLOAT2(showChainNumPos.x + 81 + ADJUST + SINGLE_CORRECTION_X, showChainNumPos.y + 51), DirectX::XMFLOAT2(66, 60), DirectX::XMFLOAT2(basicTex.x, basicTex.y + 60), DirectX::XMFLOAT2(66, 60), basicCenter, 0.0f, basicColor);//2
			}
			else
			{
				// �R���{���񌅈ȏ�
				int ten = nowChainNum % 100 / 10;
				int one = nowChainNum % 10 / 1;

				// �\
				sprGameUI->Draw(DirectX::XMFLOAT2(showChainNumPos.x + ADJUST + SINGLE_CORRECTION_X, showChainNumPos.y), DirectX::XMFLOAT2(basicSize.x * 2, basicSize.y * 2), DirectX::XMFLOAT2(basicTex.x + 36 * ten, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);//1
				// ��
				sprGameUI->Draw(DirectX::XMFLOAT2(showChainNumPos.x + 81 + ADJUST + SINGLE_CORRECTION_X, showChainNumPos.y), DirectX::XMFLOAT2(basicSize.x * 2, basicSize.y * 2), DirectX::XMFLOAT2(basicTex.x + 36 * one, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);//2
				//�A�I
				sprGameUI->Draw(DirectX::XMFLOAT2(showChainNumPos.x + 162 + ADJUST + SINGLE_CORRECTION_X, showChainNumPos.y + 51), DirectX::XMFLOAT2(66, 60), DirectX::XMFLOAT2(basicTex.x, basicTex.y + 60), DirectX::XMFLOAT2(66, 60), basicCenter, 0.0f, basicColor);

			}
		}
	}

	sprGameUI->End();
}



/*------------------------------------------*/
// �A���̎n�߂ɒʂ�Initialize����
/*------------------------------------------*/
void GameUI::SetShowChainNumInit(DirectX::XMFLOAT2 _pos)
{
	showChainNumPos = _pos;
	showChainNumPos.y += 57;
	showChainTimer = 0;
	showChainAlpha = 0.0f;
	showChainNumFirstPos = showChainNumPos.y;

	isShowChainNum = true;
}

/*------------------------------------------*/
// �A�����ɍX�V���鏈��
/*------------------------------------------*/
void GameUI::ShowChainNumUpdate()
{
	if (!isShowChainNum) return;

	if (showChainTimer < 60)
	{
		if (showChainTimer <= 10)
		{
			showChainAlpha += 0.1f;
		}
		showChainNumPos.y = easing::OutExp(static_cast<float>(showChainTimer), 60.0f, showChainNumFirstPos - 57.0f, showChainNumFirstPos);
		++showChainTimer;
	}
	else
	{
		showChainAlpha -= 0.1f;
		if (showChainAlpha <= 0.0f)
		{
			showChainAlpha = 0.0f;
		}
		++showChainTimer;
	}

	if (showChainTimer >= 120)
	{
		showChainTimer = 0;
		isShowChainNum = false;
	}
}

/*------------------------------------------*/
// �X�R�A�v�Z�֐�
/*------------------------------------------*/
void GameUI::CalcScore(int _eraseNum, int _chainCount)
{
	float nowPlusScore{};

	// 6 �R���{�ȏ�͊�b�_���ς��Ȃ�����
	if (_chainCount >= 5)
	{
		_chainCount = 5;
	}

	if (_eraseNum - 2 == 0)
	{
		nowPlusScore = static_cast<float>(baseScore_tbl[_chainCount]);
	}
	else
	{
		nowPlusScore = baseScore_tbl[_chainCount] * (_eraseNum - 2) * 1.5f;
	}

	totalScore += nowPlusScore;
}

/*------------------------------------------*/
// ���ɗ���u���b�N�̐F���Z�b�g����֐�
/*------------------------------------------*/
void GameUI::SetNextBlockColors()
{
	for (int i = 0; i < BlockManager::BOARD_ROW_MAX; i++)
	{
		Block* ans = nullptr;
		provisionalBlockManager.SearchBlock(i, 9, &ans);
		if (ans != nullptr)
		{
			nextBlockColors[i] = ans->GetColor();
		}
	}
}

/*------------------------------------------*/
// �Q�[�W�����Z�b�g����֐�
/*------------------------------------------*/
void GameUI::ResetGauge()
{
	isTimerStop = false;
	isGaugeMax = false;

	meter.tex.y = 936.0f;
}

/*------------------------------------------*/
// �Q�[�W�̍X�V�֐�
/*------------------------------------------*/
void GameUI::UpdateOfGauge()
{
	if (!isTimerStop && !provisionalBlockManager.GetIsPushing())
	{
		second = interpolationTimeCnt / 60.0f;

		if (maxSecond + 1 < second)
		{
			isTimerStop = true;
			isGaugeMax = true;
			second = 0;
			gaugeDownCnt = 0;
			interpolationTimeCnt = -1;
			provisionalBlockManager.SetIsPushUpByGauge(true);
		}
		interpolationTimeCnt++;
	}

	if (isGaugeMax)
	{
		if (GAUGE_DOWN_CNT_MAX <= gaugeDownCnt)
		{
			second = 0;
			ResetGauge();
		}
		else
		{
			second = easing::OutExp(scast_f(gaugeDownCnt), scast_f(GAUGE_DOWN_CNT_MAX), 0.0f, maxSecond);
		}
		gaugeDownCnt++;
	}

	gaugeTexPosY = meter.tex.y + (maxSecond - second) / maxSecond * meter.size.y;
	if (gaugeTexPosY < 951.0f)
	{
		gaugeTexPosY = 936.0f + 570.0f;
		//if (_playerNum == 1) meter.tex.x = 36.0f;
		meter.tex.y = 936.0f + 570.0f;
	}
	else if (maxSecond <= second && gaugeTexPosY <= 936.0f + 570.0f)
	{
		gaugeTexPosY = 936.0f + 570.0f;
	}
	gaugeTexSizeY = meter.size.y - (gaugeTexPosY - meter.tex.y);

	gaugePosY = meter.pos.y + (meter.size.y - gaugeTexSizeY);

}
