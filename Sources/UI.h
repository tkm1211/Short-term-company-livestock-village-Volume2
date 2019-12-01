#pragma once

#include <memory>
#include <sprite.h>

class GameUI
{
private:
	std::shared_ptr<SpriteBatch> sprGameUI;

public:
	GameUI() {}
	~GameUI() {}
	void Init();
	void Uninit();
	void Update();
	void Draw();
};

extern GameUI provisionalGameUI;