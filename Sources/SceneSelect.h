#pragma once

#include <vector>
#include <memory>
#include "Scene.h"
#include "sprite.h"
#include "Selector.h"


#define IF_SINGLE_NOW sceneSelect.JudgeGameMode(SelectGameMode::Single)
#define IF_MULTI_NOW sceneSelect.JudgeGameMode(SelectGameMode::Multi) || sceneSelect.JudgeGameMode(SelectGameMode::CPU)


class SceneSelect : public BaseScene
{
//private:
//	enum SelectMode
//	{
//		PlayStyle,
//		CharacterSelect,
//		LevelSelect,
//		CPULevelSelect,
//		Option,
//		GoGame
//	};
//	enum UpdateState
//	{
//		Move,
//		Choice
//	};
//
//public:
//	enum SelectGameMode
//	{
//		Single = 0,
//		CPU,
//		Multi,
//		Tutorial
//	};

private:
	/*const int characterMin = 1;
	const int characterMax = 7;
	const int player01CharacterDefault = 0;
	const int player02CharacterDefault = 1;*/

	const int levelMin = 1;
	const int levelMax = 7;
	const int levelDefault = 4;

	const int cpuLevelMin = 1;
	const int cpuLevelMax = 3;
	const int cpuLevelDefault = 1;

	const int bgmMin = 1;
	const int bgmMax = 10;
	const int bgmDefault = 5;

	const int seMin = 1;
	const int seMax = 10;
	const int seDefault = 5;

	//const int styleNumMin = 0;
	//const int styleNumMax= 4;
	//const int styleNumDefault = 0;

	//const float frameMMoveX = 17.3f; // 519.0f / 30.0f
	//const float frameLToMMoveX = 19.2f; // (-424.0f - 154.0f) / 30.0f
	//const float frameLToMMoveY =  4.0f; // (415.0f - 295.0f) / 30.0f
	//const float frameMToLMoveX = 23.2f; // (851.0f - 154.0f) / 30.0f
	//const float frameMToLMoveY = 4.0f; // (415.0f - 295.0f) / 30.0f
	//const float frameMScale =  4.0f; // 120.0f / 30.0f
	//const int moveCntMax = 30;

	//const float characterLeftPosX = 246.0f;
	//const float characterRightPosX = 1674.0f; // 1920.0f - 246.0f
	//const float characterPosY = 436.0f;
	//const float characterSizeX = 416.0f;
	//const float characterSizeY = 416.0f;
	//const float characterFrameOriginPosX = 660.0f;
	//const float characterFrameOriginPosY = 508.0f;
	//const float characterFrameSizeX = 150.0f;
	//const float characterFrameSizeY = 150.0f;
	//const float characterFrameTexPosX = 0.0f;
	//const float characterFrameTexPosY = 1012.0f;
	//const float characterPickTexPosX = 0.0f;
	//const float characterPickTexPosY = 1162.0f;
	//const float characterPickSizeX = 150.0f;
	//const float characterPickSizeY = 150.0f;
	
private:
	// セレクター管理
	std::vector<std::shared_ptr<ISelector>> selectors;
	int selectorIndex;
	int previousSelectorIndex;
	int changeSceneIndex;

	// Sprite
	std::shared_ptr<SpriteBatch> sprSelect;
	std::shared_ptr<SpriteBatch> sprSelectCPU;
	std::shared_ptr<SpriteBatch> sprMenuBG;
	std::shared_ptr<SpriteBatch> sprMenuBG2;
//	std::shared_ptr<SpriteBatch> sprSelectFrameL;
//	std::shared_ptr<SpriteBatch> sprSelectFrameM;
//	std::shared_ptr<SpriteBatch> sprSelectFrameS;
//	std::shared_ptr<SpriteBatch> sprSelectArrow;
//	std::shared_ptr<SpriteBatch> sprCharacters[8];
//	
//	// Sprite Pos
//	DirectX::XMFLOAT2 pos;
//	DirectX::XMFLOAT2 frameLPos;
//	DirectX::XMFLOAT2 frameMPos[5];
//	DirectX::XMFLOAT2 frameMSize[5];
//
//	DirectX::XMFLOAT2 characterPos[2];
//	DirectX::XMFLOAT2 characterSize[2];
//	DirectX::XMFLOAT2 characterFrameOriginPos;
//	DirectX::XMFLOAT2 characterFrameSize;
//	DirectX::XMFLOAT2 characterPick[2];
//
//	// enum
//	SelectMode selectMode;
//	UpdateState updateState;
//
//	// PlayStyle選択用
//	int styleNum;
//	bool selectMoveRight;
//	bool selectMoveLeft;
//	int moveCnt;
//
//	// Character選択用
//	int selectCharacterNum[2];
//	bool characterDecision[2];
//	int selectCPUCharacterState;
//
//	// Level選択用
//	int levelNum[2];
//	bool levelDecision[2];
//
//	// CPULevel選択用
//	int cpuLevelNum;
//	bool cpuLevelDecision;
//
	// オプション用
	int bgmVolume;
	int seVolume;
	bool colorCorrection;

public:
	SelectGameMode gameMode;

public:
	SceneSelect() { /*Init();*/ }
	void Init();
	void Uninit();
	void Update();
	void Draw();

	template<class T>
	T* AddSelector();

	void GoGameUpdate();

	bool FileLoad();
	bool FileSave();

	bool JudgeGameMode(SelectGameMode _b); // 【a == b】 is true

	/// <summary>
	/// キャラクター番号の取得関数
	/// </summary>
	/// <param name="playerNumber"> 0 or 1 </param>
	/// <returns> キャラクター番号 </returns>
	int GetCharacterNumber(int playerNumber);
};
