/*------------------------------------------------------*/

/*includeゾーン*/

/*------------------------------------------------------*/
#include"easing.h"
#include"Production.h"
#include "SceneManager.h"
#include "Audio.h"
#include <input_device.h>
#include <Blender.h>
/*------------------------------------------------------*/

/*定義*/

/*------------------------------------------------------*/


/*------------------------------------------------------*/

/*Productionクラス*/

/*------------------------------------------------------*/
/************************/
//Productionの初期化
/************************/
void Production::Init()
{
//	sprBlackCurtain = std::make_unique<SpriteBatch>(L"Data/change.png", 2, DirectX::XMFLOAT2(0, 0));
	sprBlackCurtainL = std::make_unique<SpriteBatch>(L"Data/scenechangeL.png", 2, DirectX::XMFLOAT2(0, 0));
	sprBlackCurtainR = std::make_unique<SpriteBatch>(L"Data/scenechangeR.png", 2, DirectX::XMFLOAT2(0, 0));
	sprRule = std::make_unique<SpriteBatch>(L"Data/rule.png",5);
}

/********************************/
//イージングを始める前に呼び出す関数
/********************************/
void Production::Start()
{
	time = 0;			//timeを初期化
	state++;			//0から1にする
	easingPosX1 = -960.f - 2.0f;
	easingPosX2 = 1920.0f +1.0f;
	tutorialState = 0;
	tutorialTime = 0;
	tutorialAlpha = 0.0f;
	tutorialNext = false;
	tutorialPosY = 0;
}

/**********************************/
//イージングが終わった時に呼び出す関数
/**********************************/
void Production::End()
{
	checkFlag = 0b00000000; //フラグを全部倒す
	time = 0;				//timeを初期化
	state = 0;				//0から1にする
}

/**********************************/
//イージング関係の描画
/**********************************/
void Production::Draw()
{
	SetDrawBlendMode(BLEND_MODE::ALPHA);
	//DrawBox(0, 0, SCREEN_WIDTH, static_cast<int>(easingPosY1), 0x000000, true);
	//DrawBox(0, SCREEN_HEIGHT, SCREEN_WIDTH, static_cast<int>(easingPosY2), 0x000000, true);
	sprBlackCurtainL->Begin();
	sprBlackCurtainL->Draw(easingPosX1,0,960,1080,0.0f);
	sprBlackCurtainL->End();
	sprBlackCurtainL->Begin();
	sprBlackCurtainL->Draw(easingPosX2, 0, 960, 1080, 0.0f);
	sprBlackCurtainL->End();

	sprRule->Begin();
	sprRule->Draw(288, tutorialPosY, 1344, 756, 0, 0, 1344, 756, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, tutorialAlpha);

	if (tutorialNext)
	{
		sprRule->Draw(1632, 918, 288, 162, 0, 756, 288, 162, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, tutorialAlpha);
	}
	sprRule->End();
}

/*********************************************************/
//フラグが立っているかを確認する変数
/*********************************************************/
bool Production::CheckFlag(unsigned char flag)
{
	if (checkFlag & flag)return true;
	else				return false;
}

/*********************************************************/
//横のOutExp処理をする関数(Change Scene OutExp to horizontal)
/*********************************************************/
void Production::CSOH(int _setStageNum, bool _isDisplayTutorial)
{
	switch (state)
	{
	case INITIALIZE:    //初期化
//		Init();
		Start();
		break;

	case FIRST_EASING:  //1回目のイージング
		//イージング処理を引数にかける
		easingPosX1 = easing::OutExp(time, 60.f, 0.0f + 1.0f, -960.0f - 2.0f);
		easingPosX2 = easing::OutExp(time, 60.f, 960.0f - 1.0f, 1920.0f + 2.0f);
		time++;
		//60フレーム分が終わったら次に進む
		if (time == 60) {
			state++;
			time = 0;
		}
		break;

	case CHANGE:		//切り替え
		if (!_isDisplayTutorial)
		{
			SCENE_MANAGER->SetScene(_setStageNum);
//			pAudio->Play(SOUND->seHandle[SOUND->SCENE_CHANGE]);
			state++;
		}
		else
		{
			switch (tutorialState)
			{
			case INITIALIZE_T:
				tutorialPosY = 162 + 57;
				tutorialState++;
				//break;
			case FIRST_EASING_T:

				if (tutorialAlpha<=1.0f)
				{
					tutorialAlpha += 0.1f;
				}
				tutorialPosY = easing::OutExp(tutorialTime, 60, 162, 162 + 57);

				if (++tutorialTime >= 60)
				{
					tutorialTime = 0;
					tutorialState++;
				}
				break;
			case INTERBAL_T:

				if (++tutorialTime >= 180)
				{
					tutorialNext = true;
				}
				if (tutorialNext)
				{
					//ボタン処理
					if(pad[0].bAt/*tutorialNext/*処理変える*/)
					{
						tutorialState++;
						tutorialTime = 0;
					}
				}
				break;
			case SECOND_EASING_T:
				if (tutorialAlpha >= 0.0f)
				{
					tutorialAlpha -= 0.1f;
				}
				if (++tutorialTime >= 60)
				{
					tutorialTime = 0;
					tutorialState++;
				}
				break;
			case CHANGE_T:
				SCENE_MANAGER->SetScene(_setStageNum);
				tutorialState = 0;
				tutorialTime = 0;
				state++;
				break;
			}
		}
		break;

	case INTERBAL:		//インターバル
		if (30 <= ++time) {
			state++;
			time = 0;
		}
		break;

	case SECOND_EASING: //2回目のイージング
		//イージング処理を引数にかける
		time++;
		easingPosX1 = easing::OutExp(time, 60.f, -960.0f - 2.0f, 0.0f + 1.0f);
		easingPosX2 = easing::OutExp(time, 60.f, 1920.0f + 2.0f, 960.0f - 1.0f);

		//60フレーム分が終わったら次に進む
		if (time == 60) {
			time = 0;
			state++;
		}
		break;

	case END:			//終了処理
		End();
		break;
    }
}
