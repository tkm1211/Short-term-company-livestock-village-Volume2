#include "Sound.h"

const wchar_t* address_of_se[] =
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
	L"Data/Sound/SE/logocan.wav",
	L"Data/Sound/SE/LandingS.wav",
	L"Data/Sound/SE/LandingL.wav"
};

const wchar_t* address_of_bgm[] =
{
	L"Data/Sound/BGM/title.wav",
	L"Data/Sound/BGM/select.wav",
	L"Data/Sound/BGM/game.wav",
};

Sound* Sound::instance;

void Sound::Load()
{
	for (int i = 0; i < SE::SE_END; i++)
	{
		seHandle[i] = std::make_unique<AudioHandle>();
		pAudio->LoadSoundFile(address_of_se[i], seHandle[i].get());
	}

	for (int i = 0; i < BGM::BGM_END; i++)
	{
		bgmHandle[i] = std::make_unique<AudioHandle>();
		pAudio->LoadSoundFile(address_of_bgm[i], bgmHandle[i].get());
	}
}

void Sound::Update()
{
	for (auto& it : seHandle)
	{
		pAudio->Update(it.get());
	}
	for (auto& it : bgmHandle)
	{
		pAudio->Update(it.get());
	}
}

void Sound::Uninit()
{
	// Unknown
}

//pAudio->Play(Sound::Get()->bgmHandle[Sound::BGM::GAME].get());
