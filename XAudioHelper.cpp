
#include "XAudioHelper.h"

using namespace XAUDIO2HELPER;

XAudioHelper::XAudioHelper() :
m_XAudio(nullptr),
m_MasteringVoice(nullptr)
{

	Initialize();

}

XAudioHelper::~XAudioHelper()
{

	Cleanup();

}


HRESULT XAudioHelper::Initialize()
{

	HRESULT hr;

	if (hr = CoInitializeEx(NULL, COINIT_MULTITHREADED)) return hr;
	if (hr = XAudio2Create(&m_XAudio, 0)) return hr;
	if (hr = m_XAudio->CreateMasteringVoice(&m_MasteringVoice)) return hr;

	m_XAudioQueVector.clear();

	return S_OK;

}

void XAudioHelper::Cleanup()
{

	for (int i = 0; i < m_XAudioQueVector.size(); i++)
	{
		delete m_XAudioQueVector[i];
	}
	m_XAudioQueVector.clear();

	if (m_MasteringVoice){
		m_MasteringVoice->DestroyVoice();
		m_MasteringVoice = nullptr;
	}
	if (m_XAudio){
		m_XAudio->Release();
		m_XAudio = nullptr;
	}

	CoUninitialize();

}

int XAudioHelper::AddWaveFile(LPWSTR file)
{

	WaveLoader* pWaveLoader = new WaveLoader();

	if (pWaveLoader->LoadWaveFile(file))
	{
		return -1;
	}

	// add vector and return index : size - 1
	m_WaveLoaderVector.push_back(pWaveLoader);
	return m_WaveLoaderVector.size() - 1;

}

int XAudioHelper::CreateQue(int index, float volume, float pitch)
{

	XAudioQue* que = new XAudioQue();
	
	// wave loader index check
	if (index < 0 || index > m_WaveLoaderVector.size() - 1)
	{
		delete que;
		return -1;
	}

	if (m_WaveLoaderVector[index]->CreateQue(m_XAudio, que))
	{
		delete que;
		return -1;
	}

	// set volume and pitch
	que->SetVolume(volume);
	que->SetPitch(pitch);

	// add vector and return index : size - 1
	m_XAudioQueVector.push_back(que);
	return m_XAudioQueVector.size() - 1;

}

void XAudioHelper::Update()
{

	for (int i = 0; i < m_XAudioQueVector.size(); i++)
	{

		if (m_XAudioQueVector[i]->Check()){
			delete m_XAudioQueVector[i];
			m_XAudioQueVector.erase(m_XAudioQueVector.begin() + i);
			continue;
		}

	}

}

void XAudioHelper::AllStop()
{

	for (int i = 0; i < m_XAudioQueVector.size(); i++)
		m_XAudioQueVector[i]->Stop();

}

void XAudioHelper::AllResume()
{

	for (int i = 0; i < m_XAudioQueVector.size(); i++)
		m_XAudioQueVector[i]->Resume();

}
