#include "BG.h"

#include "UI.h"
#include "Resource.h"
/*--------------------------------------*/
//	Global area
/*--------------------------------------*/

// unknown


/*--------------------------------------*/
//	Menber function
/*--------------------------------------*/
void BG::Init()
{
	sprBG = RESOURCE->GetSpriteData(Resource::Texture::BG);
	sprPlayerBack = RESOURCE->GetSpriteData(Resource::Texture::PlayerBack);
	sprGrid = RESOURCE->GetSpriteData(Resource::Texture::Grid);
}

void BG::Uninit()
{

}

void BG::Update()
{

}

void BG::Draw()
{
	//Not using.
	// Draw BackGround
	sprBG->Begin();
	sprBG->Draw(0, 0, 1980, 1080, 0.0f);
	sprBG->End();

	// Draw Player Back
	sprPlayerBack->Begin();
	sprPlayerBack->Draw(0, 270, 618, 540, 0.0f);
	sprPlayerBack->End();

	// Draw Grid Line
	sprGrid->Begin();
	sprGrid->Draw(615, 12, 690, 1032, 0.0f);
	sprGrid->End();
}

void BG::DrawOfSingle()
{
	// Draw BackGround
	sprBG->Begin();
	sprBG->Draw(0, 0, 1980, 1080, 0.0f);
	sprBG->End();

	// Draw Player Back
	sprPlayerBack->Begin();
	sprPlayerBack->Draw(0, 270, 618, 540, 0.0f);
	sprPlayerBack->End();

	// Draw Grid Line
	sprGrid->Begin();
	sprGrid->Draw(615, 12, 690, 1032, 0.0f);
	sprGrid->End();
}

void BG::DrawOfMulti()
{
	// Draw BackGround
	sprBG->Begin();
	sprBG->Draw(0, 0, 1980, 1080, 0.0f);
	sprBG->End();

	// Draw Grid line.
	sprGrid->Begin();
	sprGrid->Draw(GameUI::MULTIPLAY_ONE_ORIJIN_X, 12, 690, 1032, 0.0f);
	sprGrid->Draw(GameUI::MULTIPLAY_TWO_ORIJIN_X, 12, 690, 1032, 0.0f);
	sprGrid->End();
}
