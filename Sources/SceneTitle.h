#pragma once

#include "Resource.h"
#include "Scene.h"

class SceneTitle : public BaseScene
{
private:
	

private:
	std::shared_ptr<SpriteBatch> sprLogo;
	std::shared_ptr<SpriteBatch> sprTitleBack;
	std::shared_ptr<SpriteBatch> sprChar;

	TexData logoData;
	TexData titleBackData;
	TexData charData;
	TexData bigBlockData;
	TexData smallBlockData;
	TexData pressATextData;

	int		charAnmFrm;
	int		charAnmCnt;

	float	pressATextAlpha;

public:
	SceneTitle() { /*Init();*/ }
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void Animation();
};