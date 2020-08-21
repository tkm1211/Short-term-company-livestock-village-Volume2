#pragma once

#include "Scene.h"
#include "SceneSelect.h"
#include <array>

#pragma region Single Game
class SceneSingleGame : public BaseScene
{
public:
	SelectGameMode gameMode;
private:

	int readyTimer;
	bool isGameReady;

	int bgmRnd;
public:
	bool isGameover;

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

	int bgmRnd;
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
private:
	int bgmRnd;

public:
	SelectGameMode gameMode;

	bool isGameReady;
	std::array<bool, 2> isGameover;
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