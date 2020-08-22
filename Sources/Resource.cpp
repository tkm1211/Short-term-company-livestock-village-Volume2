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
		sprTitle = std::make_shared<SpriteBatch>(L"Data/Image/UI/title.png", 10);
		assert(sprTitle && "Misstake load texture data <sprTitle>");
	}
	if (!sprTitleBack)
	{
		sprTitleBack = std::make_shared<SpriteBatch>(L"Data/Image/BG/titleBG.png");
		assert(sprTitleBack && "Misstake load texture data <sprTitleBack>");
	}
	if (!sprSelect)
	{
		sprSelect = std::make_shared<SpriteBatch>(L"Data/Image/UI/modeselect.png", 10);
		assert(sprSelect && "Misstake load texture data <sprSelect>");
	}
	if (!sprSelectBack)
	{
		sprSelectBack = std::make_shared<SpriteBatch>(L"Data/Image/BG/selectBG.png");
		assert(sprSelectBack && "Mistake load texture data <sprSelectBack>");
	}
	if (!sprBG)
	{
		sprBG = std::make_shared<SpriteBatch>(L"Data/Image/BG/BG.png");
		assert(sprBG && "Misstake load texture data <sprBG>");
	}
	if (!sprCurtain)
	{
		sprCurtain = std::make_shared<SpriteBatch>(L"Data/Image/UI/scenechangeL.png");
		assert(sprCurtain && "Misstake load texture data <sprCurtain>");
	}
	if (!sprDescriptionRule)
	{
		sprDescriptionRule = std::make_shared<SpriteBatch>(L"Data/Image/UI/rule.png", 10);
		assert(sprDescriptionRule && "Misstake load texture data <sprDescriptionRule>");
	}
	if (!sprPlayerBack)
	{
		sprPlayerBack = std::make_shared<SpriteBatch>(L"Data/Image/BG/gameBG.png");
		assert(sprPlayerBack && "Misstake load texture data <sprPlayerBack>");
	}
	if (!sprGrid)
	{
		sprGrid = std::make_shared<SpriteBatch>(L"Data/Image/BG/frame.png", 2);
		assert(sprGrid && "Misstake load texture data <sprGrid>");
	}
	if (!sprBlueFrame)
	{
		sprBlueFrame = std::make_shared<SpriteBatch>(L"Data/Image/BG/frame1p.png");
		assert(sprBlueFrame && "Misstake load texture data <sprBlueFrame>");
	}
	if (!sprRedFrame)
	{
		sprRedFrame = std::make_shared<SpriteBatch>(L"Data/Image/BG/frame2p.png");
		assert(sprRedFrame && "Misstake load texture data <sprRedFrame>");
	}
	if (!sprUI)
	{
		sprUI = std::make_shared<SpriteBatch>(L"Data/Image/UI/UI.png", 100, DirectX::XMFLOAT2(114, 114));
		assert(sprUI && "Misstake load texture data <sprUI>");
	}
	if (!sprCPUUI)
	{
		sprCPUUI = std::make_shared<SpriteBatch>(L"Data/Image/UI/UI_CPU.png", 100, DirectX::XMFLOAT2(114, 114));
		assert(sprCPUUI && "Misstake load texture data <sprCPUUI>");
	}
	if (!sprBlock)
	{
		sprBlock = std::make_shared<SpriteBatch>(L"Data/Image/UI/block.png", BlockManager::BLOCK_NUM_MAX, DirectX::XMFLOAT2(114, 114));
		assert(sprBlock && "Misstake load texture data <sprBlock>");
	}
	if (!sprCharcter01)
	{
		sprCharcter01 = std::make_shared<SpriteBatch>(L"Data/Image/Character/character01.png", 10, DirectX::XMFLOAT2(416, 416));
		assert(sprCharcter01 && "Misstake load texture data <sprCharcter01>");
	}
	if (!sprCharcter02)
	{
		sprCharcter02 = std::make_shared<SpriteBatch>(L"Data/Image/Character/character02.png", 10, DirectX::XMFLOAT2(416, 416));
		assert(sprCharcter02 && "Misstake load texture data <sprCharcter02>");
	}
	if (!sprCharcter03)
	{
		sprCharcter03 = std::make_shared<SpriteBatch>(L"Data/Image/Character/character03.png", 10, DirectX::XMFLOAT2(416, 416));
		assert(sprCharcter03 && "Misstake load texture data <sprCharcter03>");
	}
	if (!sprCharcter04)
	{
		sprCharcter04 = std::make_shared<SpriteBatch>(L"Data/Image/Character/character04.png", 10, DirectX::XMFLOAT2(416, 416));
		assert(sprCharcter04 && "Misstake load texture data <sprCharcter04>");
	}
	if (!sprCharcter05)
	{
		sprCharcter05 = std::make_shared<SpriteBatch>(L"Data/Image/Character/character05.png");
		assert(sprCharcter05 && "Misstake load texture data <sprCharcter05>");
	}
	if (!sprCharcter06)
	{
		sprCharcter06 = std::make_shared<SpriteBatch>(L"Data/Image/Character/character06.png");
		assert(sprCharcter06 && "Misstake load texture data <sprCharcter06>");
	}
	if (!sprCharcter07)
	{
		sprCharcter07 = std::make_shared<SpriteBatch>(L"Data/Image/Character/character07.png");
		assert(sprCharcter07 && "Misstake load texture data <sprCharcter07>");
	}
	if (!sprCharcter08)
	{
		sprCharcter08 = std::make_shared<SpriteBatch>(L"Data/Image/Character/character08.png");
		assert(sprCharcter08 && "Misstake load texture data <sprCharcter08>");
	}

	// Select
	{
		if (!sprSelectCPU)
		{
			sprSelectCPU = std::make_shared<SpriteBatch>(L"Data/Image/UI/modeselect_CPU.png", 10);
			assert(sprSelectCPU && "Misstake load texture data <sprSelectCPU>");
		}
		if (!sprMenuBG)
		{
			sprMenuBG = std::make_shared<SpriteBatch>(L"Data/Image/BG/selectBG.png", 10);
			assert(sprMenuBG && "Misstake load texture data <sprMenuBG>");
		}
		if (!sprMenuBG2)
		{
			sprMenuBG2 = std::make_shared<SpriteBatch>(L"Data/Image/BG/MenuBG2.png", 10);
			assert(sprMenuBG2 && "Misstake load texture data <sprMenuBG2>");
		}
		if (!sprSelectFrameL)
		{
			sprSelectFrameL = std::make_shared<SpriteBatch>(L"Data/Image/UI/SelectFrameL.png", 7, DirectX::XMFLOAT2(600, 600));
			assert(sprSelectFrameL && "Misstake load texture data <sprSelectFrameL>");
		}
		if (!sprSelectFrameM)
		{
			sprSelectFrameM = std::make_shared<SpriteBatch>(L"Data/Image/UI/SelectFrameM.png", 7, DirectX::XMFLOAT2(480, 480));
			assert(sprSelectFrameM && "Misstake load texture data <sprSelectFrameM>");
		}
		if (!sprSelectFrameS)
		{
			sprSelectFrameS = std::make_shared<SpriteBatch>(L"Data/Image/UI/SelectFrameS.png", 5, DirectX::XMFLOAT2(60, 60));
			assert(sprSelectFrameS && "Misstake load texture data <sprSelectFrameS>");
		}
		if (!sprSelectArrow)
		{
			sprSelectArrow = std::make_shared<SpriteBatch>(L"Data/Image/UI/SelectArrow.png", 2, DirectX::XMFLOAT2(54, 138));
			assert(sprSelectArrow && "Misstake load texture data <sprsprSelectArrow>");
		}
	}
	
	if (!sprMiniCharacter)
	{
		sprMiniCharacter = std::make_shared<SpriteBatch>(L"Data/Image/Effect/newMiniPlayer.png", 100, DirectX::XMFLOAT2(32.0f, 64.0f));
		//sprMiniCharacter = std::make_shared<SpriteBatch>(L"Data/MiniPlayer.png", 100, DirectX::XMFLOAT2(96.0f, 240.0f));
		assert(sprMiniCharacter && "Misstake load texture data <sprMiniCharacter>");
	}
	if (!sprMiniBlock)
	{
		sprMiniBlock = std::make_shared<SpriteBatch>(L"Data/Image/Effect/MiniStone.png", 10, DirectX::XMFLOAT2(96.0f, 240.0f));
		assert(sprMiniBlock && "Misstake load texture data <sprMiniBlock>");
	}
	if (!sprPause)
	{
		sprPause = std::make_shared<SpriteBatch>(L"Data/Image/UI/pause.png", 4);
		assert(sprPause && "Misstake load texture data <sprPause>");
	}
	if (!sprStack)
	{
		sprStack = std::make_shared<SpriteBatch>(L"Data/Image/UI/stock.png", 10, DirectX::XMFLOAT2(396.0f/2, 198.0f));
		assert(sprPause && "Misstake load texture data <sprStack>");
	}
	if (!sprStackNum)
	{
		sprStackNum = std::make_shared<SpriteBatch>(L"Data/Image/UI/stock_number.png", 10, DirectX::XMFLOAT2(36.0f, 60.0f));
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
	case Texture::CPU_UI:
		return sprCPUUI;				// break;
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
