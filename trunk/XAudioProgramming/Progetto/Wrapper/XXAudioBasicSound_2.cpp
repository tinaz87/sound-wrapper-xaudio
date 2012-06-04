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
D3DXVECTOR3 ListenerPos;
D3DXVECTOR3 EmitterPos;

int main()
{
	InitAudioWrapper* init = new InitAudioWrapper();
	init->InitializeXAudio();

	AudioWrapper* audioWrapper = new AudioWrapper(init);

	AudioWrapper* audioWrapper1 = new AudioWrapper(init);

	HKL hkl =LoadKeyboardLayoutW(L"00000410",KLF_ACTIVATE); // 00000410 codice tastira italiana
	//AudioWrapper* audioWrapper = new AudioWrapper();

	//audioWrapper->InitializeXAudio();

	audioWrapper->PrepareAudio(L"MusicMono.wav");

	//AudioWrapper* audioWrapper1 = new AudioWrapper();

	//audioWrapper1->InitializeXAudio();

	audioWrapper1->PrepareAudio(L"heli.wav");

	short pause = VkKeyScanExW('p', hkl );
	short resume = VkKeyScanExW('r', hkl );

	//

	//

	while( !GetAsyncKeyState( VK_ESCAPE ) ){
		audioWrapper->UpdateAudio(0.05f);


		if (GetAsyncKeyState( pause ) && !audioWrapper->IsPaused() && !audioWrapper->IsStopped() )
		{
			wprintf( L"\nKey Pause pressed: ");
			audioWrapper->PauseAudio();
			
		}

		if (GetAsyncKeyState( resume ) && audioWrapper->IsPaused())
		{
			wprintf( L"\nKey Resume pressed: ");
			audioWrapper->ResumeAudio();
			
		}

		if (GetAsyncKeyState( VK_RETURN ) && !audioWrapper->IsPlaing() )
		{
			wprintf( L"\nKey Play pressed: ");

			audioWrapper->PlayAudio();

			audioWrapper1->PlayAudio();

			
		}

		if (GetAsyncKeyState(VK_DELETE) && !audioWrapper->IsStopped() )
		{
			wprintf( L"\nKey Stop pressed:");
			audioWrapper->StopAudio();

		}


		if (GetAsyncKeyState(VK_LEFT))
		{
			audioWrapper->TranslateEmitterPosition(D3DXVECTOR3(-0.0001f,0,0));
		}
		
		if (GetAsyncKeyState(VK_RIGHT))
		{
			audioWrapper->TranslateEmitterPosition(D3DXVECTOR3(0.0001f,0,0));
		}

		if (GetAsyncKeyState(VK_UP))
		{
			audioWrapper->TranslateEmitterPosition(D3DXVECTOR3(0,0.0001f,0));
		}

		if (GetAsyncKeyState(VK_DOWN))
		{
			audioWrapper->TranslateEmitterPosition(D3DXVECTOR3(0,-0.0001f,0));
		}
	}



	

}
