#include "DxLib.h"
#include "AudioMng.h"

std::unique_ptr<AudioMng, AudioMng::AudioMngDeleter> AudioMng::s_Instance(new AudioMng());

AudioMng::AudioMng()
{
}

const VEC_INT & AudioMng::GetAudio(std::string f_name)
{
	if (musMap.find(f_name) == musMap.end())
	{
		musMap[f_name].resize(1);
		musMap[f_name][0] = LoadSoundMem(f_name.c_str());
	}
	return musMap[f_name];
}

const SoundName AudioMng::GetSound()
{
	return audio;
}

void AudioMng::PlaySE(std::string f_name)
{
	GetAudio(f_name);
	PlaySoundMem(musMap[f_name][0], DX_PLAYTYPE_BACK, true);
}

void AudioMng::PlayBGM(std::string f_name)
{
	GetAudio(f_name);

	if (!CheckSoundMem(musMap[f_name][0]))
	{
		StopSoundMem(bgmName);
		bgmName = musMap[f_name][0];
		PlaySoundMem(bgmName, DX_PLAYTYPE_BACK, true);
	}
}

void AudioMng::RepeatBGM(std::string f_name)
{
	GetAudio(f_name);
	if (!CheckSoundMem(musMap[f_name][0]))
	{
		StopSoundMem(bgmName);
		bgmName = musMap[f_name][0];
		PlaySoundMem(bgmName, DX_PLAYTYPE_LOOP, true);
	}
}

void AudioMng::ChangeVolume(int vol, std::string f_name)
{
	ChangeVolumeSoundMem(255 * vol / 100, musMap[f_name][0]);
}

void AudioMng::StopBGM()
{
	StopSoundMem(bgmName);
	bgmName = 0;
}