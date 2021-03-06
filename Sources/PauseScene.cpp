#include "PauseScene.h"
#include "Resource.h"
#include <framework.h>
#include <input_device.h>
#include "Production.h"
#include "SceneSelect.h"
#include "SceneManager.h"
#include "Sound.h"

Pause pause;

void Pause::Init()
{
	sprPause = RESOURCE->GetSpriteData(Resource::Texture::Pause);
	sprBack = RESOURCE->GetSpriteData(Resource::Texture::SelectBack);

	backData.pos = DirectX::XMFLOAT2(0.0f, 0.0f);
	backData.tex = DirectX::XMFLOAT2(0.0f, 0.0f);
	backData.size = DirectX::XMFLOAT2((float)framework::SCREEN_WIDTH, (float)framework::SCREEN_HEIGHT);

	menuData.pos = DirectX::XMFLOAT2((float)framework::SCREEN_WIDTH / 2.0f - 336.0f, (float)framework::SCREEN_HEIGHT / 2.0f - 393.0f);
	menuData.tex = DirectX::XMFLOAT2(0.0f, 0.0f);
	menuData.size = DirectX::XMFLOAT2(672.0f, 786.0f);

	selectData.pos = DirectX::XMFLOAT2((float)framework::SCREEN_WIDTH / 2.0f - 336.0f, (float)framework::SCREEN_HEIGHT / 2.0f - 393.0f);
	selectData.tex = DirectX::XMFLOAT2(0.0f, 0.0f);
	selectData.size = DirectX::XMFLOAT2(672.0f, 786.0f);

	pushCnt[0] = 0;
	pushCnt[1] = 0;

	isPauseNow = false;
	lastIsPauseNow = false;
	selectMenu = SelectMenu::BackGame;
}

void Pause::Uninit()
{

}

void Pause::Update()
{
	lastIsPauseNow = isPauseNow;

	// ゲーム中以外は作用しない
	if (sceneMultiGame.isGameover[0] || sceneMultiGame.isGameover[1] || sceneSingleGame.isGameover)return;
	if (sceneSelect.gameMode == SelectGameMode::Single && sceneSingleGame.GetIsGameReady()) return;
	if (sceneSelect.gameMode == SelectGameMode::Multi && sceneMultiGame.GetIsGameReady()) return;
	if (sceneSelect.gameMode == SelectGameMode::CPU && sceneCPUGame.GetIsGameReady()) return;


	// 切り替え処理
	{
		static int cnt = 0;
		if (pad[0].bSTARTt)
		{
			if (++cnt == 1)
			{
				isPauseNow = !isPauseNow;
				pAudio->Play(Sound::Get()->seHandle[Sound::SE::OK].get());
			}
		}
		else
			cnt = 0;
	}

	if (!isPauseNow)return;

	// 選択
	{
		if (pad[0].sLY < 0 || pad[0].bUPs)
		{
			if (++pushCnt[0] == 1)
			{
				if (SelectMenu::BackGame < selectMenu)
				{
					selectMenu--;
					pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
				}
			}
			if (pushCnt[0] >= 15)
			{
				if (pushCnt[0] % 7 == 0)
				{
					if (SelectMenu::BackGame < selectMenu)
					{
						selectMenu--;
						pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
					}
				}
			}
		}
		else
			pushCnt[0] = 0;

		if (0 < pad[0].sLY || pad[0].bDOWNs)
		{
			if (++pushCnt[1] == 1)
			{
				if (selectMenu < SelectMenu::GoTitle)
				{
					selectMenu++;
					pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
				}
			}
			if (pushCnt[1] >= 15)
			{
				if (pushCnt[1] % 7 == 0)
				{
					if (selectMenu < SelectMenu::GoTitle)
					{
						selectMenu++;
						pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
					}
				}
			}
		}
		else
			pushCnt[1] = 0;
	}

	selectData.tex.x = 672.0f * (selectMenu + 1);

	// 決定
	{
		if (pad[0].bAt)
		{
			switch (selectMenu)
			{
			case SelectMenu::BackGame:
				isPauseNow = false;
				break;
			case SelectMenu::Restart:
				if (IF_SINGLE_NOW)
					PRODUCTION->SetOn(GO_SINGLEGAME);
				else if(sceneSelect.JudgeGameMode(SelectGameMode::Multi))
					PRODUCTION->SetOn(GO_MULTIGAME);
				else
					PRODUCTION->SetOn(GO_CPUGAME);
				break;
			case SelectMenu::GoTitle:
				PRODUCTION->SetOn(GO_TITLE);
				break;
			default:
				break;
			}
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::OK].get());
		}
	}

	// 戻る
	{
		if (pad[0].bBt)
		{
			isPauseNow = false;
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::BACK].get());
		}
	}

}

void Pause::Draw()
{
	if (!isPauseNow)return;

	// 背景
	{
		sprBack->Begin();
		sprBack->Draw(backData);
		sprBack->End();
	}

	// メニュー
	{
		sprPause->Begin();
		sprPause->Draw(menuData);
		sprPause->End();
	}

	// 選択してる所を光らせる
	{
		SetDrawBlendMode(BLEND_MODE::ADD);
		sprPause->Begin();
		sprPause->Draw(selectData);
		sprPause->End();
		SetDrawBlendMode(BLEND_MODE::ALPHA);
	}
}