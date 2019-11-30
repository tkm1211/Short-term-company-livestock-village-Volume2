#pragma once

#include <memory>

#include "sprite.h"
#include "Singleton.h"

class BG : public Singleton<BG>
{
private:
	std::shared_ptr<SpriteBatch> sprBG;
	std::shared_ptr<SpriteBatch> sprPlayerBack;
	std::shared_ptr<SpriteBatch> sprGrid;

public:
	BG() { Init(); }
	~BG() { Uninit(); }
	void Init();
	void Uninit();
	void Update();
	void Draw();
};

#define BG_INSTANCE BG::getInstance()
