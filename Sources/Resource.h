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
		TitleBack,
		Select,
		SelectBack,
		BG,
		Curtain,
		DescriptionRule,
		PlayerBack,
		Grid,
		RedFrame,
		BlueFrame,
		UI,
		Block,
		Character01,
		Character02,
		Character03,
		Character04,
		MiniCharacter,
		MiniBlock,
		Pause,
		StackBlock,
		StackNum,
	};

private:
	std::shared_ptr<SpriteBatch> sprTitle;
	std::shared_ptr<SpriteBatch> sprTitleBack;
	std::shared_ptr<SpriteBatch> sprSelect;
	std::shared_ptr<SpriteBatch> sprSelectBack;
	std::shared_ptr<SpriteBatch> sprBG;
	std::shared_ptr<SpriteBatch> sprCurtain;
	std::shared_ptr<SpriteBatch> sprDescriptionRule;
	std::shared_ptr<SpriteBatch> sprPlayerBack;
	std::shared_ptr<SpriteBatch> sprGrid;
	std::shared_ptr<SpriteBatch> sprRedFrame;
	std::shared_ptr<SpriteBatch> sprBlueFrame;
	std::shared_ptr<SpriteBatch> sprUI;
	std::shared_ptr<SpriteBatch> sprBlock;
	std::shared_ptr<SpriteBatch> sprCharcter01;
	std::shared_ptr<SpriteBatch> sprCharcter02;
	std::shared_ptr<SpriteBatch> sprCharcter03;
	std::shared_ptr<SpriteBatch> sprCharcter04;
	std::shared_ptr<SpriteBatch> sprMiniCharacter;
	std::shared_ptr<SpriteBatch> sprMiniBlock;
	std::shared_ptr<SpriteBatch> sprPause;
	std::shared_ptr<SpriteBatch> sprStack;
	std::shared_ptr<SpriteBatch> sprStackNum;


public:
	void Init();

	std::shared_ptr<SpriteBatch>& GetSpriteData(Resource::Texture _textureName);

};

#define RESOURCE Resource::getInstance()

