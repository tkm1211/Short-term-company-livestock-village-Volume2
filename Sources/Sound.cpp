#include "Sound.h"

const wchar_t* SEadress[] =
{
	L"Data/Sound/SE/decision.wav",
	L"Data/Sound/SE/move.wav",
	L"Data/Sound/SE/destruction1.wav",
	L"Data/Sound/SE/gaugefull.wav",
	L"Data/Sound/SE/levelselect.wav",
	L"Data/Sound/SE/OK.wav",
	L"Data/Sound/SE/result.wav",
	L"Data/Sound/SE/scenechange.wav",
	L"Data/Sound/SE/speed.wav",
	L"Data/Sound/SE/go.wav",
	L"Data/Sound/SE/ojamakowasenaindesuwagomennegehe.wav",
	L"Data/Sound/SE/ready.wav",
	L"Data/Sound/SE/Alert.wav",
	L"Data/Sound/SE/end.wav",
	L"Data/Sound/SE/logo.wav",
	L"Data/Sound/SE/logocan.wav"
};

const wchar_t* BGMadress[] =
{
	L"Data/Sound/BGM/title.wav",
	L"Data/Sound/BGM/select.wav",
	L"Data/Sound/BGM/game.wav",
};

void Sound::Load()
{
	// Audio関係のInit
	for (int i = 0; i < SE::SE_END; i++)
	{
		//WAVファイルのロード
		seHandle[i] = new AudioHandle;
		pAudio->LoadSoundFile(SEadress[i], seHandle[i]);
		//pAudio->SetVolume(seHandle[i], )
	}

	for (int i = 0; i < BGM::BGM_END; i++)
	{
		//WAVファイルのロード
		bgmHandle[i] = new AudioHandle;
		pAudio->LoadSoundFile(BGMadress[i], bgmHandle[i]);
	}
}

void Sound::Update()
{
	for (int i = 0; i < SE::SE_END; i++)
	{
		pAudio->Update(seHandle[i]);
	}
	for (int i = 0; i < BGM::BGM_END; i++)
	{
		pAudio->Update(bgmHandle[i]);
	}
}

void Sound::Uninit()
{
	for (int i = 0; i < SE::SE_END; i++)
	{
		if (seHandle[i])
		{
			seHandle[i]->sourceVoice.clear();
			delete seHandle[i];
		}
		seHandle[i] = nullptr;
	}

	for (int i = 0; i < BGM::BGM_END; i++)
	{
		if (bgmHandle[i])
		{
			bgmHandle[i]->sourceVoice.clear();
			delete bgmHandle[i];
		}
		bgmHandle[i] = nullptr;
	}

}