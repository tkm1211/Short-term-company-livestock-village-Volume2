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

	static constexpr int OBSTACLE_KEEPING_COUNT_MAX = 180;


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

		FallObstacle,
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

	// Relation of Obstacle
	int obstacleNum;		// お邪魔ブロックの数
	int obstacleKeepTime;	// お邪魔が降るまでのキープ時間
	int fallObstacleNum;	// 落ちるお邪魔の数

	bool isChainContinued;		// チェイン中
	bool isChainAfterPushUp;	// 上げコン中
	bool isPushing;				// プッシュアップ中
	bool isPushUpByGauge;		// ゲージMAXによりプッシュアップ
	
	bool isObstacleKeeping;		// お邪魔をキープ中
	bool fallObstacleNow;		// お邪魔が降っている最中か
		

public:
	BlockManager(){}
	~BlockManager(){}
	void Init(int _pn = 0);
	void Uninit();
	void Update(int _pn = 0);
	void Draw();
	void DrawOfSingle();
	void DrawOfMulti(int _pn);


	// 一人プレイ用の更新
	void ProcessOfSingleGame();
	// 二人プレイ用の更新
	void ProcessOfMultiGame(int _pn);


	// ブロックの生成系
	void GenerateBlock(int _row, int _column, int _color, bool _isFall = true);
	void SetStartBlock();


	// ブロックの破壊系
	bool BreakBlock(int _row, int _column);
	void BreakBlock(int _elementNum);
	void CheckDownBlock();


	// ブロックのソート関数
	void SetSortBlocks();


	// ブロックのコンボ系
	void RagisterChainBlock(int _pn);
	bool RagisterUpComboBlock();


	// ブロックのPush Up系
	void PreparationPushUp();
	void MovePushUpBoard(int _pn);

	
	// お邪魔ブロック系
	void AttackForOppenent(int _pn);
	void SetObstacleWhenCountReaches(int _pn);
	int  SetNumOfObstacle(int _num);
	void SetFallObstacle(int _fallObstacleNum, int _pn);
	void CheckDownObstacle();
	void AddObstacleByBreakNum(int _pn);


	// 各プロセス関数
	void WaitProcess();
	void BreakProcess();
	void ChainProcess(int _pn);
	void PopRowLineProcess();
	void PushUpProcess(int _pn);
	void CheckUpComboProcess(int _pn);
	void FallObstacleProcess(int _pn);

	// Getter and Setter
	State	GetStatus() { return status; }
	int		GetColorMax() { return colorMax; }
	bool	GetIsPushUpByGauge() { return isPushUpByGauge; }
	bool	GetIsPushing() { return isPushing; }
	void	SetStatus(State _n) { lastStatus = status; status = _n; }
	void	SetIsPushUpByGauge(bool _n) { isPushUpByGauge = _n; }
	std::array<Block, BLOCK_NUM_MAX> GetBlocks() { return blocks; } // 2020/03/19 : TAKUMA追加

	int GetFallObstacleNum() {return fallObstacleNum;}

	/*---------------------------*/
	// 便利関数
	/*---------------------------*/
	// ブロックのサーチ系
	bool SearchBlock(int _row, int _column, Block** _ans);
	bool SearchBlockNum(int _row, int _column, int& _ans);

	static void UseImGui();
};

extern BlockManager provisionalBlockManager;
extern std::array<BlockManager, 2> regularBlockManager;