#include "SceneSelect.h"

#include <input_device.h>
#include <imgui.h>
#include <string>
#include <fstream>
#include <iostream>

#include "Resource.h"
#include "SceneManager.h"
#include "Production.h"
#include "Sound.h"

#include "PlayStyleSelector.h"
#include "CharacterSelector.h"
#include "LevelSelector.h"


/*--------------------------------------*/
//	Global area
/*--------------------------------------*/

// unknown


/*--------------------------------------*/
//	Menber function
/*--------------------------------------*/
void SceneSelect::Init()
{
	selectors.clear();

	AddSelector<PlayStyleSelector>();
	AddSelector<CharacterSelector>();
	AddSelector<LevelSelector>();

	for (auto& selector : selectors)
	{
		selector->Construct();
	}

	selectorIndex = 0;
	previousSelectorIndex = 0;
	changeSceneIndex = 0;
	selectors[selectorIndex]->Init(nullptr);

	sprMenuBG = RESOURCE->GetSpriteData(Resource::Texture::MenuBG);
	sprMenuBG2 = RESOURCE->GetSpriteData(Resource::Texture::MenuBG2);

	// TODO : file load
	if (!FileLoad())
	{
		bgmVolume = bgmDefault;
		seVolume = seDefault;
		colorCorrection = false;
	}
	sprSelect = RESOURCE->GetSpriteData(Resource::Texture::Select);
	gameMode = SelectGameMode::Single;

	// Play BGM
	pAudio->Play(Sound::Get()->bgmHandle[Sound::BGM::SELECT].get(), true);
	pAudio->SetVolume(Sound::Get()->bgmHandle[Sound::BGM::SELECT].get(), 1.2f);
}

template<class T>
T* SceneSelect::AddSelector()
{
	std::shared_ptr<T> selecter = std::make_shared<T>();
	selectors.emplace_back(selecter);

	return selecter.get();
}

void SceneSelect::Update()
{
	if (PRODUCTION->CheckFlag(GO_SINGLEGAME))
		PRODUCTION->CSOH(SCENE_MANAGER->SINGLE_GAME, true);
	if (PRODUCTION->CheckFlag(GO_PLAYERSTILE))
		PRODUCTION->CSOH(SCENE_MANAGER->SCENE_SELECT);
	if (PRODUCTION->CheckFlag(GO_MULTIGAME))
		PRODUCTION->CSOH(SCENE_MANAGER->MULTI_GAME, true);
	if (PRODUCTION->CheckFlag(GO_CPUGAME))
		PRODUCTION->CSOH(SCENE_MANAGER->CPU_GAME, true);
	if (PRODUCTION->CheckFlag(GO_PLAYERSELECT))
	{
		PRODUCTION->CSOH(SCENE_MANAGER->PLAYER_SELECT);
		if (PRODUCTION->state == PRODUCTION->CHANGE)
		{
			selectorIndex = changeSceneIndex;
			PRODUCTION->state++;
		}
	}


	if (PRODUCTION->CheckFlag(GO_PLAYERSTILE) || PRODUCTION->CheckFlag(GO_SINGLEGAME) || PRODUCTION->CheckFlag(GO_MULTIGAME) || PRODUCTION->CheckFlag(GO_CPUGAME) || PRODUCTION->CheckFlag(GO_PLAYERSELECT)) return;

	if (selectorIndex != static_cast<int>(SelectMode::GoGame)) // セレクト画面
	{
		if (selectors[selectorIndex]->Update())
		{
			if (selectorIndex == 0)
			{
				PlayStyleSelector* playStyleSelector = dynamic_cast<PlayStyleSelector*>(selectors[selectorIndex].get());
				gameMode = playStyleSelector->GetSelectGameMode();
			}

			SelectMode selectMode = selectors[selectorIndex]->GetSelectMode();

			if (selectMode == SelectMode::CPULevelSelect)
			{
				selectMode = SelectMode::LevelSelect;
			}

			previousSelectorIndex = selectorIndex;
			selectorIndex = static_cast<int>(selectMode);

			if (selectMode != SelectMode::GoGame)
			{
				selectors[selectorIndex]->Init(selectors[previousSelectorIndex].get());
			}

			if (selectorIndex == static_cast<int>(SelectMode::CharacterSelect) && previousSelectorIndex == static_cast<int>(SelectMode::PlayStyle))
			{
				changeSceneIndex = selectorIndex;
				selectorIndex = previousSelectorIndex;
				PRODUCTION->SetOn(GO_PLAYERSELECT);
				PRODUCTION->Start();
			}
			else if (selectorIndex == static_cast<int>(SelectMode::PlayStyle))
			{
				changeSceneIndex = selectorIndex;
				selectorIndex = previousSelectorIndex;
				PRODUCTION->SetOn(GO_PLAYERSELECT);
				PRODUCTION->Start();
			}
		}
	}
	else // ゲームへ移行
	{
		GoGameUpdate();
	}
}

