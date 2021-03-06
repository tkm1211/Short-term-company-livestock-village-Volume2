#include "CPU.h"
#include "EasyAI.h"
#include "NormalAI.h"
#include "HardAI.h"
#include "Resource.h"
#include "UI.h"
#include "BlockManager.h"
#include "SceneManager.h"
#include "imgui.h"
#include "Sound.h"

CPU regularCPU;


CPU::CPU()
{

}

CPU::~CPU()
{

}

void CPU::Init()
{
	// Init AI
	{
		int powor = sceneSelect.GetCPUPowor();
		switch (powor)
		{
		case 0: // Make EasyAI
			cpuAI = std::make_unique<EasyAI>();
			break;

		case 1: // Make NormalAI
			cpuAI = std::make_unique<NormalAI>();
			break;

		case 2: // Make HardAI
			cpuAI = std::make_unique<HardAI>();
			break;

		default: break;
		}


		cpuAI->Init();
	}

	// Input Data Init
	{
		row = 2;
		column = 4;

		for (int i = 0; i < 4; i++)
		{
			accelerationCount[i] = 0;
		}
	}

	// Sprite Data Init
	{
		sprPickel = RESOURCE->GetSpriteData(Resource::Texture::CPU_UI);
		pos = DirectX::XMFLOAT2(static_cast<float>(row * 114), static_cast<float>(column * 114));
		animFrame = 0;
	}
}

void CPU::UnInit()
{

}

void CPU::Update()
{
	if (sceneSelect.gameMode != SelectGameMode::CPU) return;
	if (sceneSelect.gameMode == SelectGameMode::CPU && sceneCPUGame.GetIsGameReady()) return;

	// Update AI
	if (regularBlockManager[1].status == BlockManager::Wait)
	{
		cpuAI->Update(row, column);
	}

	// Update CPU
	OperateCPU();
	SetBreakBlock();

#if defined(DEBUG) | defined(_DEBUG)
	ImGui::Begin("AI Status");

	switch (cpuAI->GetAIStatus())
	{
	case AIStatus::Change:
		ImGui::Text("Change");
		break;
	case AIStatus::Wait:
		ImGui::Text("Wait");
		break;
	case AIStatus::Move:
		ImGui::Text("Move");
		break;
	case AIStatus::Break:
		ImGui::Text("Break");
		break;
	default: break;
	}

	switch (cpuAI->GetDebugAIStatus())
	{
	case AIStatus::Change:
		ImGui::Text("Change");
		break;
	case AIStatus::Wait:
		ImGui::Text("Wait");
		break;
	case AIStatus::Move:
		ImGui::Text("Move");
		break;
	case AIStatus::Break:
		ImGui::Text("Break");
		break;
	default: break;
	}

	ImGui::End();
#endif
}

void CPU::Draw()
{
	sprPickel->Begin();
	sprPickel->Draw(pos.x + GameUI::ADJUST + GameUI::MULTIPLAY_TWO_ORIJIN_X, pos.y + GameUI::ADJUST + GameUI::MULTI_CORRECTION_Y, 114.0f, 114.0f, 0.0f + animFrame * 114.0f, 114.0f, 114.0f, 114.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	sprPickel->End();
}

void CPU::OperateCPU()
{
	if (cpuAI->GetInput(AIInput::UpInput))
	{
		if (++accelerationCount[0] == 1)
		{
			--column;
			pos.y -= MOVING;
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
		}
		if (accelerationCount[0] >= ACCEL_START)
		{
			if (accelerationCount[0] % ACCEL_MOVE_PER == 0)
			{
				--column;
				pos.y -= MOVING;
				pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
			}
		}
	}
	else
	{
		accelerationCount[0] = 0;
	}

	if (cpuAI->GetInput(AIInput::RightInput))
	{
		if (++accelerationCount[1] == 1)
		{
			++row;
			pos.x += MOVING;
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
		}
		if (accelerationCount[1] >= ACCEL_START)
		{
			if (accelerationCount[1] % ACCEL_MOVE_PER == 0)
			{
				++row;
				pos.x += MOVING;
				pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
			}
		}
	}
	else
	{
		accelerationCount[1] = 0;
	}

	if (cpuAI->GetInput(AIInput::DownInput))
	{
		if (++accelerationCount[2] == 1)
		{
			++column;
			pos.y += MOVING;
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
		}
		if (accelerationCount[2] >= ACCEL_START)
		{
			if (accelerationCount[2] % ACCEL_MOVE_PER == 0)
			{
				++column;
				pos.y += MOVING;
				pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
			}
		}
	}
	else
	{
		accelerationCount[2] = 0;
	}

	if (cpuAI->GetInput(AIInput::LeftInput))
	{
		if (++accelerationCount[3] == 1)
		{
			--row;
			pos.x -= MOVING;
			pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
		}
		if (accelerationCount[3] >= ACCEL_START)
		{
			if (accelerationCount[3] % ACCEL_MOVE_PER == 0)
			{
				--row;
				pos.x -= MOVING;
				pAudio->Play(Sound::Get()->seHandle[Sound::SE::MOVE].get());
			}
		}
	}
	else
	{
		accelerationCount[3] = 0;
	}

	PositionCorreciton();
}

void CPU::PositionCorreciton()
{
	if (row > 6 - 1)
	{
		row = 6 - 1;
		pos.x -= MOVING;
	}
	else if (row < 0)
	{
		row = 0;
		pos.x += MOVING;
	}

	if (column > 9 - 1)
	{
		column = 9 - 1;
		pos.y -= MOVING;
	}
	else if (column < 0)
	{
		column = 0;
		pos.y += MOVING;
	}
}

void CPU::SetBreakBlock()
{
	if (regularBlockManager[1].GetStatus() != BlockManager::State::Wait) return;

	if (cpuAI->GetInput(AIInput::BreakInput))
	{
		pAudio->Play(Sound::Get()->seHandle[Sound::SE::DESTRUCTION1].get());
		if (regularBlockManager[1].BreakBlock(row, column))
		{
			regularGameUI[1].SetIsTimerStop(true);
		}
	}
}