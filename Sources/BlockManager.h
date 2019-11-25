#pragma once

#include <memory>
#include <vector>
#include <sprite.h>
#include "Singleton.h"
#include "Block.h"

#define ORIJIN_X				615
#define ORIJIN_Y				12
#define TWO_PLAY_ONE_ORIJIN_X	15
#define TWO_PLAY_TWO_ORIJIN_X	1215
#define ADJUST					3

struct NextBlock
{
	int color;
	int par;
};

class BlockManager
{
	enum COLOR
	{
		RED,
		LIGUHT_BLUE,
		YELLOW,
		GREEN,
		ORANGE,
		BLUE,
		PURPLE,
		OBSTACLE,
	};

	enum GAME_RESULT_SINGLE
	{
		START,
		SHAKE,
		BREAK_BLOCK,
		DISPLAY_RESULT,
		DISPLAY_SCORE,
		NEXT_BEHAVIOR,
	};

	enum GAME_RESULT_MALTI
	{
		MULTI_START,
		MULTI_SHAKE,
		MULTI_RAGISTER_BREAK_BLOCK,
		MULTI_BREAK_BOCK,
		MULTI_DISPLAY_RESULT,
		MULTI_NEXT_BEHAVIOR,
	};

public:
	static int constexpr BLOCK_MAX = 54;
	static int constexpr ROW_MAX = 6;
	static int constexpr COLUMN_MAX = 9;

private:
	struct DesFunction
	{
		Block dummyBlock;
		Block sortBlock[9][6];
		Block _block[6];

		int allElement[54];
		int allElementCnt;

		int  _num;
		bool isData;
	};

	DesFunction desFunction;

public:
	// GameResult�̕ϐ�
	int gameResultState;
	int fixBlockData[10][6];
	int breakCount;
	int eraseBlockCount;
	float resultDisplayAlpha;
	float resultDisplayPosX;
	float resultDisplayPosY;
	float resultDisplayPosY2;
	float resultDisplayPosXFirst;
	float resultDisplayPosYFirst;
	float resultDisplayPosXFirst2;
	float resultDisplayPosYFirst2;
	float scoreDisplayAlpha;
	float scoreDisplayPosY;
	float resultDisplayMultiAlpha;
	int resultDisplayTimer;
	bool isInitBreak;
	bool isResultSelectLeft;
	int resultCount;
	int resultCount2;
	int resultCount3;


public:
	std::unique_ptr<SpriteBatch> sprBlock;
	bool pushUp;
	bool pushUpT;
	int pushUpCnt;

	int chainCount;
	int eraseCount;

	// Relation of Obstacle
	int obstacleCount;
	int obstacleFallWaitTime;
	int fallObstacleCount;
	bool obstacleFallWaiting;
	bool nowObstacleFall;

	// Variable of GameResult
	DirectX::XMFLOAT2	shakePos;
	DirectX::XMFLOAT2	shakePos2;
	int					shakeTimer;

public:
	std::vector<Block> block;

	int colorMax;

	int eraseRow;
	int eraseColumn;
	int playerNum;

	bool isBreak;
	bool isChain;

	bool isChainNow;
	int sleep[3];

	bool isPopRowLine;

	bool isTimePushUpBoard;

	bool isLastChain;

	bool _isLastChain;
	int  lastChainCnt;

public:
	void Init(int);
	void Uninit();
	void Update(int);
	void Draw(int);

	void	GenerateBlock(int _row, int _colum, int _color);	// �u���b�N�𐶐�
	void	SetStartBlock();									// �Q�[���X�^�[�g�̃u���b�N�̐���
	void	PushUpBoard(int);										// �����̃u���b�N��1�񉟂��グ��
	void	MoveBoard(int);										// �Ֆʂ������グ��
	void	PopRowLine();										// �ŉ���̃u���b�N�𐶐�����
	void	RagisterFallBlock(int, int, int);					// ��������u���b�N��o�^����
	void	RagisterChainBlock(int);							// �A���ŏ�����u���b�N��o�^����
	void	JudgeUpToOnBlock(int);								// �ŏ�ʂ܂Ńu���b�N�����Ă邩���肷��
	bool	JudgeOverTheCeiling();								// �Q�[���I�[�o�[����
	void	TwoPlayDraw(int);
	void	AttackOfObstacleRelation(int);						// ������܃u���b�N�֌W�̏����i�U�ߎ�j
	void	ReceiveOfObstacleRelation(int);						// ������܃u���b�N�֌W�̏����i�󂯎�j
	void	FallObstacle(int, int);								// ������܃u���b�N�𐶐����ė��Ƃ�
	int		SetNumObstacle(int);
	bool	JudgeIsFallObstacle();								// 
	void	EraseObstacleBlock(int, int, int, bool, bool, bool, bool);

	void	ProcessOfGame(int);
	void	ProcessOfGameResultOnePlayer(int);
	void	ProcessOfGameResultTwoPlayer(int);
};