#include "BG.h"

#include "UI.h"
#include "Resource.h"
#include "SceneManager.h"
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
	sprBlueFrame = RESOURCE->GetSpriteData(Resource::Texture::BlueFrame);

	if (sceneSelect.JudgeGameMode(SelectGameMode::CPU)) {
		sprRedFrame = RESOURCE->GetSpriteData(Resource::Texture::CPUFrame);
	}
	else {
		sprRedFrame = RESOURCE->GetSpriteData(Resource::Texture::RedFrame);
	}
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

	// Draw Frame
	sprBlueFrame->Begin();
	sprBlueFrame->Draw(GameUI::MULTIPLAY_ONE_ORIJIN_X - 30, 12 - 48, 750, 1128, 0.0f); // グリッド線と外枠の大きさの差分 / 2
	sprBlueFrame->End();
	sprRedFrame->Begin();
	sprRedFrame->Draw(GameUI::MULTIPLAY_TWO_ORIJIN_X - 30, 12 - 48, 750, 1128, 0.0f);
	sprRedFrame->End();

	// Draw Grid line.
	sprGrid->Begin();
	sprGrid->Draw(GameUI::MULTIPLAY_ONE_ORIJIN_X, 12, 690, 1032, 0.0f);
	sprGrid->Draw(GameUI::MULTIPLAY_TWO_ORIJIN_X, 12, 690, 1032, 0.0f);
	sprGrid->End();
}
