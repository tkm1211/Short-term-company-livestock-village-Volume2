#include "CharacterSelect.h"
#include "PlayStyleSelect.h"
#include "SceneManager.h"

#include <GamePad.h>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>


void CharacterSelect::Init()
{
	characterNum = CharacterType::CHARACTER_1;

	isCharacter1 = true;
	isCharacter2 = false;
	isCharacter3 = false;
	isCharacter4 = false;

	rightCnt = 0;
	leftCnt = 0;
}

void CharacterSelect::Update()
{
	if (playStyleSelect.playStyle == PlayStyle::ONE_PLAYER)
	{
		if (/*GetAsyncKeyState('N') < 0 || */InputTrigger(XINPUT_A))
		{
			SCENE_MANAGER->SetScene(SCENE_MANAGER->DIFFICLTY_SELECT);
		}

		if (/*InputState(XINPUT_DPAD_RIGHT)*/ InputState(XINPUT_DPAD_DOWN))
		{
			if (rightCnt == 0)
			{
				if (characterNum < CharacterType::CHARACTER_4) characterNum++;
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
				if (CharacterType::CHARACTER_1 < characterNum) characterNum--;
			}
			leftCnt++;
		}
		else
		{
			leftCnt = 0;
		}

		switch (characterNum)
		{
		case CharacterType::CHARACTER_1:
			isCharacter1 = true;
			isCharacter2 = false;
			isCharacter3 = false;
			isCharacter4 = false;
			break;
		case CharacterType::CHARACTER_2:
			isCharacter1 = false;
			isCharacter2 = true;
			isCharacter3 = false;
			isCharacter4 = false;
			break;
		case CharacterType::CHARACTER_3:
			isCharacter1 = false;
			isCharacter2 = false;
			isCharacter3 = true;
			isCharacter4 = false;
			break;
		case CharacterType::CHARACTER_4:
			isCharacter1 = false;
			isCharacter2 = false;
			isCharacter3 = false;
			isCharacter4 = true;
			break;
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin(u8"キャラクター");
		if (isCharacter1) ImGui::Text(u8"1：<-");
		else ImGui::Text(u8"1：");
		if (isCharacter2) ImGui::Text(u8"2：<-");
		else ImGui::Text(u8"2：");
		if (isCharacter3) ImGui::Text(u8"3：<-");
		else ImGui::Text(u8"3：");
		if (isCharacter4) ImGui::Text(u8"4：<-");
		else ImGui::Text(u8"4：");

		ImGui::Text(u8"\n操作方法：十字ボタンで選択　Aボタンで決定");
		ImGui::End();
	}
	else if (playStyleSelect.playStyle == PlayStyle::TWO_PLAYER)
	{

	}
}

void CharacterSelect::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}


void CharacterSelect::Uninit()
{

}