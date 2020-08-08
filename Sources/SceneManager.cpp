#include <Audio.h>
#include "SceneManager.h"
#include "Production.h"
#include "Resource.h"
#include <input_device.h>
#include "framework.h"

#include "Sound.h"

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
	Sound::Create();
	Sound::Get()->Load();
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
	Sound::Get()->Update();
}

void SceneManager::Render()
{
	ImGui::Render();
	nowScene->Draw();

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

	Sound::Get()->Uninit();
	Sound::Destroy();
	pAudio->Uninitialize();
}

void SceneManager::SetScene(int _sceneNum)
{
	nextScene = scene_tbl[_sceneNum];
}
