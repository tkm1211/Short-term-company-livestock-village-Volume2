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
	//std::array<std::array<Block, COLUMN_MAX - 10>, ROW_MAX> sortBlocks; //�Q�[���̔Ֆʂ̂�
	Block sortBlocks[COLUMN_MAX - 10][ROW_MAX];

	std::array<int, 2> waitTime; // �u���b�N�󂵂Ă���`�F�C���J�n�܂ł̃��O([0] = firstChain, [1] = �����R��)


	int chainCount;	// �`�F�C�����𐔂���ϐ�
	int pushingCount;
	int eraseBlockCount;

	int colorMax = 5;


	bool isChainContinued;		// �`�F�C����
	bool isChainAfterPushUp;	// �グ�R����
	bool isPushing;				// �v�b�V���A�b�v��
	bool isPushUpByGauge;		// �Q�[�WMAX�ɂ��v�b�V���A�b�v

public:
	BlockManager(){}
	~BlockManager(){}
	void Init();
	void Uninit();
	void Update();
	void Draw();


	// ��l�v���C�p�̍X�V
	void ProcessOfSingleGame();


	// �u���b�N�̐����n
	void GenerateBlock(int _row, int _column, int _color);
	void SetStartBlock();


	// �u���b�N�̔j��n
	void BreakBlock(int _row, int _column);
	void BreakBlock(int _elementNum);
	void CheckDownBlock();


	// �u���b�N�̃\�[�g�֐�
	void SetSortBlocks();


	// �u���b�N�̃R���{�n
	void RagisterChainBlock();
	bool RagisterUpComboBlock();


	// �u���b�N��Push Up�n
	void PreparationPushUp();
	void MovePushUpBoard();


	// �e�v���Z�X�֐�
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
	// �֗��֐�
	/*---------------------------*/
	// �u���b�N�̃T�[�`�n
	bool SearchBlock(int _row, int _column, Block** _ans);
	bool SearchBlockNum(int _row, int _column, int& _ans);
};

extern BlockManager provisionalBlockManager;