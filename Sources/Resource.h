#pragma once

#include <memory>

#include "Singleton.h"
#include "Sprite.h"

class Resource : public Singleton<Resource>
{
public:
	enum Texture
	{
		Title,
		Select,
		BG,
		Curtain,
		DescriptionRule,
		PlayerBack,
		Grid,
		UI,
		Block,
		Character01,
		Character02,
		Character03,
		Character04,
	};

private:
	std::shared_ptr<SpriteBatch> sprTitle;
	std::shared_ptr<SpriteBatch> sprSelect;
	std::shared_ptr<SpriteBatch> sprBG;
	std::shared_ptr<SpriteBatch> sprCurtain;
	std::shared_ptr<SpriteBatch> sprDescriptionRule;
	std::shared_ptr<SpriteBatch> sprPlayerBack;
	std::shared_ptr<SpriteBatch> sprGrid;
	std::shared_ptr<SpriteBatch> sprUI;
	std::shared_ptr<SpriteBatch> sprBlock;
	std::shared_ptr<SpriteBatch> sprCharcter01;
	std::shared_ptr<SpriteBatch> sprCharcter02;
	std::shared_ptr<SpriteBatch> sprCharcter03;
	std::shared_ptr<SpriteBatch> sprCharcter04;

public:
	void Init();

	std::shared_ptr<SpriteBatch>& GetSpriteData(Resource::Texture _textureName);

};

#define RESOURCE Resource::getInstance()

