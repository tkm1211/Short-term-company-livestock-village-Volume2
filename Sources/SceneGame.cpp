#include <Audio.h>
#include <input_device.h>

#include "SceneGame.h"
#include "SceneManager.h"
#include "SceneSelect.h"
#include "BlockManager.h"
#include "GameMaster.h"
#include "Production.h"
#include "Player.h"
#include "BG.h"
#include "Effect.h"
#include "Sound.h"
#include "UI.h"
#include "Pause.h"

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>


#define DEBUG_NUM 1
void SceneGame::Init()
{
#if 1
	gameMode = sceneSelect.gameMode + 1;
#else
	gameMode = DEBUG_NUM+1;
#endif
	charNum[0] = sceneSelect.characterNum;
	if (sceneSelect.gameMode == GameMode::TWO_PLAYER) charNum[1] = sceneSelect.characterNum2;

	if (gameMode - 1 != 0)
	{
		isTwoPlayer = true;
	}
	else
	{
		isTwoPlayer = false;
	}
	isGameOver_one = false;
	isGameOver_two = false;

	gameOverDelayTime = 0;
	startTimer = 0;
	goTitle = false;
	goGameInit = false;
	isStartInit = true;

	//‰Šú‰»'s
	BG_INSTANCE->Init();
	GAME_MASTER->Init();

	// TODO : pause init
	pause.Init();

	pAudio->Play(SOUND->bgmHandle[SOUND->GAME], true);
	pAudio->SetVolume(SOUND->bgmHandle[SOUND->GAME], 1.0f, 0);
}

void SceneGame::Update()
{
	if (PRODUCTION->CheckFlag(GO_GAME))
		PRODUCTION->CSOH(SCENE_MANAGER->GAME);

	if (PRODUCTION->CheckFlag(GO_TITLE))
		PRODUCTION->CSOH(SCENE_MANAGER->TITLE);

#ifdef USE_IMGUI
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#endif

	if (!isGameOver_one && !isGameOver_two)
	{
		if (pad[0].bBACKs && !isStartInit)
		{
			if (pad[0].backCnt == 0)
			{
				// TODO : pause on
				if (!pause.isOn)
				{
					pause.isOn = true;
				}
				// TODO : pause off pause on now 
				else if (pause.isOn)
				{
					pause.isOn = false;
				}
			}
			pad[0].backCnt++;
		}
		else
		{
			pad[0].backCnt = 0;
		}
	}

	//XV's	
	// TODO : pause update
	pause.Update();
	if (!pause.isOn)
	{
		BG_INSTANCE->Update();
		GAME_MASTER->Update();
		SOUND->Update();

		if (PRODUCTION->CheckFlag(GO_GAME) || PRODUCTION->CheckFlag(GO_TITLE)) return;

		if (!isTwoPlayer)
		{
			if (isGameOver_one && goTitle)
			{
				PRODUCTION->SetOn(GO_TITLE);
			}
			if (isGameOver_one && goGameInit)
			{
				PRODUCTION->SetOn(GO_GAME);
			}
		}
		else
		{
			if (isGameOver_one && goTitle)
			{
				PRODUCTION->SetOn(GO_TITLE);
			}
			if (isGameOver_one && goGameInit)
			{
				PRODUCTION->SetOn(GO_GAME);
			}

			if (isGameOver_two && goTitle)
			{
				PRODUCTION->SetOn(GO_TITLE);
			}
			if (isGameOver_two && goGameInit)
			{
				PRODUCTION->SetOn(GO_GAME);
			}
		}

	}

	if (isStartInit)
	{
		if (++startTimer >= 180)
		{
			startTimer = 0;
			isStartInit = false;
		}
	}
}

void SceneGame::Render()
{
	//•`‰æ's
	BG_INSTANCE->Draw();
	GAME_MASTER->Draw();

	// TODO : pause draw
	pause.Render();

#ifdef USE_IMGUI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif

	if (PRODUCTION->CheckFlag(GO_GAME) || PRODUCTION->CheckFlag(GO_TITLE))
	{
		PRODUCTION->Draw();
	}

}

void SceneGame::Uninit()
{
	//I—¹ˆ—'s
	BG_INSTANCE->Uninit();
	GAME_MASTER->Uninit();
//	SOUND->Uninit();

	pAudio->Stop(SOUND->bgmHandle[SOUND->GAME]);
	pAudio->DeleteSourceVoice(SOUND->bgmHandle[SOUND->GAME]);
}