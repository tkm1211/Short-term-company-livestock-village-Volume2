#include "UI.h"

#include "SceneManager.h"
#include "BlockManager.h"
#include "Resource.h"
#include "easing.h"
#include "ConvenientMath.h"
#include "Constant.h"
#include "Player.h"

/*--------------------------------------*/
//	Global area
/*--------------------------------------*/

GameUI provisionalGameUI;

/*--------------------------------------*/
//	Menber function
/*--------------------------------------*/

void GameUI::Init()
{
	// �摜�f�[�^�̃��[�h
	{
		sprGameUI = RESOURCE->GetSpriteData(Resource::Texture::UI);
		int charNum = 0;// TODO:���ϐ�
		switch (charNum)
		{
		case 0:
			sprCharacter = RESOURCE->GetSpriteData(Resource::Texture::Character01);
			break;
		case 1:
			sprCharacter = RESOURCE->GetSpriteData(Resource::Texture::Character02);
			break;
		case 2:
			sprCharacter = RESOURCE->GetSpriteData(Resource::Texture::Character03);
			break;
		case 3:
			sprCharacter = RESOURCE->GetSpriteData(Resource::Texture::Character04);
			break;
		default:
			break;
		}
	}

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

	// Speed�֌W
	{
		minTime = 0;
		secondTime = 0;
		totalTime = 0;
		speedUpCount = 0;
	}

	// Character�֌W
	{
		charTex.pos = DirectX::XMFLOAT2(150.0f, 327.0f);
		charTex.tex = DirectX::XMFLOAT2(0.0f, 0.0f);
		charTex.size = DirectX::XMFLOAT2(320.f, 416.0f);
		
		charAnimCount = 0;
		charAnimFrame = 0;
	}

	// Ready�֌W
	{
		readyState = 0;
		readyTimer = 0;
		readyAlpha = 0.0f;
		readyPosY = 462.0f;
	}
}

void GameUI::Uninit()
{

}

void GameUI::Update()
{
	/*�Q�[���J�n�O�ōs������*/
	
	// ������\������
	UpdateOfGameReady();

	// �L�����A�j���[�V�����̍X�V
	UpdateOfCharacter();


	if (sceneSingleGame.GetIsGameReady())return;

	// �A�����̕\��
	ShowChainNumUpdate();

	// NextBlockColors�ɒl���Z�b�g����
	SetNextBlockColors();

	// �Q�[�W�̍X�V
	UpdateOfGauge();

	// �Q�[�W�̃X�s�[�h�X�V
	UpdateGaugeSpeed();

	// Time�̍X�V
	UpdateOfTime();

}

