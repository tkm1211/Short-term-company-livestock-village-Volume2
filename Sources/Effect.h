#pragma once

#include <memory>
#include <sprite.h>
#include "Singleton.h"

#define PARTICLE_MAX 1000
struct Particle
{
	DirectX::XMFLOAT2 pos;
	DirectX::XMFLOAT2 velocity;
	float angle;
	int timer;
	int particleType;
	int color;
	float alpha;
	float maxTime;
	bool isEnable;
};

class Effect// : public Singleton<Effect>
{
	std::unique_ptr<SpriteBatch> sprShock;
	std::unique_ptr<SpriteBatch> sprDeadLine;


	float alpha;
public:
	// DeadLine--
	bool isAleat;
	bool isLastAleat;
	bool isPlus;
	bool isDisappearance;
	//-----------

	//---------------
	// Particle
	//---------------
	std::unique_ptr<SpriteBatch> sprParticle;
	Particle particle[1000];

public:
	void Init(int);
	void Uninit();
	void Update(int);
	void Draw(int);

	void GenerateParticle(int _row, int _column, int _color);
	void MoveParticle(int);
};

//#define EFFECT Effect::getInstance()