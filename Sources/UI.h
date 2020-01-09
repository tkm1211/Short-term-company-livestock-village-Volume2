#pragma once

#include <array>
#include <memory>
#include <sprite.h>

class GameUI
{
public:
	static constexpr int ADJUST = 3;
	static constexpr int SINGLE_CORRECTION_X = 615;
	static constexpr int SINGLE_CORRECTION_Y = 12;
	static constexpr int GAUGE_DOWN_CNT_MAX = 60;

private:
	std::shared_ptr<SpriteBatch> sprGameUI;

#pragma region Chain�֌W
	DirectX::XMFLOAT2 showChainNumPos;
	float	showChainNumFirstPos;
	bool	isShowChainNum;
	int		nowChainNum;
	int		showChainTimer;
	float	showChainAlpha;
#pragma endregion

#pragma region Score���Z�n
	const int	baseScore_tbl[6] = { 10, 20, 50, 90, 140, 200 };
	int			totalScore;
#pragma endregion

#pragma region NextBlock�֌W
	std::array<int, 6> nextBlockColors;
	std::array<TexData, 7>	drawBlockStatus;
#pragma endregion

#pragma region Gauge�֌W
	TexData meterBack;
	TexData meter;

	float	maxSecond;
	int		interpolationTimeCnt;
	float	second;
	int		gaugeDownCnt;
	float	gaugeTexPosY;
	float	gaugeTexSizeY;
	float	gaugePosY;

	bool	isTimerStop;
	bool	isGaugeMax;
#pragma endregion


public:
	GameUI():
		showChainNumPos(DirectX::XMFLOAT2(0.0f,0.0f)),
		isShowChainNum(false),
		showChainNumFirstPos(0.0f),
		showChainAlpha(0.0f),
		showChainTimer(0),
		nowChainNum(0),
		totalScore(0),
		nextBlockColors{},
		drawBlockStatus{},
		meterBack{},
		meter{},
		maxSecond(0.0f),
		interpolationTimeCnt(0),
		second(0.0f),
		isTimerStop(false),
		isGaugeMax(false),
		gaugeDownCnt(0),
		gaugePosY(0.0f),
		gaugeTexPosY(0.0f),
		gaugeTexSizeY(0.0f)
	{}
	~GameUI() {}

	void	Init();
	void	Uninit();
	void	Update();
	void	Draw();

#pragma region Chain�n
public:
	void	SetShowChainNumInit(DirectX::XMFLOAT2 _pos);
	void	ShowChainNumUpdate();
#pragma endregion

#pragma region Score���Z
public:
	void	CalcScore(int _eraseNum, int _chainCount);
#pragma endregion

#pragma region NextBlock�n
private:
	void	SetNextBlockColors();
#pragma endregion

#pragma region Gauge�n
public:
	void ResetGauge();
private:
	void UpdateOfGauge();
#pragma endregion

#pragma region Getter and Setter
public:
	// Getter and Setter 
	bool				GetIsShowChainNum() { return isShowChainNum; }
	void				SetIsShowChainNum(bool _n) { isShowChainNum = _n; }
	DirectX::XMFLOAT2	GetShowChainNumPos() { return showChainNumPos; }
	void				SetShowChainNumPos(DirectX::XMFLOAT2 _p) { showChainNumPos = _p; }
	int					GetNowChainNum() { return nowChainNum; }
	void				SetNowChainNum(int _n) { nowChainNum = _n; }
	bool				GetIsTimerStop() { return isTimerStop; }
	void				SetIsTimerStop(bool _n) { isTimerStop = _n; }
	bool				GetIsGaugeMax() { return isGaugeMax; }
	void				SetIsGaugeMax(bool _n) { isGaugeMax = _n; }
#pragma endregion
};

extern GameUI provisionalGameUI;