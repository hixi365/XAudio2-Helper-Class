
#include "XAudioHelper.h"

using namespace XAUDIO2HELPER;

WaveLoader::WaveLoader() :
m_WaveFileSize(-1),
m_Enable(false)
{
}

WaveLoader::~WaveLoader()
{

	if (m_Enable){
		m_WaveData.clear();
	}

}

HRESULT WaveLoader::LoadWaveFile(LPWSTR file)
{
	HRESULT hr = S_OK;
	
	// open wave file for mmio
	HMMIO hMmio = nullptr;
	MMIOINFO mmioInfo;

	// open wave file
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen(file, &mmioInfo, MMIO_READ);
	if (!hMmio)
		return -1;

	// RIFF chunk serch
	MMRESULT mmRes;
	MMCKINFO riffChunk;
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmRes = mmioDescend(hMmio, &riffChunk, nullptr, MMIO_FINDRIFF);
	if (mmRes != MMSYSERR_NOERROR)
		return -1;

	// format chunk serch
	MMCKINFO formatChunk;
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR)
		return -1;

	// WAVEFORMATEX
	DWORD fmsize = formatChunk.cksize;
	DWORD size = mmioRead(hMmio, (HPSTR)&m_WaveFormatEx, fmsize);
	if (size != fmsize){
		mmioClose(hMmio, 0);
		return -1;
	}

	// WAVEFORMATEX
	mmioAscend(hMmio, &formatChunk, 0);

	// data chunk serch
	MMCKINFO dataChunk;
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
	if (mmRes != MMSYSERR_NOERROR){
		mmioClose(hMmio, 0);
		return -1;
	}

	// wave data
	char * pData = new char[dataChunk.cksize];
	size = mmioRead(hMmio, (HPSTR)pData, dataChunk.cksize);
	if (size != dataChunk.cksize){
		delete[] pData;
		return -1;
	}

	// make data vector
	std::vector<BYTE>WaveData(size);
	memcpy(&WaveData[0], &pData[0], size);
	m_WaveData = WaveData;
	m_WaveFileSize = size;

	// close mmio
	mmioClose(hMmio, 0);

	m_Enable = true;

	return S_OK;

}

HRESULT WaveLoader::CreateQue(IXAudio2* xaudio, XAudioQue* que)
{

	if (!m_Enable)
		return E_FAIL;

	IXAudio2SourceVoice* sorucevoice = nullptr;

	// create source voice
	xaudio->CreateSourceVoice(&sorucevoice, &m_WaveFormatEx);

	// submit source voice
	XAUDIO2_BUFFER submit = { 0 };
	submit.AudioBytes = m_WaveFileSize;
	submit.pAudioData = &m_WaveData[0];
	submit.Flags = XAUDIO2_END_OF_STREAM;
	sorucevoice->SubmitSourceBuffer(&submit);

	// set source voice to que
	que->SetSourceVoice(sorucevoice);
	que->SetEnable(true);

	return S_OK;

}