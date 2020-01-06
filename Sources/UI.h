#pragma once

#include <memory>
#include <sprite.h>

class GameUI
{
public:
	static constexpr int ADJUST = 3;
	static constexpr int SINGLE_CORRECTION_X = 615;
	static constexpr int SINGLE_CORRECTION_Y = 12;

private:
	std::shared_ptr<SpriteBatch> sprGameUI;

	// Chain関係の変数
	DirectX::XMFLOAT2 showChainNumPos;
	float	showChainNumFirstPos;
	bool	isShowChainNum;
	int		nowChainNum;
	int		showChainTimer;
	float	showChainAlpha;


	// スコア換算系の変数
	const int	baseScore_tbl[6] = { 10, 20, 50, 90, 140, 200 };
	int			totalScore;

public:
	GameUI():
		showChainNumPos(DirectX::XMFLOAT2(0.0f,0.0f)),
		isShowChainNum(false),
		showChainNumFirstPos(0.0f),
		showChainAlpha(0.0f),
		showChainTimer(0),
		nowChainNum(0),
		totalScore(0)
	{}

	~GameUI() {}


	void	Init();
	void	Uninit();
	void	Update();
	void	Draw();

	// チェイン系
	void	SetShowChainNumInit(DirectX::XMFLOAT2 _pos);
	void	ShowChainNumUpdate();

	// スコア換算系
	void	CalcScore(int _eraseNum, int _chainCount);

	// Getter and Setter 
	bool				GetIsShowChainNum() { return isShowChainNum; }
	void				SetIsShowChainNum(bool _n) { isShowChainNum = _n; }
	DirectX::XMFLOAT2	GetShowChainNumPos() { return showChainNumPos; }
	void				SetShowChainNumPos(DirectX::XMFLOAT2 _p) { showChainNumPos = _p; }
	int					GetNowChainNum() { return nowChainNum; }
	void				SetNowChainNum(int _n) { nowChainNum = _n; }
};

extern GameUI provisionalGameUI;