#pragma once

#include "Selector.h"
#include <memory>
#include <sprite.h>


class PlayStyleSelector : public ISelector
{
private:
	const int styleNumMin = 0;
	const int styleNumMax = 4;
	const int styleNumDefault = 0;

	const float frameMMoveX = 17.3f; // 519.0f / 30.0f
	const float frameLToMMoveX = 19.2f; // (-424.0f - 154.0f) / 30.0f
	const float frameLToMMoveY = 4.0f; // (415.0f - 295.0f) / 30.0f
	const float frameMToLMoveX = 23.2f; // (851.0f - 154.0f) / 30.0f
	const float frameMToLMoveY = 4.0f; // (415.0f - 295.0f) / 30.0f
	const float frameMScale = 4.0f; // 120.0f / 30.0f
	const int moveCntMax = 30;
	const int endMoveCntMax = 60;

	const float endMoveSpeedY = 15.0f;
	const float frameSEndMoveSpeedY = 3.73333f;

private:
	std::shared_ptr<SpriteBatch> sprSelect;
	std::shared_ptr<SpriteBatch> sprSelectFrameL;
	std::shared_ptr<SpriteBatch> sprSelectFrameM;
	std::shared_ptr<SpriteBatch> sprSelectFrameS;
	std::shared_ptr<SpriteBatch> sprSelectArrow;

	DirectX::XMFLOAT2 frameLPos;
	DirectX::XMFLOAT2 frameMPos[5];
	DirectX::XMFLOAT2 frameSPos[5];
	DirectX::XMFLOAT2 arrowPos[2];
	DirectX::XMFLOAT2 frameMSize[5];

	int styleNum;
	bool selectMoveRight;
	bool selectMoveLeft;
	int moveCnt;

	int endMoveCnt;

	SelectMode previousSelectorMode;

public:
	PlayStyleSelector() {}
	~PlayStyleSelector() {}

public:
	void Construct() override;
	void Init(ISelector* selector) override;
	bool Update() override;
	void Draw() override;

private:
	void BeginMove();
	bool EndMove();
	void Choice();
	void Operation();
};