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
	static constexpr int PARTICLE_MAX = 1000;

	std::unique_ptr<SpriteBatch> sprShock;
	std::unique_ptr<SpriteBatch> sprDeadLine;
	std::unique_ptr<SpriteBatch> sprParticle;

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

public:
	void Init(int);
	void Uninit(int);
	void Update(int);
	void Draw(int);

	void GenerateParticle(int _row, int _column, int _color);
	void MoveParticle(int);
};

extern std::array<Effect, 2> regularEffects;