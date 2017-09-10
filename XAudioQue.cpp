
#include "XAudioHelper.h"

using namespace XAUDIO2HELPER;

XAudioQue::XAudioQue() :
m_Enable(false),
m_Played(false),
m_SourceVoice(nullptr)
{


}

XAudioQue::~XAudioQue()
{


}

void XAudioQue::Resume()
{

	if (!m_Enable)
		return;

	XAUDIO2_VOICE_STATE state;
	m_SourceVoice->GetState(&state);

	if (state.SamplesPlayed == 0)
		return;

	m_SourceVoice->Start();

}

int XAudioQue::Check()
{

	if (!m_Enable)
		return -1;

	XAUDIO2_VOICE_STATE state;
	m_SourceVoice->GetState(&state);
	
	// set played flag : que is playing
	if (!m_Played && state.SamplesPlayed){
		m_Played = true;
	}

	// que ended : state.SamplesPlayed == 0
	if (m_Played && state.SamplesPlayed == 0)
		return -1;

	return 0;

}