void GameUI::Draw()
{
	using namespace DirectX;

	//basic val of drawing UI.
	XMFLOAT2 basicSize(36, 60);
	XMFLOAT2 basicTex(0, 696);
	XMFLOAT2 basicCenter(0.0f, 0.0f);
	XMFLOAT4 basicColor(1.0f, 1.0f, 1.0f, 1.0f);


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

	// ReadyGo
	{
		if (sceneSingleGame.GetIsGameReady())
		{
			if (provisionalGameUI.readyState < 1)
			{
				sprGameUI->Draw(660.0f, provisionalGameUI.readyPosY, 576, 150, 0, 3364, 576, 150, 0, 0, 0.0f, 1.0f, 1.0f, 1.0f, provisionalGameUI.readyAlpha);
			}
			else
			{
				sprGameUI->Draw(660.0f, provisionalGameUI.readyPosY, 576, 158, 0, 3364 + 158, 576, 158, 0, 0, 0.0, 1.0f, 1.0f, 1.0f, provisionalGameUI.readyAlpha);
			}
		}

	}

	sprGameUI->End();


	// �L�����N�^�[�̕`��
	{
		sprCharacter->Begin();

		sprCharacter->Draw(
			charTex.pos, charTex.size,
			charTex.tex, charTex.size,
			XMFLOAT2(0.0f, 0.0f), 0.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
		);

		sprCharacter->End();
	}
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


/*------------------------------------------*/
// �X�s�[�h�̍X�V�֐�
/*------------------------------------------*/
void GameUI::UpdateGaugeSpeed()
{
	if (speedUpCount == 0) // ����̃X�s�[�h�A�b�v�̂�45�b�ŏグ��
	{
		if (secondTime >= 45 && secondTime != 0)
		{
			maxSecond -= 0.5f;
			speedUpCount++;
			secondTime = 0;

		}
	}
	else
	{
		if (secondTime >= 30 && secondTime != 0)
		{
			maxSecond -= 0.5f;
			speedUpCount++;
			secondTime = 0;
		}

		if (maxSecond <= 1.5f)
		{
			maxSecond = 1.5f;
		}
	}
}


/*------------------------------------------*/
// Time�̍X�V�֐�
/*------------------------------------------*/
void GameUI::UpdateOfTime()
{
	if (++totalTime >= 60)
	{
		secondTime++;
		totalTime = 0;
	}
	if (secondTime >= 60)
	{
		minTime++;
		secondTime = 0;
	}
}


/*------------------------------------------*/
// �L�����N�^�[�A�j���[�V�����̍X�V�֐�
/*------------------------------------------*/
void GameUI::UpdateOfCharacter()
{
	// ��Ԃ̎����L�����̕ω�
	bool proGameOver = false;//TODO : ���ϐ�
	bool proLose = false;
	bool proAleat = false;


	//charTex.tex.y = 0.0f;

	// �A�j���[�V�����̎�ޕύX
	{
		if (proGameOver)// ��l�v���C�I�� & ��l�v���C����
		{
			if (charTex.tex.y != charTex.tex.y)
			{
				charAnimFrame = 0;
			}
			charTex.tex.y = 416.0f * 3.0f;
		}
		else if (proLose)// ��l�v���C�s�k
		{
			if (charTex.tex.y != charTex.tex.y)
			{
				charAnimFrame = 0;
			}
			charTex.tex.y = 416.0f * 4.0f;
		}
		else if (proAleat)// �A���[�g��
		{
			if (charTex.tex.y != charTex.tex.y)
			{
				charAnimFrame = 0;
			}
			charTex.tex.y = 416.0f * 2.0f;
		}
		else if (BlockManager::State::Break == provisionalBlockManager.GetStatus() ||
			BlockManager::State::Chain == provisionalBlockManager.GetStatus()) // �`�F�C����
		{
			if (charTex.tex.y != charTex.tex.y)
			{
				charAnimFrame = 0;
			}
			charTex.tex.y = 416.0f * 1.0f;
		}
		else // ����ȊO
		{
			if (charTex.tex.y != charTex.tex.y)
			{
				charAnimFrame = 0;
			}
			charTex.tex.y = 416.0f * 0.0f;
		}
	}

	// �ʏ펞�̃A�j���[�V����
	if (charTex.tex.y != 0.0f)
	{
		charAnimFrame = 0;
		charTex.tex.x = 0.0f;
	}
	else
	{
		charTex.tex.x = 416.0f * charAnimFrame;
	}

	// �A�j���[�V�������[�v
	if (charAnimCount++ % 5 == 0 && charAnimCount != 0)
	{
		charAnimFrame++;
		if (CHARACTER_ANIMATION_MAX <= charAnimFrame)
		{
			charAnimFrame = 0;
		}
	}
}


/*------------------------------------------*/
// GameReady�̍X�V�֐�
/*------------------------------------------*/
void GameUI::UpdateOfGameReady()
{
	if (!sceneSingleGame.GetIsGameReady()) return;

	switch (readyState)
	{
	case 0:
		++readyTimer;
		if (readyTimer == 50)
		{
			// play sound.
		}
		if (readyTimer <= 60 && readyTimer >= 50)
		{
			readyAlpha += 0.1f;
		}
		if (readyTimer >= 50)
		{
			readyPosY = easing::OutExp(static_cast<float>(readyTimer), 40.0f, 462.0f, 462.0f + 114.0f);
		}
		if (readyTimer >= 180)
		{
			readyState++;
			readyTimer = 0;
			readyAlpha = 0.0f;
		}
		break;

	case 1:
		++readyTimer;
		if (readyTimer == 1)
		{
			// play sound.
		}

		if (++readyTimer <= 60)
		{
			readyAlpha += 0.1f;
		}
		if (readyTimer >= 50)
		{
			readyPosY = easing::OutExp(static_cast<float>(readyTimer), 40.0f, 462.0f, 462.0f + 114.0f);
		}
		if (readyTimer >= 120)
		{
			readyState++;
			readyTimer = 0;
			readyAlpha = 0.0f;
			sceneSingleGame.SetIsGameReady(false);
		}
		break;
	}
}
