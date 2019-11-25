#pragma once

#include	"Scene.h"
#include	"sprite.h"
#include	<memory>

class SceneTitle : public BaseScene
{
	std::unique_ptr<SpriteBatch> sprTitleBack;
	std::unique_ptr<SpriteBatch> sprObject;
	std::unique_ptr<SpriteBatch> sprChar;
	std::unique_ptr<SpriteBatch> sprTeamLogo;
	std::unique_ptr<SpriteBatch> sprChange;
	int trgCnt;

	DirectX::XMFLOAT2 rogoPos;
	DirectX::XMFLOAT2 blockPos1;
	DirectX::XMFLOAT2 blockPos2;
	DirectX::XMFLOAT2 characterPos;
	DirectX::XMFLOAT2 textPos;
	DirectX::XMFLOAT2 teamLogoPos;
	float textAlpha;
	int charAnimTime;
	int charAnimFrame;
	int state;
	int teamLogoTimer;
	int animFrame;
	float teamLogoAlpha;

	bool isFade;
	bool isPlayBGM;
	bool isTeamLogoEasing;
	bool isPlayCan;
	bool isplayLogo;
	
	bool isPlus;
	
	/*float vib;
	int __cnt;*/
	
public:
	SceneTitle() {}
	void Init();
	void Uninit();
	void Update();
	void Render();

	void Animation();
};

