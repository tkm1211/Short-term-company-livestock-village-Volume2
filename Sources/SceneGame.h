#pragma once

#include "Scene.h"
#include "SceneSelect.h"

#pragma region Single Game
class SceneSingleGame : public BaseScene
{
public:
	SelectGameMode gameMode;
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
	SelectGameMode gameMode;
private:
	bool isGameReady;

public:
	SceneCPUGame() { /*Init();*/ }
	void Init();
	void Uninit();
	void Update();
	void Draw();

	// Getter and Setter
	bool GetIsGameReady() { return isGameReady; }
	void SetIsGameReady(bool _n) { isGameReady = _n; }
};
#pragma endregion

#pragma region Multi Game
class SceneMultiGame : public BaseScene
{
public:
	SelectGameMode gameMode;

	bool isGameReady;
public:
	SceneMultiGame() { /*Init();*/ }
	void Init();
	void Uninit();
	void Update();
	void Draw();

	// Getter and Setter
	bool GetIsGameReady() { return isGameReady; }
	void SetIsGameReady(bool _n) { isGameReady = _n; }

};
#pragma endregion