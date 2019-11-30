#include "SceneTitle.h"

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
void SceneTitle::Init()
{
	sprTitle = RESOURCE->GetSpriteData(Resource::Texture::Title);
}

void SceneTitle::Update()
{

	if (PRODUCTION->CheckFlag(GO_PLAYERSTILE))
		PRODUCTION->CSOH(SCENE_MANAGER->SCENE_SELECT);
	if (PRODUCTION->CheckFlag(GO_TITLE))
		PRODUCTION->CSOH(SCENE_MANAGER->TITLE);

	if (PRODUCTION->CheckFlag(GO_PLAYERSTILE) || PRODUCTION->CheckFlag(GO_TITLE)) return;

	if (GetAsyncKeyState('N') & 1 || pad[0].bAt)
	{
		PRODUCTION->SetOn(GO_PLAYERSTILE);
		PRODUCTION->Start();
	}
}

void SceneTitle::Draw()
{
	sprTitle->Begin();
	sprTitle->Draw(0, 0, 1184, 544, 0, 922, 1184, 544, 0, 0, 0.0f);
	sprTitle->End();

	if (PRODUCTION->CheckFlag(GO_PLAYERSTILE) || PRODUCTION->CheckFlag(GO_TITLE))
	{
		PRODUCTION->Draw();
	}

}

void SceneTitle::Uninit()
{
}

void SceneTitle::Animation()
{
}

