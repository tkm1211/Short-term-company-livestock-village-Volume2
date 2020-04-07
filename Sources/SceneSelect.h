#pragma once

#include <memory>
#include "Scene.h"
#include "sprite.h"

#define IF_SINGLE_NOW sceneSelect.JudgeGameMode(SceneSelect::SelectGameMode::Single)
#define IF_MULTI_NOW sceneSelect.JudgeGameMode(SceneSelect::SelectGameMode::Multi) || sceneSelect.JudgeGameMode(SceneSelect::SelectGameMode::CPU)

class SceneSelect : public BaseScene
{
	std::shared_ptr<SpriteBatch> sprSelect;

public:
	enum SelectGameMode
	{
		Single = 0,
		CPU,
		Multi,
	};
	SelectGameMode gameMode;
public:
	SceneSelect() { /*Init();*/ }
	void Init();
	void Uninit();
	void Update();
	void Draw();

	bool JudgeGameMode(SelectGameMode _b); // Åya == bÅz is true
};
