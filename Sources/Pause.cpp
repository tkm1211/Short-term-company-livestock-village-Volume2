#include <DirectXMath.h>
#include <framework.h>
#include <input_device.h>

#include "Pause.h"
#include "Production.h"

Pause pause;


void Pause::Init()  
{
	sprPause = std::make_unique<SpriteBatch>(L"Data/pause.png", 4, DirectX::XMFLOAT2(0.0f, 0.0f));
	sprPauseBack = std::make_unique<SpriteBatch>(L"Data/selectBG.png", 1, DirectX::XMFLOAT2(0.0f, 0.0f));

	pauseBack.pos = DirectX::XMFLOAT2(0.0f, 0.0f);
	pauseBack.tex = DirectX::XMFLOAT2(0.0f, 0.0f);
	pauseBack.size = DirectX::XMFLOAT2((float)framework::SCREEN_WIDTH, (float)framework::SCREEN_HEIGHT);

	back.pos = DirectX::XMFLOAT2((float)framework::SCREEN_WIDTH / 2.0f - 672.0f / 2.0f, (float)framework::SCREEN_HEIGHT / 2.0f - 786.0f / 2.0f);
	back.tex = DirectX::XMFLOAT2(0.0f, 0.0f);
	back.size = DirectX::XMFLOAT2(672.0f, 786.0f);

	select.pos = DirectX::XMFLOAT2((float)framework::SCREEN_WIDTH / 2.0f - 672.0f / 2.0f, (float)framework::SCREEN_HEIGHT / 2.0f - 786.0f / 2.0f);
	select.tex = DirectX::XMFLOAT2(672.0f, 0.0f);
	select.size = DirectX::XMFLOAT2(672.0f, 786.0f);

	selectState = PauseSelectState::RETURN_GAME;

	pushCnt[0] = 0;
	pushCnt[1] = 0;

	isOn = false;
}

void Pause::Update()
{
	if (isOn)
	{
		if (pad[0].sY < 0 || pad[0].bUPs)
		{
			if (++pushCnt[0] == 1)
			{
				if (PauseSelectState::RETURN_GAME < selectState)
				{
					selectState--;
				}
			}
			if (pushCnt[0] >= 15)
			{
				if (pushCnt[0] % 7 == 0)
				{
					if (PauseSelectState::RETURN_GAME < selectState)
					{
						selectState--;
					}
				}
			}
		}
		else
		{
			pushCnt[0] = 0;
		}

		if (0 < pad[0].sY || pad[0].bDOWNs)
		{
			if (++pushCnt[1] == 1)
			{
				if (selectState < PauseSelectState::TO_TITLE)
				{
					selectState++;
				}
			}
			if (pushCnt[1] >= 15)
			{
				if (pushCnt[1] % 7 == 0)
				{
					if (selectState < PauseSelectState::TO_TITLE)
					{
						selectState++;
					}
				}
			}
		}
		else
		{
			pushCnt[1] = 0;
		}

		select.tex.x = 672.0f * (selectState + 1);

		if (pad[0].bAt)
		{
			if (!pad[0].isButtomA)
			{
				pad[0].isButtomA = true;
				switch (selectState)
				{
				case RETURN_GAME:
					isOn = false;
					break;
				case RETRY:
					PRODUCTION->SetOn(GO_GAME);
					break;
				case TO_TITLE:
					PRODUCTION->SetOn(GO_TITLE);
					break;
				}
			}
		}
		else 
		{
			pad[0].isButtomA = false;
		}
	}
}

void Pause::Render()
{
	if (isOn)
	{
		SetDrawBlendMode(BLEND_MODE::ALPHA);
		sprPauseBack->Begin();
		sprPauseBack->Draw(pauseBack.pos, pauseBack.size, pauseBack.tex, pauseBack.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		sprPauseBack->End();
		SetDrawBlendMode(BLEND_MODE::ALPHA);


		SetDrawBlendMode(BLEND_MODE::ALPHA);
		sprPause->Begin();

		sprPause->Draw(back.pos, back.size, back.tex, back.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		sprPause->End();
		SetDrawBlendMode(BLEND_MODE::ALPHA);


		SetDrawBlendMode(BLEND_MODE::ADD);
		sprPause->Begin();

		sprPause->Draw(select.pos, select.size, select.tex, select.size, DirectX::XMFLOAT2(0.0f, 0.0f), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		sprPause->End();
		SetDrawBlendMode(BLEND_MODE::ALPHA);
	}
}


void Pause::Uninit()
{

	}