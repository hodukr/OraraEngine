#pragma once

#include <xaudio2.h>
#include "component.h"
#include "cereal/types/string.hpp"


class Audio : public Component
{
private:
	static IXAudio2* m_Xaudio;
	static IXAudio2MasteringVoice* m_MasteringVoice;

	IXAudio2SourceVoice* m_SourceVoice{};
	BYTE* m_SoundData{};

	int						m_Length{};
	int						m_PlayLength{};

	float                   m_Volume{};
	std::string             m_SoundFile{""};

public:
	static void InitMaster();
	static void UninitMaster();

	using Component::Component;

	void DrawInspector()
	{
		SET_DATE(m_Volume);
		SET_NEXT_FOLDER("asset\\audio", ".");
		if (SET_DATE_STATE(m_SoundFile, CASTOMDRAWSTATE_STRING_FOLDER))
		{
			SetSound(m_SoundFile);
		}
	}
	void Init()override;
	void Uninit()override;
	void Update()override;

	void Load(const char*fileName);
	void Play(bool loop = false);

	void SetSound(std::string file);

	void Stop()
	{
		m_SourceVoice->Stop();
		m_SourceVoice->FlushSourceBuffers();
	}
	void SetVolume(float volume)
	{
		m_Volume = volume;
		if (m_SourceVoice)
			m_SourceVoice->SetVolume(m_Volume);
	}
	void SetVolumeUp(float volume)
	{
		m_Volume += volume;
		if (m_SourceVoice)
			m_SourceVoice->SetVolume(m_Volume);
	}
	void SetVolumeDown(float volume)
	{
		m_Volume -= volume;
		if (m_SourceVoice)
			m_SourceVoice->SetVolume(m_Volume);
	}

	template<class Archive>
	void serialize(Archive& archive)
	{
		try
		{
			archive(CEREAL_NVP(m_Volume),cereal::make_nvp("fileName", m_SoundFile));
		}
		catch (const std::exception&)
		{

		}

	}
};


