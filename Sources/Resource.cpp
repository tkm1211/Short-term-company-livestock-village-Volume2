#include "Resource.h"

#include "BlockManager.h"

/*--------------------------------------*/
//	Global area
/*--------------------------------------*/

// unknown


/*--------------------------------------*/
//	Menber function
/*--------------------------------------*/
void Resource::Init()
{
	if (!sprTitle)
	{
		sprTitle = std::make_shared<SpriteBatch>(L"Data/title.png");
		assert(sprTitle && "Misstake load texture data <sprTitle>");
	}
	if (!sprSelect)
	{
		sprSelect = std::make_shared<SpriteBatch>(L"Data/modeselect.png");
		assert(sprSelect && "Misstake load texture data <sprSelect>");
	}
	if (!sprBG)
	{
		sprBG = std::make_shared<SpriteBatch>(L"Data/BG.png");
		assert(sprBG && "Misstake load texture data <sprBG>");
	}
	if (!sprCurtain)
	{
		sprCurtain = std::make_shared<SpriteBatch>(L"Data/scenechangeL.png");
		assert(sprCurtain && "Misstake load texture data <sprCurtain>");
	}
	if (!sprDescriptionRule)
	{
		sprDescriptionRule = std::make_shared<SpriteBatch>(L"Data/rule.png");
		assert(sprDescriptionRule && "Misstake load texture data <sprDescriptionRule>");
	}
	if (!sprPlayerBack)
	{
		sprPlayerBack = std::make_shared<SpriteBatch>(L"Data/gameBG.png");
		assert(sprPlayerBack && "Misstake load texture data <sprPlayerBack>");
	}
	if (!sprGrid)
	{
		sprGrid = std::make_shared<SpriteBatch>(L"Data/frame.png");
		assert(sprGrid && "Misstake load texture data <sprGrid>");
	}
	if (!sprUI)
	{
		sprUI = std::make_shared<SpriteBatch>(L"Data/UI.png", 10, DirectX::XMFLOAT2(114, 114));
		assert(sprUI && "Misstake load texture data <sprUI>");
	}
	if (!sprBlock)
	{
		sprBlock = std::make_shared<SpriteBatch>(L"Data/block.png", BlockManager::BLOCK_MAX, DirectX::XMFLOAT2(114, 114));
		assert(sprBlock && "Misstake load texture data <sprBlock>");
	}
}

std::shared_ptr<SpriteBatch>& Resource::GetSpriteData(Resource::Texture _textureName)
{
	std::shared_ptr<SpriteBatch> dummy;
	switch (_textureName)
	{
	case Texture::Title:
		return sprTitle;				// break;
	case Texture::Select:
		return sprSelect;				// break;
	case Texture::BG:
		return sprBG;					// break;
	case Texture::Curtain:
		return sprCurtain;				// break;
	case Texture::DescriptionRule:
		return sprDescriptionRule;		// break;
	case Texture::PlayerBack:
		return sprPlayerBack;			// break;
	case Texture::Grid:
		return sprGrid;					// break;
	case Texture::UI:
		return sprUI;					// break;
	case Texture::Block:
		return sprBlock;				// break;
	default:
		return dummy;
		break;
	}
}
