
#include "XAudioHelper.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	XAUDIO2HELPER::XAudioHelper Xaudio;

	// "doukeshitoshoujo" by "Music Atelier Amacha"
	// http://amachamusic.chagasi.com/music_doukeshitoshoujo.html
	int load_index = Xaudio.AddWaveFile(L"doukeshitoshoujo.wav");
	int que_index = Xaudio.CreateQue(load_index);

	// single que play
	Xaudio.GetQue(que_index)->Play();

	// add delay play
	Sleep(200);
	que_index = Xaudio.CreateQue(load_index);
	Xaudio.GetQue(que_index)->Play();

	// Stop And Resume
	Sleep(3000);
	Xaudio.AllStop();
	Sleep(1000);
	Xaudio.AllResume();

	// update
	while (1)
	{

		Xaudio.Update();
		Sleep(100);

	}

	return 0;

}