#include <Audio.h>
#include "SceneManager.h"
//#include "GamePad.h"
#include "Production.h"
#include "Sound.h"
#include <input_device.h>

SceneTitle sceneTitle;
//PlayStyleSelect playStyleSelect;
//CharacterSelect characterSelect;
//DifficultySelect difficltySelect;
SceneSelect sceneSelect;
SceneGame sceneGame;

BaseScene* scene_tbl[] = {
	&sceneTitle,
	//&playStyleSelect,
	//&characterSelect,
	//&difficltySelect,
	&sceneSelect,
	&sceneGame,
};


void SceneManager::Init()
{
	//InitControllers();
	pAudio->Initialize();
	SOUND->Load();
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
	nowScene->Render();
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

	SOUND->Uninit();
	pAudio->Uninitialize();
}

void SceneManager::SetScene(int _sceneNum)
{
	nextScene = scene_tbl[_sceneNum];
}
