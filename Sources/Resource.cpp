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
		sprTitle = std::make_shared<SpriteBatch>(L"Data/title.png", 10);
		assert(sprTitle && "Misstake load texture data <sprTitle>");
	}
	if (!sprTitleBack)
	{
		sprTitleBack = std::make_shared<SpriteBatch>(L"Data/titleBG.png");
		assert(sprTitleBack && "Misstake load texture data <sprTitleBack>");
	}
	if (!sprSelect)
	{
		sprSelect = std::make_shared<SpriteBatch>(L"Data/modeselect.png", 10);
		assert(sprSelect && "Misstake load texture data <sprSelect>");
	}
	if (!sprSelectBack)
	{
		sprSelectBack = std::make_shared<SpriteBatch>(L"Data/selectBG.png");
		assert(sprSelectBack && "Mistake load texture data <sprSelectBack>");
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
		sprDescriptionRule = std::make_shared<SpriteBatch>(L"Data/rule.png", 10);
		assert(sprDescriptionRule && "Misstake load texture data <sprDescriptionRule>");
	}
	if (!sprPlayerBack)
	{
		sprPlayerBack = std::make_shared<SpriteBatch>(L"Data/gameBG.png");
		assert(sprPlayerBack && "Misstake load texture data <sprPlayerBack>");
	}
	if (!sprGrid)
	{
		sprGrid = std::make_shared<SpriteBatch>(L"Data/frame.png", 2);
		assert(sprGrid && "Misstake load texture data <sprGrid>");
	}
	if (!sprBlueFrame)
	{
		sprBlueFrame = std::make_shared<SpriteBatch>(L"Data/frame1p.png");
		assert(sprBlueFrame && "Misstake load texture data <sprBlueFrame>");
	}
	if (!sprRedFrame)
	{
		sprRedFrame = std::make_shared<SpriteBatch>(L"Data/frame2p.png");
		assert(sprRedFrame && "Misstake load texture data <sprRedFrame>");
	}
	if (!sprUI)
	{
		sprUI = std::make_shared<SpriteBatch>(L"Data/UI.png", 100, DirectX::XMFLOAT2(114, 114));
		assert(sprUI && "Misstake load texture data <sprUI>");
	}
	if (!sprBlock)
	{
		sprBlock = std::make_shared<SpriteBatch>(L"Data/block.png", BlockManager::BLOCK_NUM_MAX, DirectX::XMFLOAT2(114, 114));
		assert(sprBlock && "Misstake load texture data <sprBlock>");
	}
	if (!sprCharcter01)
	{
		sprCharcter01 = std::make_shared<SpriteBatch>(L"Data/character01.png", 10, DirectX::XMFLOAT2(416, 416));
		assert(sprCharcter01 && "Misstake load texture data <sprCharcter01>");
	}
	if (!sprCharcter02)
	{
		sprCharcter02 = std::make_shared<SpriteBatch>(L"Data/character02.png", 10, DirectX::XMFLOAT2(416, 416));
		assert(sprCharcter02 && "Misstake load texture data <sprCharcter02>");
	}
	if (!sprCharcter03)
	{
		sprCharcter03 = std::make_shared<SpriteBatch>(L"Data/character03.png", 10, DirectX::XMFLOAT2(416, 416));
		assert(sprCharcter03 && "Misstake load texture data <sprCharcter03>");
	}
	if (!sprCharcter04)
	{
		sprCharcter04 = std::make_shared<SpriteBatch>(L"Data/character04.png", 10, DirectX::XMFLOAT2(416, 416));
		assert(sprCharcter04 && "Misstake load texture data <sprCharcter04>");
	}
	if (!sprCharcter05)
	{
		sprCharcter05 = std::make_shared<SpriteBatch>(L"Data/character05.png");
		assert(sprCharcter05 && "Misstake load texture data <sprCharcter05>");
	}
	if (!sprCharcter06)
	{
		sprCharcter06 = std::make_shared<SpriteBatch>(L"Data/character06.png");
		assert(sprCharcter06 && "Misstake load texture data <sprCharcter06>");
	}
	if (!sprCharcter07)
	{
		sprCharcter07 = std::make_shared<SpriteBatch>(L"Data/character07.png");
		assert(sprCharcter07 && "Misstake load texture data <sprCharcter07>");
	}
	if (!sprCharcter08)
	{
		sprCharcter08 = std::make_shared<SpriteBatch>(L"Data/character08.png");
		assert(sprCharcter08 && "Misstake load texture data <sprCharcter08>");
	}

	// Select
	{
		if (!sprSelectCPU)
		{
			sprSelectCPU = std::make_shared<SpriteBatch>(L"Data/Image/Select/modeselect_CPU.png", 10);
			assert(sprSelectCPU && "Misstake load texture data <sprSelectCPU>");
		}
		if (!sprMenuBG)
		{
			sprMenuBG = std::make_shared<SpriteBatch>(L"Data/Image/Select/selectBG.png", 10);
			assert(sprMenuBG && "Misstake load texture data <sprMenuBG>");
		}
		if (!sprMenuBG2)
		{
			sprMenuBG2 = std::make_shared<SpriteBatch>(L"Data/Image/Select/MenuBG2.png", 10);
			assert(sprMenuBG2 && "Misstake load texture data <sprMenuBG2>");
		}
		if (!sprSelectFrameL)
		{
			sprSelectFrameL = std::make_shared<SpriteBatch>(L"Data/Image/Select/SelectFrameL.png", 7, DirectX::XMFLOAT2(600, 600));
			assert(sprSelectFrameL && "Misstake load texture data <sprSelectFrameL>");
		}
		if (!sprSelectFrameM)
		{
			sprSelectFrameM = std::make_shared<SpriteBatch>(L"Data/Image/Select/SelectFrameM.png", 7, DirectX::XMFLOAT2(480, 480));
			assert(sprSelectFrameM && "Misstake load texture data <sprSelectFrameM>");
		}
		if (!sprSelectFrameS)
		{
			sprSelectFrameS = std::make_shared<SpriteBatch>(L"Data/Image/Select/SelectFrameS.png", 5, DirectX::XMFLOAT2(60, 60));
			assert(sprSelectFrameS && "Misstake load texture data <sprSelectFrameS>");
		}
		if (!sprSelectArrow)
		{
			sprSelectArrow = std::make_shared<SpriteBatch>(L"Data/Image/Select/SelectArrow.png", 2, DirectX::XMFLOAT2(54, 138));
			assert(sprSelectArrow && "Misstake load texture data <sprsprSelectArrow>");
		}
	}
	
	if (!sprMiniCharacter)
	{
		sprMiniCharacter = std::make_shared<SpriteBatch>(L"Data/MiniPlayer.png", 10, DirectX::XMFLOAT2(96.0f, 240.0f));
		assert(sprMiniCharacter && "Misstake load texture data <sprMiniCharacter>");
	}
	if (!sprMiniBlock)
	{
		sprMiniBlock = std::make_shared<SpriteBatch>(L"Data/MiniStone.png", 10, DirectX::XMFLOAT2(96.0f, 240.0f));
		assert(sprMiniBlock && "Misstake load texture data <sprMiniBlock>");
	}
	if (!sprPause)
	{
		sprPause = std::make_shared<SpriteBatch>(L"Data/pause.png", 4);
		assert(sprPause && "Misstake load texture data <sprPause>");
	}
	if (!sprStack)
	{
		sprStack = std::make_shared<SpriteBatch>(L"Data/stock.png", 10, DirectX::XMFLOAT2(396.0f/2, 198.0f));
		assert(sprPause && "Misstake load texture data <sprStack>");
	}
	if (!sprStackNum)
	{
		sprStackNum = std::make_shared<SpriteBatch>(L"Data/stock_number.png", 10, DirectX::XMFLOAT2(36.0f, 60.0f));
		assert(sprStackNum && "Misstake load texture data <sprStackNum>");
	}
}

