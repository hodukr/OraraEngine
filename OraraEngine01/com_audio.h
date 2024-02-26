#pragma once

#include <xaudio2.h>
#include "gameObject.h"


class Audio : public Component
{
private:
	static IXAudio2* m_Xaudio;
	static IXAudio2MasteringVoice* m_MasteringVoice;

	IXAudio2SourceVoice* m_SourceVoice{};
	BYTE* m_SoundData{};

	int						m_Length{};
	int						m_PlayLength{};

	float                   m_Volume = 0.0f;
public:
	static void InitMaster();
	static void UninitMaster();

	using Component::Component;

	void Uninit();

	void Load(const char* FileName);
	void Play(bool Loop = false);
	void Stop()
	{
		m_SourceVoice->Stop();
		m_SourceVoice->FlushSourceBuffers();
	}
	void SetVolume(float volume)
	{
		m_Volume = volume;
		if (m_SourceVoice)
			m_SourceVoice->SetVolume(volume);
	}
};


