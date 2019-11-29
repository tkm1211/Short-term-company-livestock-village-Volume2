#pragma once

#include "Scene.h"

class SceneSelect : public BaseScene
{
public:
	SceneSelect() { Init(); }
	void Init();
	void Uninit();
	void Update();
	void Draw();
};
