#pragma once

#include <memory>
#include "Scene.h"
#include "sprite.h"

class SceneSelect : public BaseScene
{
	std::shared_ptr<SpriteBatch> sprSelect;
public:
	SceneSelect() { /*Init();*/ }
	void Init();
	void Uninit();
	void Update();
	void Draw();
};
