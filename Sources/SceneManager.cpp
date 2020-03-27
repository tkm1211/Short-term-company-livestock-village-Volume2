#include <Audio.h>
#include "SceneManager.h"
#include "Production.h"
#include "Resource.h"
#include <input_device.h>
#include "framework.h"

SceneTitle sceneTitle;
SceneSelect sceneSelect;
SceneSingleGame sceneSingleGame;
SceneMultiGame  sceneMultiGame;
SceneCPUGame sceneCPUGame;

BaseScene* scene_tbl[] = {
	&sceneTitle,
	&sceneSelect,
	& sceneSingleGame,
	&sceneCPUGame,
	&sceneMultiGame,
};


void SceneManager::Init()
{
	//InitControllers();
	pAudio->Initialize();
	RESOURCE->Init();
	PRODUCTION->Init();
	SetScene(SCENE::TITLE);
}

void SceneManager::Update()
{
	//UpdateControllerState();
	for (int i = 0; i < PAD_NUM_MAX; i++)
	{
		GetXInputState(&pad[i], i);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (nextScene)
	{
		if (nowScene)
		{
			nowScene->Uninit();
		}
		nowScene = nextScene;
		nextScene = nullptr;
		nowScene->Init();
	}
	nowScene->Update();
}

void SceneManager::Render()
{
	nowScene->Draw();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void SceneManager::Uninit()
{
	nowScene->Uninit();
	if (nowScene)
	{
//		delete nowScene;
	}
	if (nextScene)
	{
		delete nextScene;
	}

	nowScene = nullptr;
	nextScene = nullptr;

	pAudio->Uninitialize();
}

void SceneManager::SetScene(int _sceneNum)
{
	nextScene = scene_tbl[_sceneNum];
}
