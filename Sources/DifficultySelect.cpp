#include "DifficultySelect.h"
#include "PlayStyleSelect.h"
#include "SceneManager.h"
#include "Production.h"

#include <GamePad.h>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>


void DifficultySelect::Init()
{
	difficultyNum = DifficultyType::DIFFICULTY_4;
	colorNum = difficultyColorNum[difficultyNum];

	isDifficulty1 = false;
	isDifficulty2 = false;
	isDifficulty3 = false;
	isDifficulty4 = false;
	isDifficulty5 = false;
	isDifficulty6 = false;
	isDifficulty7 = false;

	rightCnt = 0;
	leftCnt = 0;
}

void DifficultySelect::Update()
{
	if (PRODUCTION->CheckFlag(GO_GAME))
		PRODUCTION->CSOH(GO_GAME);
	if (PRODUCTION->CheckFlag(GO_GAME))return;
	if (playStyleSelect.playStyle == PlayStyle::ONE_PLAYER)
	{
		if (/*GetAsyncKeyState('N') < 0 || */InputTrigger(XINPUT_A))
		{
//			SCENE_MANAGER->SetScene(SCENE_MANAGER->GAME);
			PRODUCTION->SetOn(GO_GAME);
		}

		if (/*InputState(XINPUT_DPAD_RIGHT)*/ InputState(XINPUT_DPAD_DOWN))
		{
			if (rightCnt == 0)
			{
				if (difficultyNum < DifficultyType::DIFFICULTY_7) difficultyNum++;
			}
			rightCnt++;
		}
		else
		{
			rightCnt = 0;
		}
		if (/*InputState(XINPUT_DPAD_LEFT)*/ InputState(XINPUT_DPAD_UP))
		{
			if (leftCnt == 0)
			{
				if (DifficultyType::DIFFICULTY_1 < difficultyNum) difficultyNum--;
			}
			leftCnt++;
		}
		else
		{
			leftCnt = 0;
		}

		switch (difficultyNum)
		{
		case DifficultyType::DIFFICULTY_1:
			isDifficulty1 = true;
			isDifficulty2 = false;
			isDifficulty3 = false;
			isDifficulty4 = false;
			isDifficulty5 = false;
			isDifficulty6 = false;
			isDifficulty7 = false;
			break;
		case DifficultyType::DIFFICULTY_2:
			isDifficulty1 = false;
			isDifficulty2 = true;
			isDifficulty3 = false;
			isDifficulty4 = false;
			isDifficulty5 = false;
			isDifficulty6 = false;
			isDifficulty7 = false;
			break;
		case DifficultyType::DIFFICULTY_3:
			isDifficulty1 = false;
			isDifficulty2 = false;
			isDifficulty3 = true;
			isDifficulty4 = false;
			isDifficulty5 = false;
			isDifficulty6 = false;
			isDifficulty7 = false;
			break;
		case DifficultyType::DIFFICULTY_4:
			isDifficulty1 = false;
			isDifficulty2 = false;
			isDifficulty3 = false;
			isDifficulty4 = true;
			isDifficulty5 = false;
			isDifficulty6 = false;
			isDifficulty7 = false;
			break;
		case DifficultyType::DIFFICULTY_5:
			isDifficulty1 = false;
			isDifficulty2 = false;
			isDifficulty3 = false;
			isDifficulty4 = false;
			isDifficulty5 = true;
			isDifficulty6 = false;
			isDifficulty7 = false;
			break;
		case DifficultyType::DIFFICULTY_6:
			isDifficulty1 = false;
			isDifficulty2 = false;
			isDifficulty3 = false;
			isDifficulty4 = false;
			isDifficulty5 = false;
			isDifficulty6 = true;
			isDifficulty7 = false;
			break;
		case DifficultyType::DIFFICULTY_7:
			isDifficulty1 = false;
			isDifficulty2 = false;
			isDifficulty3 = false;
			isDifficulty4 = false;
			isDifficulty5 = false;
			isDifficulty6 = false;
			isDifficulty7 = true;
			break;
		}
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin(u8"難易度選択");
		if (isDifficulty1) ImGui::Text(u8"1：<-");
		else ImGui::Text(u8"1：");
		if (isDifficulty2) ImGui::Text(u8"2：<-");
		else ImGui::Text(u8"2：");
		if (isDifficulty3) ImGui::Text(u8"3：<-");
		else ImGui::Text(u8"3：");
		if (isDifficulty4) ImGui::Text(u8"4：<-");
		else ImGui::Text(u8"4：");
		if (isDifficulty5) ImGui::Text(u8"5：<-");
		else ImGui::Text(u8"5：");
		if (isDifficulty6) ImGui::Text(u8"6：<-");
		else ImGui::Text(u8"6：");
		if (isDifficulty7) ImGui::Text(u8"7：<-");
		else ImGui::Text(u8"7：");

		ImGui::Text(u8"\n操作方法：十字ボタンで選択　Aボタンで決定");
		ImGui::End();

		colorNum = difficultyColorNum[difficultyNum];
	}
	else if (playStyleSelect.playStyle == PlayStyle::TWO_PLAYER)
	{

	}
}

void DifficultySelect::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (PRODUCTION->CheckFlag(GO_GAME))
	{
		PRODUCTION->Draw();
	}
}


void DifficultySelect::Uninit()
{

}