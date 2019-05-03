#pragma once
#include <map>
#include <memory>
#include <mutex>
#include <vector>

using VEC_INT = std::vector<int>;

#define LpAudioMng (AudioMng::GetInstance())

struct SoundName
{
	/// 効果音のパス
	const std::string sampleSE = "resource/SE/sample.mp3";
	const std::string shot	   = "resource/SE/shot.mp3";
	const std::string bubble   = "resource/SE/bubble.mp3";
	const std::string hit	   = "resource/SE/hit.mp3";
	const std::string pop	   = "resource/SE/pop.mp3";
	const std::string jump	   = "resource/SE/jump.mp3";
	const std::string die	   = "resource/SE/die.mp3";
	
	/// 音楽のパス
	const std::string mainBGM   = "resource/BGM/main.mp3";
	const std::string resultBGM = "resource/BGM/result.mp3";
};

class AudioMng
{
public:
	static AudioMng & GetInstance()
	{
		return *s_Instance;
	}

	const SoundName GetSound();
	void PlaySE(std::string f_name);
	void PlayBGM(std::string f_name);
	void RepeatBGM(std::string f_name);
	void ChangeVolume(int vol, std::string f_name);	
	void StopBGM();
private:
	const VEC_INT & GetAudio(std::string f_name);
	struct AudioMngDeleter
	{
		void operator()(AudioMng * musMng) const
		{
			delete musMng;
		}
	};

	AudioMng();

	int bgmName;
	SoundName audio;

	static std::unique_ptr<AudioMng, AudioMngDeleter> s_Instance;

	std::map<std::string, VEC_INT> musMap;
};

