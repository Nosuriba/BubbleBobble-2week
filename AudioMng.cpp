#include "DxLib.h"
#include "AudioMng.h"

std::unique_ptr<AudioMng, AudioMng::AudioMngDeleter> AudioMng::s_Instance(new AudioMng());

AudioMng::AudioMng()
{
}

const int & AudioMng::GetAudio(std::string f_name)
{
	if (audioMap.find(f_name) == audioMap.end())
	{
		audioMap[f_name] = LoadSoundMem(f_name.c_str());
	}
	return audioMap[f_name];
}

const SoundName AudioMng::GetSound()
{
	return audio;
}

void AudioMng::PlaySE(std::string f_name)
{
	GetAudio(f_name);
	PlaySoundMem(audioMap[f_name], DX_PLAYTYPE_BACK, true);
}

void AudioMng::PlayBGM(std::string f_name)
{
	GetAudio(f_name);

	if (!CheckSoundMem(audioMap[f_name]))
	{
		StopSoundMem(bgmName);
		bgmName = audioMap[f_name];
		PlaySoundMem(bgmName, DX_PLAYTYPE_BACK, true);
	}
}

void AudioMng::RepeatBGM(std::string f_name)
{
	GetAudio(f_name);
	if (!CheckSoundMem(audioMap[f_name]))
	{
		StopSoundMem(bgmName);
		bgmName = audioMap[f_name];
		PlaySoundMem(bgmName, DX_PLAYTYPE_LOOP, true);
	}
}

void AudioMng::ChangeVolume(int vol, std::string f_name)
{
	ChangeVolumeSoundMem(255 * vol / 100, audioMap[f_name]);
}

void AudioMng::StopBGM()
{
	StopSoundMem(bgmName);
	bgmName = 0;
}