#pragma once

#include "easing.h"


enum class SelectMode
{
	PlayStyle,
	CharacterSelect,
	LevelSelect,
	CPULevelSelect,
	Option,
	GoGame
};

enum class UpdateState
{
	BeginMove,
	Choice,
	EndMove
};

enum class SelectGameMode
{
	Single = 0,
	Multi,
	CPU,
	//Tutorial
};

class ISelector
{
protected:
	SelectGameMode gameMode;
	SelectMode selectMode;
	SelectMode thisSelectMode;
	UpdateState updateState;

public:
	ISelector() {}
	virtual ~ISelector() {}

public:
	virtual void Construct() = 0;
	virtual void Init(ISelector* selector) = 0;
	virtual bool Update() = 0;
	virtual void Draw() = 0;

public:
	SelectGameMode GetSelectGameMode() { return gameMode; }
	SelectMode GetSelectMode() { return selectMode; }
	SelectMode GetThisSelectMode() { return thisSelectMode; }
};