void SceneSelect::GoGameUpdate()
{
	if (gameMode == SelectGameMode::Single)
	{
		PRODUCTION->SetOn(GO_SINGLEGAME);
		PRODUCTION->Start();
	}
	else if (gameMode == SelectGameMode::Multi)
	{
		PRODUCTION->SetOn(GO_MULTIGAME);
		PRODUCTION->Start();
	}
	else if (gameMode == SelectGameMode::CPU)
	{
		PRODUCTION->SetOn(GO_CPUGAME);
		PRODUCTION->Start();
	}
}

void SceneSelect::Draw()
{
	if (selectorIndex == static_cast<int>(SelectMode::PlayStyle))
	{
		sprMenuBG2->Begin();
		sprMenuBG2->Draw(0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 0.0f, 0.0f);
		sprMenuBG2->End();
	}
	else
	{
		sprMenuBG->Begin();
		sprMenuBG->Draw(0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 0.0f, 0.0f);
		sprMenuBG->End();
	}

	if (selectorIndex != static_cast<int>(SelectMode::GoGame)) // セレクト中
	{
		selectors[selectorIndex]->Draw();
	}
	else // ゲーム移行中
	{
		selectors[previousSelectorIndex]->Draw();
	}

	if (PRODUCTION->CheckFlag(GO_PLAYERSTILE) || PRODUCTION->CheckFlag(GO_SINGLEGAME) || PRODUCTION->CheckFlag(GO_MULTIGAME) || PRODUCTION->CheckFlag(GO_CPUGAME) || PRODUCTION->CheckFlag(GO_PLAYERSELECT))
	{
		PRODUCTION->Draw();
	}
}


void SceneSelect::Uninit()
{
	// Play BGM
	pAudio->Stop(Sound::Get()->bgmHandle[Sound::BGM::SELECT].get());
	pAudio->DeleteSourceVoice(Sound::Get()->bgmHandle[Sound::BGM::SELECT].get());
}

bool SceneSelect::FileLoad()
{
	wchar_t fileName[] = L"Data/Document/Option.txt";

	std::ifstream fin(fileName);
	if (!fin.is_open()) return false;

	char command[256];
	int num_cnt = 0;

	while (fin)
	{
		fin >> command;

		if (0 == strcmp(command, "bgm"))
		{
			fin >> bgmVolume;
			fin.ignore(1024, '\n');
		}
		else if (0 == strcmp(command, "se"))
		{
			fin >> seVolume;
			fin.ignore(1024, '\n');
		}
		else if (0 == strcmp(command, "color"))
		{
			fin >> colorCorrection;
			fin.ignore(1024, '\n');
		}
		else
		{
			fin.ignore(1024, '\n');
		}
	}
	fin.close();

	return true;
}

bool SceneSelect::FileSave()
{
	wchar_t fileName[] = L"Data/Document/Option.txt";

	std::wifstream ifs(fileName);

	std::wofstream writeFile;
	writeFile.open(fileName, std::wios::out);

	writeFile << "bgm" << " " << bgmVolume << std::endl;
	writeFile << "se" << " " << seVolume << std::endl;
	writeFile << "color" << " " << colorCorrection << std::endl;

	writeFile.close();

	return true;
}

bool SceneSelect::JudgeGameMode(SelectGameMode _b)
{
	if (gameMode == _b)return true;

	return false;
}

int SceneSelect::GetCharacterNumber(int playerNumber)
{
	int index = static_cast<int>(SelectMode::CharacterSelect);
	CharacterSelector* characterSelector = dynamic_cast<CharacterSelector*>(selectors[index].get());

	return characterSelector->GetSelectCharacterNum(playerNumber);
}

int SceneSelect::GetLevel(int playerNumber)
{
	int index = static_cast<int>(SelectMode::LevelSelect);
	LevelSelector* levelSelector = dynamic_cast<LevelSelector*>(selectors[index].get());

	return levelSelector->GetLevel(playerNumber);
}

int SceneSelect::GetCPUPowor()
{
	int index = static_cast<int>(SelectMode::LevelSelect);
	LevelSelector* levelSelector = dynamic_cast<LevelSelector*>(selectors[index].get());

	return levelSelector->GetCPUPowor();
}