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
	static constexpr int MULTI_CORRECTION_Y = 12;

	static constexpr float MULTIPLAY_ONE_ORIJIN_X = 15.0f;
	static constexpr float MULTIPLAY_TWO_ORIJIN_X = 1215.0f;

	static constexpr int GAUGE_DOWN_CNT_MAX = 60;

	static constexpr int CHARACTER_ANIMATION_MAX = 6;

private:
	std::shared_ptr<SpriteBatch> sprGameUI;
	std::shared_ptr<SpriteBatch> sprCharacter;

#pragma region Chainä÷åW
	DirectX::XMFLOAT2 showChainNumPos;
	float	showChainNumFirstPos;
	bool	isShowChainNum;
	int		nowChainNum;
	int		showChainTimer;
	float	showChainAlpha;
#pragma endregion

#pragma region Scoreä∑éZån
	const int			baseScore_tbl[6] = { 10, 20, 50, 90, 140, 200 };
	int					totalScore;
	DirectX::XMFLOAT2	scorePos;
	TexData				scoreBoard;
#pragma endregion

#pragma region NextBlockä÷åW
	std::array<int, 6> nextBlockColors;
	std::array<TexData, 7>	drawBlockStatus;
#pragma endregion

#pragma region Gaugeä÷åW
	TexData meterBack;
	TexData meter;

	float	maxSecond;
	int		interpolationTimeCnt;
	float	second;
	int		gaugeDownCnt;
	float	gaugeTexPosY;
	float	gaugeTexSizeY;
	float	gaugePosY;
	bool	isGaugeMax;
public:
	bool	isTimerStop;
#pragma endregion

#pragma region Speedä÷åW
	int totalTime;
	int minTime;
	int secondTime;
	int speedUpCount;
#pragma endregion

#pragma region Characterä÷åW
	TexData charTex;
	int		charAnimCount;
	int		charAnimFrame;
#pragma endregion

#pragma region Readyä÷åW
	int		readyState;
	int		readyTimer;
	float	readyAlpha;
	float	readyPosY;
#pragma endregion

#pragma region Resultä÷åW
	int rankingScore[3];
	float winLoseAlpha;
	bool  isWinLoseUp;
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
		scorePos{},
		scoreBoard{},
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
		gaugeTexSizeY(0.0f),
		totalTime(0),
		minTime(0),
		secondTime(0),
		speedUpCount(0),
		charAnimCount(0),
		charAnimFrame(0),
		readyState(0),
		readyTimer(0),
		readyAlpha(0.0f),
		readyPosY(0.0f),
		rankingScore{0,0,0},
		isWinLoseUp(false),
		winLoseAlpha(0.0f)
	{}
	~GameUI() {}

	void	Init(int _pn = 0);
	void	Uninit();
	void	Update(int _pn = 0);
	void	Draw();
	void	DrawOfSingle();
	void	DrawOfMulti(int _pn);

#pragma region Chainån
public:
	void	SetShowChainNumInit(DirectX::XMFLOAT2 _pos);
	void	ShowChainNumUpdate();
#pragma endregion

#pragma region Scoreä∑éZ
public:
	void	CalcScore(int _eraseNum, int _chainCount);
#pragma endregion

#pragma region NextBlockån
private:
	void	SetNextBlockColors(int _pn);
#pragma endregion

#pragma region Gaugeån
public:
	void ResetGauge();
private:
	void UpdateOfGauge(int _pn);
	void UpdateGaugeSpeed();
#pragma endregion

#pragma region Speedån
	void UpdateOfTime();
#pragma endregion

#pragma region Characterån
	void UpdateOfCharacter(int _pn);
#pragma endregion

#pragma region Readyån
	void UpdateOfGameReady();
#pragma endregion

public:
#pragma region Fileä«óù
	void FileRead();
	void FileWrite();
	void RankingSort();
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
extern std::array<GameUI, 2> regularGameUI;