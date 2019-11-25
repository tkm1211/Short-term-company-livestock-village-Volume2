#include <Windows.h>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>

//#include <GamePad.h>
#include <input_device.h>

#include "Sound.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "Production.h"
#include "easing.h"




void SceneTitle::Init()
{
	sprTitleBack = std::make_unique<SpriteBatch>(L"Data/titleBG.png");
	sprObject = std::make_unique<SpriteBatch>(L"Data/title.png",10);
	sprChar = std::make_unique<SpriteBatch>(L"Data/character01.png", 1, DirectX::XMFLOAT2(416, 416));
	sprTeamLogo = std::make_unique<SpriteBatch>(L"Data/logo.png", 1, DirectX::XMFLOAT2(336, 248));
	sprChange = std::make_unique<SpriteBatch>(L"Data/change.png");
	trgCnt = 0;

	rogoPos = DirectX::XMFLOAT2(0.0f, 0.0f);
	blockPos1 = DirectX::XMFLOAT2(0.0f,696.0f);
	blockPos2 = DirectX::XMFLOAT2(0.0f, 696.0f);
	characterPos = DirectX::XMFLOAT2(2050.0f, -50.0f);
	textPos = DirectX::XMFLOAT2(432.0f, 600.0f);
	teamLogoPos = DirectX::XMFLOAT2(1920.0f, 0.0f);
	charAnimTime = 0;
	charAnimFrame = 0;
	textAlpha = 0;
	state = 0;
	teamLogoAlpha = 0.0f;
	animFrame = 0;
	isPlus = true;
	isPlayBGM = false;
	isFade = false;
	isPlayCan = false;
	isplayLogo = false;

	teamLogoTimer = 0;
	isTeamLogoEasing = true;
	//SetXInputVibration(65535 / 2, 65535 / 2);
	//SetXInputVibration(65535 / 2, 65535 / 2, 1);

	/*vib = 10000;
	__cnt = 60;*/
}

void SceneTitle::Update()
{
	if (PRODUCTION->CheckFlag(GO_PLAYERSTILE))
		PRODUCTION->CSOH(SCENE_MANAGER->SCENE_SELECT);
	if (PRODUCTION->CheckFlag(GO_TITLE))
		PRODUCTION->CSOH(SCENE_MANAGER->TITLE);

	if (PRODUCTION->CheckFlag(GO_PLAYERSTILE) || PRODUCTION->CheckFlag(GO_TITLE)) return;

	SOUND->Update();

	switch (state)
	{
	case 0:
		if (isTeamLogoEasing)
		{
			teamLogoPos.x = easing::OutExp(static_cast<float>(teamLogoTimer), 60.0f, 436.0f, 1920.0f);
			if (++teamLogoTimer >= 60)
			{
				teamLogoTimer = 0;
				isTeamLogoEasing = false;
			}
		}
		else
		{
			++teamLogoTimer;
			if (!isFade)
			{
				if (animFrame <= 4)
				{
					if (teamLogoTimer % 3 == 0)
					{
						animFrame++;
					}
					if (animFrame == 4)
					{
						if (!isPlayCan)
						{
							pAudio->Play(SOUND->seHandle[SOUND->LOGO_CAN]);
							isPlayCan = true;
						}
					}
				}
				else
				{
					if (animFrame == 8)
					{
						if (!isplayLogo)
						{
							pAudio->Play(SOUND->seHandle[SOUND->LOGO]);
							isplayLogo = true;
						}
					}
					if (teamLogoTimer % 8 == 0)
					{
						animFrame++;
						if (animFrame >= 19)
						{
							animFrame = 19;
							isFade = true;
						}
					}
				}
			}
			else
			{
				teamLogoAlpha += 0.01f;
				if (teamLogoAlpha >= 1.0f)
				{
					state++;
					teamLogoTimer = 0;
				}
			}
		}
		break;
	case 1:
		teamLogoAlpha -= 0.07f;
		if (teamLogoAlpha <= 0.0f)
		{
			teamLogoAlpha = 0.0f;
		}
		if (!isPlayBGM)
		{
			isPlayBGM = true;
			pAudio->Play(SOUND->bgmHandle[SOUND->TITLE], true);
			pAudio->SetVolume(SOUND->bgmHandle[SOUND->TITLE], 1.0f, 0);
		}

		// アニメーション
		Animation();


		/*ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin(u8"pad");
		ImGui::SliderFloat("vib speed", &vib, 0, 65535);
		ImGui::SliderInt("maxCnt", &__cnt, 0, 10 * 60);
		ImGui::End();*/

		/*if (GetAsyncKeyState('I') < 0)
		{
			SetXInputVibration((int)vib, (int)vib, __cnt, 0);
		}
		if (GetAsyncKeyState('O') < 0)
		{
			SetXInputVibration((int)vib, (int)vib, __cnt, 1);
		}
		if (GetAsyncKeyState('K') < 0)
		{
			SetXInputVibration((int)vib, (int)vib, __cnt, 0);
			SetXInputVibration((int)vib, (int)vib, __cnt, 1);
		}*/
		/*if (GetAsyncKeyState('L') < 0)
		{
			SetXInputVibration(0, 0, 1);
		}*/

		if (PRODUCTION->CheckFlag(GO_PLAYERSTILE))
			PRODUCTION->CSOH(SCENE_MANAGER->SCENE_SELECT);
		if (PRODUCTION->CheckFlag(GO_TITLE))
			PRODUCTION->CSOH(SCENE_MANAGER->TITLE);

		if (PRODUCTION->CheckFlag(GO_PLAYERSTILE) || PRODUCTION->CheckFlag(GO_TITLE)) return;


		if (GetAsyncKeyState('N') < 0 || pad[0].bAt/* || InputTrigger(XINPUT_A, 1)*/)
		{
			//		SCENE_MANAGER->SetScene(SCENE_MANAGER->PLAY_STYLE_SELECT);
			PRODUCTION->SetOn(GO_PLAYERSTILE);
			PRODUCTION->Start();
			pAudio->Play(SOUND->seHandle[SOUND->OK]);
		}

		break;
	}

}

