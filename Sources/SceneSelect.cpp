#include "SceneSelect.h"

#include <input_device.h>

#include "Resource.h"
#include "SceneManager.h"
#include "Production.h"

/*--------------------------------------*/
//	Global area
/*--------------------------------------*/

// unknown


/*--------------------------------------*/
//	Menber function
/*--------------------------------------*/
void SceneSelect::Init()
{
	sprSelect = RESOURCE->GetSpriteData(Resource::Texture::Select);
}

void SceneSelect::Update()
{
	if (PRODUCTION->CheckFlag(GO_SINGLEGAME))
		PRODUCTION->CSOH(SCENE_MANAGER->SINGLE_GAME);
	if (PRODUCTION->CheckFlag(GO_PLAYERSTILE))
		PRODUCTION->CSOH(SCENE_MANAGER->SCENE_SELECT);


	if (GetAsyncKeyState('N') & 1 || pad[0].bAt)
	{
		PRODUCTION->SetOn(GO_SINGLEGAME);
		PRODUCTION->Start();
	}
}

void SceneSelect::Draw()
{
	sprSelect->Begin();
	sprSelect->Draw(0, 0, 1440, 160, 0, 0, 1440, 160, 0, 0, 0.0f);
	sprSelect->End();

	if (PRODUCTION->CheckFlag(GO_PLAYERSTILE) || PRODUCTION->CheckFlag(GO_SINGLEGAME))
	{
		PRODUCTION->Draw();
	}

}

void SceneSelect::Uninit()
{

}