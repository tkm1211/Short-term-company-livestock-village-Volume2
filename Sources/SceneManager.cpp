#include <Audio.h>
#include "SceneManager.h"
#include "Production.h"
#include "Resource.h"
#include <input_device.h>

SceneTitle sceneTitle;
SceneSelect sceneSelect;
SceneSingleGame sceneSingleGame;

BaseScene* scene_tbl[] = {
	&sceneTitle,
	&sceneSelect,
	& sceneSingleGame,
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
