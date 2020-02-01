#pragma once

#include <memory>
#include <Windows.h>
#include "Scene.h"
#include "SceneTitle.h"
#include "SceneSelect.h"
#include "SceneGame.h"

extern SceneTitle sceneTitle;
extern SceneSelect sceneSelect;
extern SceneSingleGame sceneSingleGame;
extern SceneMultiGame sceneMultiGame;
extern SceneCPUGame sceneCPUGame;


class SceneManager
{
public:
	enum SCENE
	{
		TITLE,
		SCENE_SELECT,
		SINGLE_GAME,
		CPU_GAME,
		MULTI_GAME,
	};

	BaseScene* nowScene;
	BaseScene* nextScene;

public:
	void Init();
	void Uninit();
	void Update();
	void Render();
	void SetScene(int _sceneNum);

	static SceneManager* GetInstance()
	{
		static SceneManager instance;
		return &instance;
	}
};

#define SCENE_MANAGER	SceneManager::GetInstance()

