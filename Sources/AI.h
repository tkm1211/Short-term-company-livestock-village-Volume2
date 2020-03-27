#pragma once


enum class AIStatus
{
	Change, // ��ԃ`�F���W
	Wait,   // �ҋ@
	Move,   // �s�b�P���ړ�
	Break,  // �u���b�N�j��
};

enum class AIInput
{
	BreakInput, // �u���b�N�j��
	UpInput,    // ��ړ�
	DownInput,  // ���ړ�
	RightInput, // �E�ړ�
	LeftInput,  // ���ړ�
};


class AI
{
protected:
	static const int flgInit    = 0x0000;
	static const int breakInput = 0x0001;
	static const int upInput    = 0x0002;
	static const int downInput  = 0x0004;
	static const int rightInput = 0x0008;
	static const int leftInput  = 0x0010;

	static const int maxWaitTimer = 30;

protected:
	// AI�̏��
	AIStatus aiStatus;
	AIStatus debugAiStatus;

	// Input
	int inputFlg;

	// ���̏�ԂɈڍs����܂ł̃^�C�}�[
	int nextStatusTimer;

	// Wait
	int waitTimer;

public:
	AI() {}
	virtual ~AI() {}

public:
	virtual void Init() = 0;
	virtual void Update(int row, int column) = 0;

protected:
	virtual void ChangeAIStatus() = 0;
	virtual void WaitUpdate() = 0;
	virtual int  CheckMove() = 0;
	virtual void MoveUpdate() = 0;
	virtual void BreakUpdate() = 0;

public:
	AIStatus GetAIStatus();
	AIStatus GetDebugAIStatus();
	bool GetInput(AIInput _input);
};