#include "SceneTitle.h"

#include <input_device.h>
#include "Resource.h"
#include "SceneManager.h"
#include "Production.h"
#include "Sound.h"

/*--------------------------------------*/
//	Global area
/*--------------------------------------*/

// unknown


/*--------------------------------------*/
//	Menber function
/*--------------------------------------*/
void SceneTitle::Init()
{
	sprLogo = RESOURCE->GetSpriteData(Resource::Texture::Title);
	sprTitleBack = RESOURCE->GetSpriteData(Resource::Texture::TitleBack);
	sprChar = RESOURCE->GetSpriteData(Resource::Texture::Character01);

	logoData.pos =		DirectX::XMFLOAT2(0.0f, 0.0f);
	logoData.size =		DirectX::XMFLOAT2(1184.0f, 544.0f);
	logoData.tex =		DirectX::XMFLOAT2(0.0f, 922.0f);

	titleBackData.pos = DirectX::XMFLOAT2(0.0f, 0.0f);
	titleBackData.size = DirectX::XMFLOAT2(1920.0f, 1080.0f);
	titleBackData.tex = DirectX::XMFLOAT2(0.0f, 0.0f);

	charData.pos = DirectX::XMFLOAT2(2050.0f, -50.0f);
	charData.size = DirectX::XMFLOAT2(-416.0f*3, 416.0f*3);
	charData.tex = DirectX::XMFLOAT2(0.0f, 0.0f);

	bigBlockData.pos = DirectX::XMFLOAT2(0.0f, 696.0f);
	bigBlockData.size = DirectX::XMFLOAT2(1920.0, 384.0f);
	bigBlockData.tex = DirectX::XMFLOAT2(0.0f, 0.0f);

	smallBlockData.pos = DirectX::XMFLOAT2(0.0f, 696.0f);
	smallBlockData.size = DirectX::XMFLOAT2(1920.0f, 384.0f);
	smallBlockData.tex = DirectX::XMFLOAT2(0.0f, 384.0f);

	pressATextData.pos = DirectX::XMFLOAT2(432.0f, 600.0f);
	pressATextData.size = DirectX::XMFLOAT2(1056.0, 154.0f);
	pressATextData.tex = DirectX::XMFLOAT2(0.0f, 768.0f);


	charAnmFrm = 0;
	charAnmCnt = 0;

	pressATextAlpha = 1.0f;

	// Play BGM
	pAudio->Play(Sound::Get()->bgmHandle[Sound::BGM::TITLE].get(), true);
}

void SceneTitle::Update()
{

	if (PRODUCTION->CheckFlag(GO_PLAYERSTILE))
		PRODUCTION->CSOH(SCENE_MANAGER->SCENE_SELECT);
	if (PRODUCTION->CheckFlag(GO_TITLE))
		PRODUCTION->CSOH(SCENE_MANAGER->TITLE);

	if (PRODUCTION->CheckFlag(GO_PLAYERSTILE) || PRODUCTION->CheckFlag(GO_TITLE)) return;



#if defined(DEBUG) | defined(_DEBUG)
	if (GetAsyncKeyState('N') & 1 || pad[0].bAt)
#else
	if (pad[0].bAt)
#endif
	{
		PRODUCTION->SetOn(GO_PLAYERSTILE);
		PRODUCTION->Start();
		pAudio->Play(Sound::Get()->seHandle[Sound::SE::OK].get());
	}

	Animation();
}

void SceneTitle::Draw()
{
	// 背景
	{
		sprTitleBack->Begin();
		sprTitleBack->Draw(titleBackData);
		sprTitleBack->End();
	}

	// タイトルオブジェクト(キャラクターより後方)
	{
		sprLogo->Begin();
		// タイトルロゴ
		sprLogo->Draw(logoData);
		// Big block
		sprLogo->Draw(bigBlockData);
		sprLogo->End();
	}

	// キャラクター
	{
		sprChar->Begin();
		sprChar->Draw(
			charData.pos.x, charData.pos.y,
			charData.size.x, charData.size.y,
			0.0f, 1.0f, 1.0f, 1.0f, 1.0f, charAnmFrm);
		sprChar->End();
	}

	// タイトルオブジェクト(キャラクターより前方)
	{
		sprLogo->Begin();
		// small block
		sprLogo->Draw(smallBlockData);
		// Press A
		sprLogo->Draw(pressATextData, 0.0f, 1.0f, 1.0f, 1.0f, pressATextAlpha);
		sprLogo->End();
	}

	if (PRODUCTION->CheckFlag(GO_PLAYERSTILE) || PRODUCTION->CheckFlag(GO_TITLE))
	{
		PRODUCTION->Draw();
	}

}

void SceneTitle::Uninit()
{
	// Stop BGM
	pAudio->Stop(Sound::Get()->bgmHandle[Sound::BGM::TITLE].get());
	pAudio->DeleteSourceVoice(Sound::Get()->bgmHandle[Sound::BGM::TITLE].get());
}

void SceneTitle::Animation()
{
	// Move Logo
	{
		static float deg = 0.0f;
		float t = DirectX::XMConvertToRadians(deg);

		int moveHeight = 25.0f;
		logoData.pos.y = sinf(t) * moveHeight;

		deg += 3.0f;
		if (deg >= 180.0f)
		{
			deg *= -1;
		}
	}

	// Animation Character
	{
		if (++charAnmCnt % 8 == 0)
		{
			if (++charAnmFrm >= 6)
			{
				charAnmCnt = 0;
				charAnmFrm = 0;
			}
		}
	}

	// Changing Alpha
	{
		static bool isPlus = false;

		switch (isPlus)
		{
		case true:
			pressATextAlpha += 0.01f;
			if (pressATextAlpha >= 1.0f)
			{
				pressATextAlpha = 1.0f;
				isPlus = false;
			}
			break;

		case false:
			pressATextAlpha -= 0.01f;
			if (pressATextAlpha <= 0.5f)
			{
				pressATextAlpha = 0.5f;
				isPlus = true;
			}
			break;
		}
	}
}

