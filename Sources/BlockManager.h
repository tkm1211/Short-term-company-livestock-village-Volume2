#pragma once

#include <memory>
#include <array>

#include "sprite.h"
#include "Block.h"

class BlockManager
{
public:
	static int constexpr ROW_MAX = 6;
	static int constexpr COLUMN_MAX = 9 + 9 + 1; // �Ֆ� + ���ז�����ɏ����ʕ� + ���̎��ɏo�Ă���u���b�N
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
	//std::array<std::array<Block, COLUMN_MAX - 10>, ROW_MAX> sortBlocks; //�Q�[���̔Ֆʂ̂�
	Block sortBlocks[COLUMN_MAX - 10][ROW_MAX];

	std::array<int, 2> waitTime; // �u���b�N�󂵂Ă���`�F�C���J�n�܂ł̃��O([0] = firstChain, [1] = �����R��)


	int chainCount;	// �`�F�C�����𐔂���ϐ�
	int pushingCount;
	int eraseBlockCount;

	int colorMax = 5;

	// Relation of Obstacle
	int obstacleNum;		// ���ז��u���b�N�̐�
	int obstacleKeepTime;	// ���ז����~��܂ł̃L�[�v����
	int fallObstacleNum;	// �����邨�ז��̐�

	bool isChainContinued;		// �`�F�C����
	bool isChainAfterPushUp;	// �グ�R����
	bool isPushing;				// �v�b�V���A�b�v��
	bool isPushUpByGauge;		// �Q�[�WMAX�ɂ��v�b�V���A�b�v
	
	bool isObstacleKeeping;		// ���ז����L�[�v��
	bool fallObstacleNow;		// ���ז����~���Ă���Œ���
		

public:
	BlockManager(){}
	~BlockManager(){}
	void Init(int _pn = 0);
	void Uninit();
	void Update(int _pn = 0);
	void Draw();
	void DrawOfSingle();
	void DrawOfMulti(int _pn);


	// ��l�v���C�p�̍X�V
	void ProcessOfSingleGame();
	// ��l�v���C�p�̍X�V
	void ProcessOfMultiGame(int _pn);


	// �u���b�N�̐����n
	void GenerateBlock(int _row, int _column, int _color, bool _isFall = true);
	void SetStartBlock();


	// �u���b�N�̔j��n
	bool BreakBlock(int _row, int _column);
	void BreakBlock(int _elementNum);
	void CheckDownBlock();


	// �u���b�N�̃\�[�g�֐�
	void SetSortBlocks();


	// �u���b�N�̃R���{�n
	void RagisterChainBlock(int _pn);
	bool RagisterUpComboBlock();


	// �u���b�N��Push Up�n
	void PreparationPushUp();
	void MovePushUpBoard(int _pn);

	
	// ���ז��u���b�N�n
	void AttackForOppenent(int _pn);
	void SetObstacleWhenCountReaches(int _pn);
	int  SetNumOfObstacle(int _num);
	void SetFallObstacle(int _fallObstacleNum, int _pn);
	void CheckDownObstacle();
	void AddObstacleByBreakNum(int _pn);


	// �e�v���Z�X�֐�
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
	std::array<Block, BLOCK_NUM_MAX> GetBlocks() { return blocks; } // 2020/03/19 : TAKUMA�ǉ�

	int GetFallObstacleNum() {return fallObstacleNum;}

	/*---------------------------*/
	// �֗��֐�
	/*---------------------------*/
	// �u���b�N�̃T�[�`�n
	bool SearchBlock(int _row, int _column, Block** _ans);
	bool SearchBlockNum(int _row, int _column, int& _ans);

	static void UseImGui();
};

extern BlockManager provisionalBlockManager;
extern std::array<BlockManager, 2> regularBlockManager;