void SceneTitle::Render()
{
	switch (state)
	{
	case 0:
		sprTeamLogo->Begin();
		sprTeamLogo->Draw(teamLogoPos.x,teamLogoPos.y,1008,744,0.0f,1.0f,1.0f,1.0f,1.0f,animFrame);
		sprTeamLogo->End();
		break;
	case 1:
		sprTitleBack->Begin();
		sprTitleBack->Draw(0, 0, 1920, 1080, 0.0f);
		sprTitleBack->End();

		sprObject->Begin();
		// ロゴ
		sprObject->Draw(rogoPos, DirectX::XMFLOAT2(1184.0f, 544.0f), DirectX::XMFLOAT2(0, 922.0f), DirectX::XMFLOAT2(1184.0f, 544.0f), DirectX::XMFLOAT2(0.0f, 0.0), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		// Block Big
		sprObject->Draw(blockPos1, DirectX::XMFLOAT2(1920.0f, 384.0f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1920.0f, 384.0f), DirectX::XMFLOAT2(0.0f, 0.0), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		sprObject->End();

		sprChar->Begin();
		// キャラ
		sprChar->Draw(characterPos.x, characterPos.y, -416 * 3, 416 * 3, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, charAnimFrame);
		sprChar->End();

		sprObject->Begin();
		// Block small
		sprObject->Draw(blockPos2, DirectX::XMFLOAT2(1920.0f, 384.0f), DirectX::XMFLOAT2(0.0f, 384.0f), DirectX::XMFLOAT2(1920.0f, 384.0f), DirectX::XMFLOAT2(0.0f, 0.0), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		// text
		sprObject->Draw(textPos, DirectX::XMFLOAT2(1056.0f, 154.0f), DirectX::XMFLOAT2(0.0f, 768.0f), DirectX::XMFLOAT2(1056.0f, 154.0f), DirectX::XMFLOAT2(0.0f, 0.0), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, textAlpha));
		sprObject->End();


		/*ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());*/

		break;

	}
	sprChange->Begin();
	sprChange->Draw(0, 0, 1920, 1080, 0, 0, 960, 1080, 0.0f, 0.0f, 0.0f, teamLogoAlpha, teamLogoAlpha, teamLogoAlpha, teamLogoAlpha);
	sprChange->End();

	if (PRODUCTION->CheckFlag(GO_PLAYERSTILE) || PRODUCTION->CheckFlag(GO_TITLE))
	{
		PRODUCTION->Draw();
	}

}

void SceneTitle::Uninit()
{
	if (state != 0)
	{
		pAudio->Stop(SOUND->bgmHandle[SOUND->TITLE]);
	}
	pAudio->DeleteSourceVoice(SOUND->bgmHandle[SOUND->TITLE]);
}


void SceneTitle::Animation()
{
	// キャラクター
	if (++charAnimTime % 8 == 0)
	{
		if (++charAnimFrame >= 6)
		{
			charAnimFrame = 0;
			charAnimTime = 0;
		}
	}

	// タイトルロゴ
	static int a;
	float t = DirectX::XMConvertToRadians((float)a);

	rogoPos.y = sinf(t) * 25;

	a+=3;
	if (a >= 180)
	{
		a *= -1;
	}

	// ふぁんふぁん
	switch (isPlus)
	{
	case true:
		textAlpha += 0.01f;
		if (textAlpha >= 1.0f)
		{
			textAlpha = 1.0f;
			isPlus = false;
		}
		break;
	case false:
		textAlpha -= 0.01f;
		if (textAlpha <= 0.5f)
		{
			textAlpha = 0.5f;
			isPlus = true;
		}
		break;
	}
}