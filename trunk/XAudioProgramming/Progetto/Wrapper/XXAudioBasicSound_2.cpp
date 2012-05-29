//--------------------------------------------------------------------------------------
// File: XAudio2BasicSound.cpp
//
// XNA Developer Connection
// (C) Copyright Microsoft Corp.  All rights reserved.
//--------------------------------------------------------------------------------------
#include "AudioWrapper.h"

//--------------------------------------------------------------------------------------
// Entry point to the program
//--------------------------------------------------------------------------------------
int main()
{
	AudioWrapper* audioWrapper = new AudioWrapper();

	audioWrapper->InitializeXAudio();

	audioWrapper->PrepareAudio(L"MusicMono.wav");

	audioWrapper->PlayAudio();

	while( !GetAsyncKeyState( VK_ESCAPE ) ){
		audioWrapper->UpdateAudio(0.05f);
	}



	

}
