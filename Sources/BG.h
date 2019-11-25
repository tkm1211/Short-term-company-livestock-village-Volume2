#pragma once

#include <memory>
#include <sprite.h>
#include "Singleton.h"

class BG : public Singleton<BG>
{
public:
	std::unique_ptr<SpriteBatch> sprBackBG;
	std::unique_ptr<SpriteBatch> sprGrid;
	std::unique_ptr<SpriteBatch> sprGameBG;

public:
	
	void Init();
	void Uninit();
	void Update();
	void Draw();

};

#define BG_INSTANCE BG::getInstance()
