#pragma once

#include "Resource.h"
#include "Scene.h"

class SceneTitle : public BaseScene
{
private:
	std::shared_ptr<SpriteBatch> sprTitle;
public:
	SceneTitle() { /*Init();*/ }
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void Animation();
};