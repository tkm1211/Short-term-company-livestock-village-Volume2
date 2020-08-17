#include "UI.h"

#include <Blender.h>
#include "SceneManager.h"
#include "BlockManager.h"
#include "Resource.h"
#include "easing.h"
#include "ConvenientMath.h"
#include "Constant.h"
#include "Player.h"
#include "Sound.h"

/*--------------------------------------*/
//	Global area
/*--------------------------------------*/

GameUI provisionalGameUI;
std::array<GameUI, 2> regularGameUI;

/*--------------------------------------*/
//	Menber function
/*--------------------------------------*/

void GameUI::Init(int _pn)
{
	// �摜�f�[�^�̃��[�h
	{
		sprGameUI = RESOURCE->GetSpriteData(Resource::Texture::UI);
		//int charNum = _pn;// TODO:���ϐ�
		int charNum = sceneSelect.GetCharacterNumber(_pn);
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
		case 4:
			sprCharacter = RESOURCE->GetSpriteData(Resource::Texture::Character05);
			break;
		case 5:
			sprCharacter = RESOURCE->GetSpriteData(Resource::Texture::Character06);
			break;
		case 6:
			sprCharacter = RESOURCE->GetSpriteData(Resource::Texture::Character07);
			break;
		default:
			break;
		}
	}

	// ���݂̃Q�[�����[�h���擾
	auto nowGameMode = sceneSelect.gameMode;

	// �Z���N�g���x���ɉ����Ēl��ς���
	{
		//TODO:���Œu��
		maxSecond = scast_f(6.5);

	}

	// ���ɏo�Ă���u���b�N�̐F
	{
		ConvMath::InitializeArray(&nextBlockColors[_pn], scast_i(nextBlockColors.size()), -1);
		SetNextBlockColors(_pn);

		for (int i = 0; i < BlockManager::ROW_MAX; i++)
		{
			using namespace DirectX;
			auto& it = drawBlockStatus[i];

			if (nowGameMode == SelectGameMode::Single)
			{
				it.pos = XMFLOAT2(3.0f + 615.0f + 114.0f * i, 12.0f + 1032.0f);
			}
			else if (nowGameMode == SelectGameMode::Multi || nowGameMode == SelectGameMode::CPU)
			{
				switch (_pn)
				{
				case 0:
					it.pos = XMFLOAT2(3.0f + MULTIPLAY_ONE_ORIJIN_X + 114.0f * i, 12.0f + 1032.0f);
					break;
				case 1:
					it.pos = XMFLOAT2(3.0f + MULTIPLAY_TWO_ORIJIN_X + 114.0f * i, 12.0f + 1032.0f);
					break;
				}
			}
			it.size = XMFLOAT2(114.0f, 228.0f);
		}

		for (int i = 0; i < regularBlockManager[_pn].GetColorMax(); i++)
		{
			drawBlockStatus[i].tex = DirectX::XMFLOAT2(114.0f * (i + 1), 228.0f);
		}
	}

	// ���[�^�[
	{
		interpolationTimeCnt = 0;
		second = 0.0f;
		gaugeDownCnt = 0;

		isTimerStop = false;
		isGaugeMax = false;

		if (nowGameMode == SelectGameMode::Single)
		{
			meterBack.pos = DirectX::XMFLOAT2(1302.0f + ADJUST, 510.0f);
			meterBack.tex = DirectX::XMFLOAT2(36.0f, 936.0f);
			meterBack.size = DirectX::XMFLOAT2(36.0f, 570.0f);

			meter.pos = DirectX::XMFLOAT2(1302.0f + ADJUST, 510.0f);
			meter.tex = DirectX::XMFLOAT2(0.0f, 936.0f);
			meter.size = DirectX::XMFLOAT2(36.0f, 570.0f);
		}
		else if (nowGameMode == SelectGameMode::Multi || nowGameMode == SelectGameMode::CPU)
		{
			switch (_pn)
			{
			case 0:
				meterBack.pos = DirectX::XMFLOAT2(711.0f - ADJUST * 2.0f, 510.0f);
				meterBack.tex = DirectX::XMFLOAT2(36.0f, 936.0f);
				meterBack.size = DirectX::XMFLOAT2(36.0f, 570.0f);

				meter.pos = DirectX::XMFLOAT2(711.0f - ADJUST * 2.0f, 510.0f);
				meter.tex = DirectX::XMFLOAT2(0.0f, 936.0f);
				meter.size = DirectX::XMFLOAT2(36.0f, 570.0f);
				break;

			case 1:
				meterBack.pos = DirectX::XMFLOAT2(1173.0f + ADJUST * 2.0f, 510.0f);
				meterBack.tex = DirectX::XMFLOAT2(36.0f, 936.0f);
				meterBack.size = DirectX::XMFLOAT2(36.0f, 570.0f);

				meter.pos = DirectX::XMFLOAT2(1173.0f + ADJUST * 2.0f, 510.0f);
				meter.tex = DirectX::XMFLOAT2(0.0f, 936.0f);
				meter.size = DirectX::XMFLOAT2(36.0f, 570.0f);
				break;
			}
		}
		gaugeTexPosY = meter.tex.y + (maxSecond - second) / maxSecond * meter.size.y;
		if (gaugeTexPosY < 951.0f)
		{
			gaugeTexPosY = 936.0f + 570.0f;
			if (sceneSelect.gameMode == SelectGameMode::Multi && _pn == 1) meter.tex.x = 36.0f;
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
		if (nowGameMode == SelectGameMode::Single)
		{
			scoreBoard.pos = DirectX::XMFLOAT2(1416.0f, 774.0f);
			scoreBoard.tex = DirectX::XMFLOAT2(0.0f, 312.0f);
			scoreBoard.size = DirectX::XMFLOAT2(312.0f, 192.0f);

			scorePos = DirectX::XMFLOAT2(1463.0f, 879.0f);
		}
		else if (nowGameMode == SelectGameMode::Multi || nowGameMode == SelectGameMode::CPU)
		{
			switch (_pn)
			{
			case 0:
				scoreBoard.pos = DirectX::XMFLOAT2(804.0f, 744.0f);
				scoreBoard.tex = DirectX::XMFLOAT2(0.0f, 312.0f);
				scoreBoard.size = DirectX::XMFLOAT2(312.0f, 192.0f);

				scorePos = DirectX::XMFLOAT2(852.0f, 849.0f);
				break;

			case 1:
				scoreBoard.pos = DirectX::XMFLOAT2(804.0f, 215.0f);
				scoreBoard.tex = DirectX::XMFLOAT2(0.0f, 312.0f + 192.0f);
				scoreBoard.size = DirectX::XMFLOAT2(312.0f, 192.0f);

				scorePos = DirectX::XMFLOAT2(852.0f, 319.0f);
				break;
			}
		}

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
		if (nowGameMode == SelectGameMode::Single)
		{
			charTex.pos = DirectX::XMFLOAT2(150.0f, 327.0f);
			charTex.tex = DirectX::XMFLOAT2(0.0f, 0.0f);
			charTex.size = DirectX::XMFLOAT2(320.f, 416.0f);
		}
		else if (nowGameMode == SelectGameMode::Multi || nowGameMode == SelectGameMode::CPU)
		{
			switch (_pn)
			{
			case 0:
				charTex.pos = DirectX::XMFLOAT2(780.0f, 609.0f);
				charTex.tex = DirectX::XMFLOAT2(0.0f, 0.0f);
				charTex.size = DirectX::XMFLOAT2(160.0f, 208.0f);
				break;

			case 1:
				charTex.pos = DirectX::XMFLOAT2(1150.0f, 81.0f);
				charTex.tex = DirectX::XMFLOAT2(0.0f, 0.0f);
				charTex.size = DirectX::XMFLOAT2(-160.0f, 208.0f);
				break;
			}
		}
		
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

void GameUI::Update(int _pn)
{
	/*�Q�[���J�n�O�ōs������*/
	
	// ������\������
	if (_pn == 0)
	{
		UpdateOfGameReady();
	}

	// �L�����A�j���[�V�����̍X�V
	UpdateOfCharacter(_pn);


	if (sceneSingleGame.GetIsGameReady())return;
	if (sceneMultiGame.GetIsGameReady())return;
	if (sceneCPUGame.GetIsGameReady())return;

	// �A�����̕\��
	ShowChainNumUpdate();

	// NextBlockColors�ɒl���Z�b�g����
	SetNextBlockColors(_pn);

	// �Q�[�W�̍X�V
	UpdateOfGauge(_pn);

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
			if (regularGameUI[0].readyState < 1)
			{
				sprGameUI->Draw(660.0f, regularGameUI[0].readyPosY, 576, 150, 0, 3364, 576, 150, 0, 0, 0.0f, 1.0f, 1.0f, 1.0f, regularGameUI[0].readyAlpha);
			}
			else
			{
				sprGameUI->Draw(660.0f, regularGameUI[0].readyPosY, 576, 158, 0, 3364 + 158, 576, 158, 0, 0, 0.0, 1.0f, 1.0f, 1.0f, regularGameUI[0].readyAlpha);
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

void GameUI::DrawOfSingle()
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
			if (regularGameUI[0].readyState < 1)
			{
				sprGameUI->Draw(660.0f, regularGameUI[0].readyPosY, 576, 150, 0, 3364, 576, 150, 0, 0, 0.0f, 1.0f, 1.0f, 1.0f, regularGameUI[0].readyAlpha);
			}
			else
			{
				sprGameUI->Draw(660.0f, regularGameUI[0].readyPosY, 576, 158, 0, 3364 + 158, 576, 158, 0, 0, 0.0, 1.0f, 1.0f, 1.0f, regularGameUI[0].readyAlpha);
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

void GameUI::DrawOfMulti(int _pn)
{
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
		using namespace DirectX;
		//basic val of drawing UI.
		XMFLOAT2 basicSize(36, 60);
		XMFLOAT2 basicTex(0, 696);
		XMFLOAT2 basicCenter(0.0f, 0.0f);
		XMFLOAT4 basicColor(1.0f, 1.0f, 1.0f, 1.0f);
		int hundredThousand = totalScore % 1000000 / 100000;
		int tenThousand = totalScore % 100000 / 10000;
		int thousand = totalScore % 10000 / 1000;
		int hundred = totalScore % 1000 / 100;
		int ten = totalScore % 100 / 10;
		int one = totalScore % 10 / 1;

		// Board
		sprGameUI->Draw(scoreBoard.pos, scoreBoard.size, scoreBoard.tex, scoreBoard.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, basicColor);

		switch (_pn)
		{
		case 0:
			// �\��
			sprGameUI->Draw(XMFLOAT2(scorePos.x + 36 * 0, scorePos.y), XMFLOAT2(36, 60), XMFLOAT2(basicTex.x + 36 * hundredThousand/*�����Ɍ������*/, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
			// ��
			sprGameUI->Draw(XMFLOAT2(scorePos.x + 36 * 1, scorePos.y), XMFLOAT2(36, 60), XMFLOAT2(basicTex.x + 36 * tenThousand/*�����Ɍ������*/, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
			// ��
			sprGameUI->Draw(XMFLOAT2(scorePos.x + 36 * 2, scorePos.y), XMFLOAT2(36, 60), XMFLOAT2(basicTex.x + 36 * thousand/*�����Ɍ������*/, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
			// �S
			sprGameUI->Draw(XMFLOAT2(scorePos.x + 36 * 3, scorePos.y), XMFLOAT2(36, 60), XMFLOAT2(basicTex.x + 36 * hundred/*�����Ɍ������*/, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
			// �\
			sprGameUI->Draw(XMFLOAT2(scorePos.x + 36 * 4, scorePos.y), XMFLOAT2(36, 60), XMFLOAT2(basicTex.x + 36 * ten/*�����Ɍ������*/, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
			// ��
			sprGameUI->Draw(XMFLOAT2(scorePos.x + 36 * 5, scorePos.y), XMFLOAT2(36, 60), XMFLOAT2(basicTex.x + 36 * one/*�����Ɍ������*/, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
			break;

		case 1:
			basicTex.y += 60.0f;

			// �\��
			sprGameUI->Draw(XMFLOAT2(scorePos.x + 36 * 0, scorePos.y), XMFLOAT2(36, 60), XMFLOAT2(basicTex.x + 36 * hundredThousand/*�����Ɍ������*/, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
			// ��
			sprGameUI->Draw(XMFLOAT2(scorePos.x + 36 * 1, scorePos.y), XMFLOAT2(36, 60), XMFLOAT2(basicTex.x + 36 * tenThousand/*�����Ɍ������*/, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
			// ��
			sprGameUI->Draw(XMFLOAT2(scorePos.x + 36 * 2, scorePos.y), XMFLOAT2(36, 60), XMFLOAT2(basicTex.x + 36 * thousand/*�����Ɍ������*/, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
			// �S
			sprGameUI->Draw(XMFLOAT2(scorePos.x + 36 * 3, scorePos.y), XMFLOAT2(36, 60), XMFLOAT2(basicTex.x + 36 * hundred/*�����Ɍ������*/, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
			// �\
			sprGameUI->Draw(XMFLOAT2(scorePos.x + 36 * 4, scorePos.y), XMFLOAT2(36, 60), XMFLOAT2(basicTex.x + 36 * ten/*�����Ɍ������*/, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
			// ��
			sprGameUI->Draw(XMFLOAT2(scorePos.x + 36 * 5, scorePos.y), XMFLOAT2(36, 60), XMFLOAT2(basicTex.x + 36 * one/*�����Ɍ������*/, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
			break;
		}
	}

	// �`�F�C����
	{
		if (isShowChainNum)
		{
			DirectX::XMFLOAT2 basicSize(36, 60);
			DirectX::XMFLOAT2 basicTex(0, 816);
			DirectX::XMFLOAT2 basicCenter(0.0f, 0.0f);
			DirectX::XMFLOAT4 basicColor(1.0f, 1.0f, 1.0f, showChainAlpha);

			switch (_pn)
			{
			case 0:
				if (nowChainNum < 10)
				{
					// �R���{�����񌅖���
					// ��
					sprGameUI->Draw(DirectX::XMFLOAT2(showChainNumPos.x + ADJUST + MULTIPLAY_ONE_ORIJIN_X, showChainNumPos.y), DirectX::XMFLOAT2(basicSize.x * 2, basicSize.y * 2), DirectX::XMFLOAT2(basicTex.x + 36 * nowChainNum, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
					// �A�I
					sprGameUI->Draw(DirectX::XMFLOAT2(showChainNumPos.x + 81 + ADJUST + MULTIPLAY_ONE_ORIJIN_X, showChainNumPos.y + 51), DirectX::XMFLOAT2(66, 60), DirectX::XMFLOAT2(basicTex.x, basicTex.y + 60), DirectX::XMFLOAT2(66, 60), basicCenter, 0.0f, basicColor);//2
				}
				else
				{
					// �R���{���񌅈ȏ�
					int ten = nowChainNum % 100 / 10;
					int one = nowChainNum % 10 / 1;

					// �\
					sprGameUI->Draw(DirectX::XMFLOAT2(showChainNumPos.x + ADJUST + MULTIPLAY_ONE_ORIJIN_X, showChainNumPos.y), DirectX::XMFLOAT2(basicSize.x * 2, basicSize.y * 2), DirectX::XMFLOAT2(basicTex.x + 36 * ten, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);//1
					// ��
					sprGameUI->Draw(DirectX::XMFLOAT2(showChainNumPos.x + 81 + ADJUST + MULTIPLAY_ONE_ORIJIN_X, showChainNumPos.y), DirectX::XMFLOAT2(basicSize.x * 2, basicSize.y * 2), DirectX::XMFLOAT2(basicTex.x + 36 * one, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);//2
					//�A�I
					sprGameUI->Draw(DirectX::XMFLOAT2(showChainNumPos.x + 162 + ADJUST + MULTIPLAY_ONE_ORIJIN_X, showChainNumPos.y + 51), DirectX::XMFLOAT2(66, 60), DirectX::XMFLOAT2(basicTex.x, basicTex.y + 60), DirectX::XMFLOAT2(66, 60), basicCenter, 0.0f, basicColor);
				}
				break;
				

			case 1:
				if (nowChainNum < 10)
				{
					// �R���{�����񌅖���
					// ��
					sprGameUI->Draw(DirectX::XMFLOAT2(showChainNumPos.x + ADJUST + MULTIPLAY_TWO_ORIJIN_X, showChainNumPos.y), DirectX::XMFLOAT2(basicSize.x * 2, basicSize.y * 2), DirectX::XMFLOAT2(basicTex.x + 36 * nowChainNum, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);
					// �A�I
					sprGameUI->Draw(DirectX::XMFLOAT2(showChainNumPos.x + 81 + ADJUST + MULTIPLAY_TWO_ORIJIN_X, showChainNumPos.y + 51), DirectX::XMFLOAT2(66, 60), DirectX::XMFLOAT2(basicTex.x, basicTex.y + 60), DirectX::XMFLOAT2(66, 60), basicCenter, 0.0f, basicColor);//2
				}
				else
				{
					// �R���{���񌅈ȏ�
					int ten = nowChainNum % 100 / 10;
					int one = nowChainNum % 10 / 1;

					// �\
					sprGameUI->Draw(DirectX::XMFLOAT2(showChainNumPos.x + ADJUST + MULTIPLAY_TWO_ORIJIN_X, showChainNumPos.y), DirectX::XMFLOAT2(basicSize.x * 2, basicSize.y * 2), DirectX::XMFLOAT2(basicTex.x + 36 * ten, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);//1
					// ��
					sprGameUI->Draw(DirectX::XMFLOAT2(showChainNumPos.x + 81 + ADJUST + MULTIPLAY_TWO_ORIJIN_X, showChainNumPos.y), DirectX::XMFLOAT2(basicSize.x * 2, basicSize.y * 2), DirectX::XMFLOAT2(basicTex.x + 36 * one, basicTex.y), basicSize, basicCenter, 0.0f, basicColor);//2
					//�A�I
					sprGameUI->Draw(DirectX::XMFLOAT2(showChainNumPos.x + 162 + ADJUST + MULTIPLAY_TWO_ORIJIN_X, showChainNumPos.y + 51), DirectX::XMFLOAT2(66, 60), DirectX::XMFLOAT2(basicTex.x, basicTex.y + 60), DirectX::XMFLOAT2(66, 60), basicCenter, 0.0f, basicColor);
				}
				break;
			}
		}
	}

	// Ready Go
	{
		if (sceneMultiGame.GetIsGameReady() && _pn == 0)
		{
			if (regularGameUI[0].readyState < 1)
			{
				sprGameUI->Draw(660.0f, regularGameUI[0].readyPosY, 576, 150, 0, 3364, 576, 150, 0, 0, 0.0f, 1.0f, 1.0f, 1.0f, regularGameUI[0].readyAlpha);
			}
			else
			{
				sprGameUI->Draw(660.0f, regularGameUI[0].readyPosY, 576, 158, 0, 3364 + 158, 576, 158, 0, 0, 0.0, 1.0f, 1.0f, 1.0f, regularGameUI[0].readyAlpha);
			}
		}

	}

	sprGameUI->End();

	// �L�����N�^�[�̕`��
	{
		sprCharacter->Begin();

		sprCharacter->Draw(
			charTex.pos, charTex.size,
			charTex.tex, DirectX::XMFLOAT2(320.0f, 416.0f) ,
			DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

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
void GameUI::SetNextBlockColors(int _pn)
{
	for (int i = 0; i < BlockManager::BOARD_ROW_MAX; i++)
	{
		Block* ans = nullptr;
		regularBlockManager[_pn].SearchBlock(i, 9, &ans);
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
void GameUI::UpdateOfGauge(int _pn)
{
	if (!isTimerStop && !regularBlockManager[_pn].GetIsPushing())
	{
		second = interpolationTimeCnt / 60.0f;

		if (maxSecond + 1 < second)
		{
			isTimerStop = true;
			isGaugeMax = true;
			second = 0;
			gaugeDownCnt = 0;
			interpolationTimeCnt = -1;
			regularBlockManager[_pn].SetIsPushUpByGauge(true);
		}
		interpolationTimeCnt++;
	}

	if (isGaugeMax)
	{
		if (GAUGE_DOWN_CNT_MAX <= gaugeDownCnt)
		{
			second = 0;

			auto state = regularBlockManager[_pn].GetStatus();
			if (state != BlockManager::State::Chain && state != BlockManager::State::Break)
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
		pAudio->Play(Sound::Get()->seHandle[Sound::SE::SPEED_UP].get());
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
void GameUI::UpdateOfCharacter(int _pn)
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
		else if (BlockManager::State::Break == regularBlockManager[_pn].GetStatus() ||
			BlockManager::State::Chain == regularBlockManager[_pn].GetStatus()) // �`�F�C����
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
	if (sceneSelect.gameMode == SelectGameMode::Single && !sceneSingleGame.GetIsGameReady()) return;
	if (sceneSelect.gameMode == SelectGameMode::Multi && !sceneMultiGame.GetIsGameReady()) return;
	if (sceneSelect.gameMode == SelectGameMode::CPU && !sceneCPUGame.GetIsGameReady()) return;

	switch (readyState)
	{
	case 0:
		++readyTimer;
		if (readyTimer == 50)
		{
			// play sound.
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::READY].get());
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
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::GO].get());
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
			if (sceneSelect.JudgeGameMode(SelectGameMode::Single))
				sceneSingleGame.SetIsGameReady(false);
			else if (sceneSelect.JudgeGameMode(SelectGameMode::Multi))
				sceneMultiGame.SetIsGameReady(false);
			else if (sceneSelect.JudgeGameMode(SelectGameMode::CPU))
				sceneCPUGame.SetIsGameReady(false);
		}
		break;
	}
}
