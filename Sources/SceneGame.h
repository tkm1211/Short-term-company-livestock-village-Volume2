#pragma once

#include "Scene.h"

#pragma region Single Game
class SceneSingleGame : public BaseScene
{
private:
	int readyTimer;

	bool isGameReady;

public:
	SceneSingleGame() { /*Init();*/ }
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void ProcessOfGameReady();

	// Getter and Setter
	bool GetIsGameReady() { return isGameReady; }
	void SetIsGameReady(bool _n) { isGameReady = _n; }
};
#pragma endregion

#pragma region CPU Game
class SceneCPUGame : public BaseScene
{
public:
	SceneCPUGame() { /*Init();*/ }
	void Init();
	void Uninit();
	void Update();
	void Draw();
};
#pragma endregion

#pragma region Multi Game
class SceneMultiGame : public BaseScene
{
public:
	SceneMultiGame() { /*Init();*/ }
	void Init();
	void Uninit();
	void Update();
	void Draw();
};
#pragma endregion