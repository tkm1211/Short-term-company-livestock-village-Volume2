#pragma once

#include <memory>
#include <Windows.h>
#include "Scene.h"
#include "SceneTitle.h"
//#include "PlayStyleSelect.h"
//#include "CharacterSelect.h"
//#include "DifficultySelect.h"
#include "SceneSelect.h"
#include "SceneGame.h"

extern SceneTitle sceneTitle;
//extern PlayStyleSelect playStyleSelect;
//extern CharacterSelect characterSelect;
//extern DifficultySelect difficltySelect;
extern SceneSelect sceneSelect;
extern SceneGame sceneGame;


class SceneManager
{
public:
	enum SCENE
	{
		TITLE,
		//PLAY_STYLE_SELECT,
		//CHARACTER_SELECT,
		//DIFFICLTY_SELECT,
		SCENE_SELECT,
		GAME,
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
