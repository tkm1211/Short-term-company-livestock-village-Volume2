#pragma once

#include "Scene.h"

class SceneGame : public BaseScene
{
public:
	SceneGame() { Init(); }
	void Init();
	void Uninit();
	void Update();
	void Draw();
};