std::shared_ptr<SpriteBatch>& Resource::GetSpriteData(Resource::Texture _textureName)
{
	std::shared_ptr<SpriteBatch> dummy = nullptr;
	switch (_textureName)
	{
	case Texture::Title:
		return sprTitle;				// break;
	case Texture::TitleBack:
		return sprTitleBack;			// break;
	case Texture::Select:
		return sprSelect;				// break;
	case Texture::SelectBack:
		return sprSelectBack;			// break;
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
	case Texture::RedFrame:
		return sprRedFrame;				// break;
	case Texture::BlueFrame:
		return sprBlueFrame;			// break;
	case Texture::UI:
		return sprUI;					// break;
	case Texture::Block:
		return sprBlock;				// break;
	case Texture::Character01:
		return sprCharcter01;			// break;
	case Texture::Character02:
		return sprCharcter02;			// break;
	case Texture::Character03:
		return sprCharcter03;			// break;
	case Texture::Character04:
		return sprCharcter04;			// break;
case Texture::Character05:
		return sprCharcter05;			// break;
	case Texture::Character06:
		return sprCharcter06;			// break;
	case Texture::Character07:
		return sprCharcter07;			// break;
	case Texture::Character08:
		return sprCharcter08;			// break;
	case Texture::SelectCPU:
		return sprSelectCPU;			// break;
	case Texture::MenuBG:
		return sprMenuBG;				// break;
	case Texture::MenuBG2:
		return sprMenuBG2;				// break;
	case Texture::SelectFrameL:
		return sprSelectFrameL;			// break;
	case Texture::SelectFrameM:
		return sprSelectFrameM;			// break;
	case Texture::SelectFrameS:
		return sprSelectFrameS;			// break;
	case Texture::SelectArrow:
		return sprSelectArrow;			// break;
	case Texture::MiniCharacter:
		return sprMiniCharacter;		// break;
	case Texture::MiniBlock:
		return sprMiniBlock;			// break;
	case Texture::Pause:
		return sprPause;				// break;
	case Texture::StackBlock:
		return sprStack;				// break;
	case Texture::StackNum:
		return sprStackNum;
	default:
		return dummy;
		break;
	}
}
