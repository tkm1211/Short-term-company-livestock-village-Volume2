#include "PlayStyleSelect.h"
#include "SceneManager.h"
#include "Production.h"

#include <GamePad.h>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>


void PlayStyleSelect::Init()
{
	playStyle = PlayStyle::ONE_PLAYER;

	isOnePlayer = true;
	isTwoPlayer = false;
}

void PlayStyleSelect::Update()
{
	if (/*GetAsyncKeyState('N') < 0 || */InputTrigger(XINPUT_A) && !PRODUCTION->CheckFlag(GO_PLAYERSTILE))
	{
		if (playStyle == PlayStyle::ONE_PLAYER || (playStyle == PlayStyle::TWO_PLAYER && 2 < dInput.js.size()))
		{
			SCENE_MANAGER->SetScene(SCENE_MANAGER->CHARACTER_SELECT);
		}
	}

	if (/*InputState(XINPUT_DPAD_RIGHT)*/ InputState(XINPUT_DPAD_DOWN))
	{
		if (playStyle < PlayStyle::TWO_PLAYER) playStyle++;
	}
	if (/*InputState(XINPUT_DPAD_LEFT)*/ InputState(XINPUT_DPAD_UP))
	{
		if (PlayStyle::ONE_PLAYER < playStyle) playStyle--;
	}

	switch (playStyle)
	{
	case PlayStyle::ONE_PLAYER:
		isOnePlayer = true;
		isTwoPlayer = false;
		break;
	case PlayStyle::TWO_PLAYER:
		isOnePlayer = false;
		isTwoPlayer = true;
		break;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin(u8"モード選択");
	if (isOnePlayer) ImGui::Text(u8"1P：<-");
	else ImGui::Text(u8"1P：");
	if (isTwoPlayer)
	{
		if (2 < dInput.js.size()) ImGui::Text(u8"2P：<-");
		else ImGui::Text(u8"2P：<- 2個のコントローラーが接続されていないのプレイ不可");
	}
	else ImGui::Text(u8"2P：");

	ImGui::Text(u8"\n操作方法：十字ボタンで選択　Aボタンで決定");
	ImGui::End();

	if (PRODUCTION->CheckFlag(GO_PLAYERSTILE))
		PRODUCTION->CSOH(SCENE_MANAGER->PLAY_STYLE_SELECT);
}

void PlayStyleSelect::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (PRODUCTION->CheckFlag(GO_PLAYERSTILE))
	{
		PRODUCTION->Draw();
	}
}


void PlayStyleSelect::Uninit()
{

}