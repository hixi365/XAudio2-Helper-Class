
#pragma once

#include <xaudio2.h>

#include <mmsystem.h>

#include <vector>

#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib, "winmm.lib")

namespace XAUDIO2HELPER
{

	class XAudioQue;

	class WaveLoader
	{

	private:
		
		int					m_WaveFileSize;
		WAVEFORMATEX		m_WaveFormatEx;
		std::vector<BYTE>	m_WaveData;

		bool				m_Enable;

	public:

		WaveLoader();
		~WaveLoader();

		HRESULT LoadWaveFile(LPWSTR file);
		HRESULT CreateQue(IXAudio2* xaudio, XAudioQue* que);

		int		GetSize() { return m_WaveFileSize; }

	};

	class XAudioQue
	{

	private:

		bool						m_Enable;
		bool						m_Played;
		IXAudio2SourceVoice*		m_SourceVoice;

	public:

		XAudioQue();
		~XAudioQue();

		void	SetSourceVoice(IXAudio2SourceVoice* source) { m_SourceVoice = source; }
		void	SetEnable(bool flg) { m_Enable = flg; }

		void	SetVolume(float volume) { if (m_SourceVoice) m_SourceVoice->SetVolume(volume); }
		void	SetPitch(float pitch) { if (m_SourceVoice) m_SourceVoice->SetFrequencyRatio(pitch); }

		void	Play(){ if (m_SourceVoice) m_SourceVoice->Start(); }
		void	Stop(){ if (m_SourceVoice) m_SourceVoice->Stop(); }

		void	Resume();

		int		Check();

	};

	class XAudioHelper
	{

	private:

		IXAudio2*					m_XAudio;
		IXAudio2MasteringVoice*		m_MasteringVoice;

		std::vector<WaveLoader*>	m_WaveLoaderVector;
		std::vector<XAudioQue*>		m_XAudioQueVector;

		HRESULT	Initialize();
		void	Cleanup();


	public:

		XAudioHelper();
		~XAudioHelper();

		void		Update();

		int			AddWaveFile(LPWSTR file);
		int			CreateQue(int index, float volume = 1.0f, float pitch = 1.0f);

		XAudioQue*	GetQue(int index) { return m_XAudioQueVector[index]; }

		void		AllStop();
		void		AllResume();

	};

}