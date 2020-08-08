#pragma once

#include <array>
#include <memory>
#include <sprite.h>

struct Particle
{
	DirectX::XMFLOAT2 pos;
	DirectX::XMFLOAT2 velocity;

	float	angle;
	float	alpha;
	float	maxTime;
	int		timer;
	int		particleType;
	int		color;

	bool	isEnable;
};

class Effect
{
public:
	struct Smoke
	{
		DirectX::XMFLOAT2 pos;
		int anmFrm;
		int anmCnt;
		bool isEnable;
	};

	struct MiniChar
	{
		DirectX::XMFLOAT2 pos;
		int anmFrm;
		int anmCnt;
		bool isEnable;
	};

	struct MiniBlock
	{
		DirectX::XMFLOAT2 pos;
		DirectX::XMFLOAT2 firstPos;
		int anmFrm;
		int anmCnt;
		int flyingCnt;
		int attackerNum;
		
		bool isEnable;
		bool nowFlying;
		bool nowFalling;

	};

private:
	static constexpr int PARTICLE_MAX = 1000;
	static constexpr int SMOKE_S_MAX = 6;
	static constexpr float FORCE_OF_MINIBLOCK = -15.0;

public:
	static constexpr int BORDER_OF_SMOKEL = 4;

private:
	std::unique_ptr<SpriteBatch> sprShock;
	std::unique_ptr<SpriteBatch> sprDeadLine;
	std::unique_ptr<SpriteBatch> sprParticle;
	std::unique_ptr<SpriteBatch> sprSmokeS;
	std::unique_ptr<SpriteBatch> sprSmokeL;
	std::shared_ptr<SpriteBatch> sprMiniChar;
	std::shared_ptr<SpriteBatch> sprMiniBlock;
	std::shared_ptr<SpriteBatch> sprStack;

	float deadLineAlpha;

public:
#pragma region DeadLineån
	bool	isAleat;
	bool	isLastAleat;
	bool	isPlus;
	bool	isDisappearance;
#pragma endregion

#pragma region Particleån
	std::array<Particle, PARTICLE_MAX> particle;
#pragma endregion

#pragma region Smokeån
	std::array<Smoke, SMOKE_S_MAX> smokeS;
	Smoke smokeL;
	bool isShowSmoke;
	bool isShowSmokeL;
	bool isReadySmokeL;
#pragma endregion

#pragma region MiniCharån
	MiniChar miniChar;
	bool isShowMiniChar;
#pragma endregion

#pragma region MiniBlockån
	std::array<MiniBlock, 10> miniBlock;
	bool isShowMiniBlock;
	bool isShowStackNum;
	int stackObstacleNum;
#pragma endregion 

public:
	void Init(int);
	void Uninit(int);
	void Update(int);
	void Draw(int);
	void DrawOfSingle();
	void DrawOfMulti(int _pn);

	void GenerateParticle(int _row, int _column, int _color);
	void MoveParticle(int);

#pragma region Smokeån
	bool JudgeIsShowingSmoke();
	void UpdateOfSmoke();
	void UpdateOfSmokeL(int _pn);
	void DrawSmokeS(int _pn);
	void DrawSmokeL(int _pn);
	void SetIsShowSmoke(bool n) { isShowSmoke = n; }
	bool GetIsShowSmoke() { return isShowSmoke; }
	void GenerateSmoke(int _row, int _column);
	void GenerateSmokeL(int _pn);
#pragma endregion

#pragma region MiniCharån
	void GenerateMiniChar(int _row, int _column, int _pn);
	void UpdateOfMiniChar();
	void DrawMiniChar(int _pn);
#pragma endregion

#pragma region MiniBlockån
	void GenerateMiniBlock(int _row, int _column, int _pn);
	void UpdateOfMiniBlock(int _pn);
	void DrawMiniBlock(int _pn);
	void DrawStackObstacle(int _pn);
	void SetStackObstacleNum(int _pn);
#pragma endregion

#pragma region Setter
	void SetIsReadySmokeL(bool _n) { isReadySmokeL = _n; }
	void SetIsShowSmokeL(bool _n) { isShowSmokeL = _n; }
	void SetIsShowMiniChar(bool _n) { isShowMiniChar = _n; }
	void SetIsShowMiniBlock(bool _n) { isShowMiniBlock = _n; }
	void SetStackObstacleNumber(int _n) { stackObstacleNum = _n; }
#pragma endregion

#pragma region Getter
	bool GetIsReadySmokeL() { return isReadySmokeL; }
	bool GetIsShowSmokeL() { return isShowSmokeL; }
	bool GetIsShowMiniChar() { return isShowMiniChar; }
	bool GetIsShowMiniBlock() { return isShowMiniBlock; }
#pragma endregion


	static void UseImGui();
};

extern std::array<Effect, 2> regularEffects;