#pragma once

#include <memory>
#include <array>

#include "sprite.h"
#include "Block.h"

class BlockManager
{
public:
	static int constexpr ROW_MAX = 6;
	static int constexpr COLUMN_MAX = 9 + 9 + 1; // 盤面 + お邪魔が上に上限一面分 + 下の次に出てくるブロック
	static int constexpr BLOCK_NUM_MAX = ROW_MAX * COLUMN_MAX;

	static int constexpr BOARD_ROW_MAX = 6;
	static int constexpr BOARD_COLUMN_MAX = 9;
	static int constexpr BLOCK_BOARD_MAX = BOARD_ROW_MAX * BOARD_COLUMN_MAX;

	static constexpr int ADJUST = 3;
	static constexpr int SINGLE_CORRECTION_X = 615;
	static constexpr int SINGLE_CORRECTION_Y = 12;

	static constexpr int CHAIN_RAG = 30;

	static constexpr int END_MAX_PAR = 40;
	static constexpr int END_MIN_PAR = 30;
	static constexpr int MAX_PAR = 35;
	static constexpr int MIN_PAR = 30;


public:
	enum Color
	{
		Red,
		LightBlue,
		Yellow,
		Green,
		Orange,
		Blue,
		Purple,
		Obstacle,
	};

	enum State
	{
		Wait,
		Break,
		Chain,
		PushUp,
		PopRowLine,
		CheckUpCombo,
	};
	State status;
	State lastStatus;

private:
	std::shared_ptr<SpriteBatch> sprBlock;

	std::array<Block, BLOCK_NUM_MAX> blocks;
	//std::array<std::array<Block, COLUMN_MAX - 10>, ROW_MAX> sortBlocks; //ゲームの盤面のみ
	Block sortBlocks[COLUMN_MAX - 10][ROW_MAX];

	std::array<int, 2> waitTime; // ブロック壊してからチェイン開始までのラグ([0] = firstChain, [1] = 落ちコン)


	int chainCount;	// チェイン数を数える変数
	int pushingCount;
	int eraseBlockCount;

	int colorMax = 5;


	bool isChainContinued;		// チェイン中
	bool isChainAfterPushUp;	// 上げコン中
	bool isPushing;				// プッシュアップ中
	bool isPushUpByGauge;		// ゲージMAXによりプッシュアップ

public:
	BlockManager(){}
	~BlockManager(){}
	void Init();
	void Uninit();
	void Update();
	void Draw();


	// 一人プレイ用の更新
	void ProcessOfSingleGame();


	// ブロックの生成系
	void GenerateBlock(int _row, int _column, int _color);
	void SetStartBlock();


	// ブロックの破壊系
	void BreakBlock(int _row, int _column);
	void BreakBlock(int _elementNum);
	void CheckDownBlock();


	// ブロックのソート関数
	void SetSortBlocks();


	// ブロックのコンボ系
	void RagisterChainBlock();
	bool RagisterUpComboBlock();


	// ブロックのPush Up系
	void PreparationPushUp();
	void MovePushUpBoard();


	// 各プロセス関数
	void WaitProcess();
	void BreakProcess();
	void ChainProcess();
	void PopRowLineProcess();
	void PushUpProcess();
	void CheckUpComboProcess();

	// Getter and Setter
	State	GetStatus() { return status; }
	void	SetStatus(State _n) { lastStatus = status; status = _n; }
	int		GetColorMax() { return colorMax; }
	bool	GetIsPushUpByGauge() { return isPushUpByGauge; }
	void	SetIsPushUpByGauge(bool _n) { isPushUpByGauge = _n; }
	bool	GetIsPushing() { return isPushing; }

	/*---------------------------*/
	// 便利関数
	/*---------------------------*/
	// ブロックのサーチ系
	bool SearchBlock(int _row, int _column, Block** _ans);
	bool SearchBlockNum(int _row, int _column, int& _ans);
};

extern BlockManager provisionalBlockManager;