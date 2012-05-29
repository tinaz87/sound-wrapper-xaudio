#include "AudioWrapper.h"

// Must match order of g_PRESET_NAMES
XAUDIO2FX_REVERB_I3DL2_PARAMETERS g_PRESET_PARAMS[ NUM_PRESETS ] =
{
    XAUDIO2FX_I3DL2_PRESET_FOREST,
    XAUDIO2FX_I3DL2_PRESET_DEFAULT,
    XAUDIO2FX_I3DL2_PRESET_GENERIC,
    XAUDIO2FX_I3DL2_PRESET_PADDEDCELL,
    XAUDIO2FX_I3DL2_PRESET_ROOM,
    XAUDIO2FX_I3DL2_PRESET_BATHROOM,
    XAUDIO2FX_I3DL2_PRESET_LIVINGROOM,
    XAUDIO2FX_I3DL2_PRESET_STONEROOM,
    XAUDIO2FX_I3DL2_PRESET_AUDITORIUM,
    XAUDIO2FX_I3DL2_PRESET_CONCERTHALL,
    XAUDIO2FX_I3DL2_PRESET_CAVE,
    XAUDIO2FX_I3DL2_PRESET_ARENA,
    XAUDIO2FX_I3DL2_PRESET_HANGAR,
    XAUDIO2FX_I3DL2_PRESET_CARPETEDHALLWAY,
    XAUDIO2FX_I3DL2_PRESET_HALLWAY,
    XAUDIO2FX_I3DL2_PRESET_STONECORRIDOR,
    XAUDIO2FX_I3DL2_PRESET_ALLEY,
    XAUDIO2FX_I3DL2_PRESET_CITY,
    XAUDIO2FX_I3DL2_PRESET_MOUNTAINS,
    XAUDIO2FX_I3DL2_PRESET_QUARRY,
    XAUDIO2FX_I3DL2_PRESET_PLAIN,
    XAUDIO2FX_I3DL2_PRESET_PARKINGLOT,
    XAUDIO2FX_I3DL2_PRESET_SEWERPIPE,
    XAUDIO2FX_I3DL2_PRESET_UNDERWATER,
    XAUDIO2FX_I3DL2_PRESET_SMALLROOM,
    XAUDIO2FX_I3DL2_PRESET_MEDIUMROOM,
    XAUDIO2FX_I3DL2_PRESET_LARGEROOM,
    XAUDIO2FX_I3DL2_PRESET_MEDIUMHALL,
    XAUDIO2FX_I3DL2_PRESET_LARGEHALL,
    XAUDIO2FX_I3DL2_PRESET_PLATE,
};

AudioWrapper::AudioWrapper():pXAudio2(nullptr),pMasteringVoice(nullptr)
							,hr(NULL),isPlaing(false),pSourceVoice(nullptr)
							,pbSampleData(nullptr)
{
	// empty
}


//--------------------------------------------------------------------------------------//

//HRESULT AudioWrapper::PlayPCM( IXAudio2* pXaudio2, LPCWSTR szFilename )
//{
//	
//	HRESULT hr = S_OK;
//
//	//
//	// Locate the wave file
//	//
//	WCHAR strFilePath[MAX_PATH];
//	if( FAILED( hr = FindMediaFileCch( strFilePath, MAX_PATH, szFilename ) ) )
//	{
//		wprintf( L"Failed to find media file: %s\n", szFilename );
//		return hr;
//	}
//
//	//
//	// Read in the wave file
//	//
//	CWaveFile wav;
//	if( FAILED( hr = wav.Open( strFilePath, NULL, WAVEFILE_READ ) ) )
//	{
//		wprintf( L"Failed reading WAV file: %#X (%s)\n", hr, strFilePath );
//		return hr;
//	}
//
//	// Get format of wave file
//	WAVEFORMATEX* pwfx = wav.GetFormat();
//
//	// Calculate how many bytes and samples are in the wave
//	DWORD cbWaveSize = wav.GetSize();
//
//	// Read the sample data into memory
//	BYTE* pbWaveData = new BYTE[ cbWaveSize ];
//
//	if( FAILED( hr = wav.Read( pbWaveData, cbWaveSize, &cbWaveSize ) ) )
//	{
//		wprintf( L"Failed to read WAV data: %#X\n", hr );
//		SAFE_DELETE_ARRAY( pbWaveData );
//		return hr;
//	}
//
//	//
//	// Play the wave using a XAudio2SourceVoice
//	//
//
//	// Create the source voice
//	IXAudio2SourceVoice* pSourceVoice;
//	if( FAILED( hr = pXaudio2->CreateSourceVoice( &pSourceVoice, pwfx ) ) )
//	{
//		wprintf( L"Error %#X creating source voice\n", hr );
//		SAFE_DELETE_ARRAY( pbWaveData );
//		return hr;
//	}
//
//	// Submit the wave sample data using an XAUDIO2_BUFFER structure
//	XAUDIO2_BUFFER buffer = {0};
//	buffer.pAudioData = pbWaveData;
//	buffer.Flags = XAUDIO2_END_OF_STREAM;  // tell the source voice not to expect any data after this buffer
//	buffer.AudioBytes = cbWaveSize;
//
//	if( FAILED( hr = pSourceVoice->SubmitSourceBuffer( &buffer ) ) )
//	{
//		wprintf( L"Error %#X submitting source buffer\n", hr );
//		pSourceVoice->DestroyVoice();
//		SAFE_DELETE_ARRAY( pbWaveData );
//		return hr;
//	}
//
//	hr = pSourceVoice->Start( 0 );
//	
//	return hr;
//}
//

//--------------------------------------------------------------------------------------
// Helper function to try to find the location of a media file
//--------------------------------------------------------------------------------------
HRESULT FindMediaFileCch( WCHAR* strDestPath, int cchDest, LPCWSTR strFilename )
{
	
	bool bFound = false;

	if( NULL == strFilename || strFilename[0] == 0 || NULL == strDestPath || cchDest < 10 )
		return E_INVALIDARG;

	// Get the exe name, and exe path
	WCHAR strExePath[MAX_PATH] = {0};
	WCHAR strExeName[MAX_PATH] = {0};
	WCHAR* strLastSlash = NULL;
	GetModuleFileName( NULL, strExePath, MAX_PATH );
	strExePath[MAX_PATH - 1] = 0;
	strLastSlash = wcsrchr( strExePath, TEXT( '\\' ) );
	if( strLastSlash )
	{
		wcscpy_s( strExeName, MAX_PATH, &strLastSlash[1] );

		// Chop the exe name from the exe path
		*strLastSlash = 0;

		// Chop the .exe from the exe name
		strLastSlash = wcsrchr( strExeName, TEXT( '.' ) );
		if( strLastSlash )
			*strLastSlash = 0;
	}

	wcscpy_s( strDestPath, cchDest, strFilename );
	if( GetFileAttributes( strDestPath ) != 0xFFFFFFFF )
		return S_OK;

	// Search all parent directories starting at .\ and using strFilename as the leaf name
	WCHAR strLeafName[MAX_PATH] = {0};
	wcscpy_s( strLeafName, MAX_PATH, strFilename );

	WCHAR strFullPath[MAX_PATH] = {0};
	WCHAR strFullFileName[MAX_PATH] = {0};
	WCHAR strSearch[MAX_PATH] = {0};
	WCHAR* strFilePart = NULL;

	GetFullPathName( L".", MAX_PATH, strFullPath, &strFilePart );
	if( strFilePart == NULL )
		return E_FAIL;

	while( strFilePart != NULL && *strFilePart != '\0' )
	{
		swprintf_s( strFullFileName, MAX_PATH, L"%s\\%s", strFullPath, strLeafName );
		if( GetFileAttributes( strFullFileName ) != 0xFFFFFFFF )
		{
			wcscpy_s( strDestPath, cchDest, strFullFileName );
			bFound = true;
			break;
		}

		swprintf_s( strFullFileName, MAX_PATH, L"%s\\%s\\%s", strFullPath, strExeName, strLeafName );
		if( GetFileAttributes( strFullFileName ) != 0xFFFFFFFF )
		{
			wcscpy_s( strDestPath, cchDest, strFullFileName );
			bFound = true;
			break;
		}

		swprintf_s( strSearch, MAX_PATH, L"%s\\..", strFullPath );
		GetFullPathName( strSearch, MAX_PATH, strFullPath, &strFilePart );
	}
	if( bFound )
		return S_OK;

	// On failure, return the file as the path but also return an error code
	wcscpy_s( strDestPath, cchDest, strFilename );
	
	return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );
}

//--------------------------------------------------------------------------------------//




HRESULT AudioWrapper::InitializeXAudio(){

	//
	// Initialize XAudio2
	//
	CoInitializeEx( NULL, COINIT_MULTITHREADED );// COINIT_MULTITHREADED
												 // Initializes the thread for multithreaded object concurrency .
	
	UINT32 flags = 0;
#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif

	if( FAILED( hr = XAudio2Create( &pXAudio2, flags ) ) )
	{
		wprintf( L"Failed to init XAudio2 engine: %#X\n", hr );
		return hr;
	}

	//
	// Create a mastering voice
	//
	if( FAILED( hr = pXAudio2->CreateMasteringVoice( &pMasteringVoice ) ) )
	{
		wprintf( L"Failed creating mastering voice: %#X\n", hr );
		SAFE_RELEASE( pXAudio2 );
		CoUninitialize();
		return hr;
	}


	// Check device details to make sure it's within our sample supported parameters
    XAUDIO2_DEVICE_DETAILS details;
    if( FAILED( hr = pXAudio2->GetDeviceDetails( 0, &details ) ) )
    {
        SAFE_RELEASE( pXAudio2 );
        return hr;
    }

    if( details.OutputFormat.Format.nChannels > OUTPUTCHANNELS )
    {
        SAFE_RELEASE( pXAudio2 );
        return E_FAIL;
    }

	dwChannelMask = details.OutputFormat.dwChannelMask;
    nChannels = details.OutputFormat.Format.nChannels;

	 //
    // Create reverb effect
    //
    flags = 0;
#ifdef _DEBUG
    flags |= XAUDIO2FX_DEBUG;
#endif

    if( FAILED( hr = XAudio2CreateReverb( &pReverbEffect, flags ) ) )
    {
        SAFE_RELEASE( pXAudio2 );
        return hr;
    }

    //
    // Create a submix voice
    //

    // Performance tip: you need not run global FX with the sample number
    // of channels as the final mix.  For example, this sample runs
    // the reverb in mono mode, thus reducing CPU overhead.
    XAUDIO2_EFFECT_DESCRIPTOR effects[] = { { pReverbEffect, TRUE, 1 } };
    XAUDIO2_EFFECT_CHAIN effectChain = { 1, effects };

    if( FAILED( hr = pXAudio2->CreateSubmixVoice( &pSubmixVoice, 1,
                                                  details.OutputFormat.Format.nSamplesPerSec, 0, 0,
                                                  NULL, &effectChain ) ) )
    {
        SAFE_RELEASE( pXAudio2 );
        SAFE_RELEASE( pReverbEffect );
        return hr;
    }

    // Set default FX params
    XAUDIO2FX_REVERB_PARAMETERS native;
    ReverbConvertI3DL2ToNative( &g_PRESET_PARAMS[0], &native );
    pSubmixVoice->SetEffectParameters( 0, &native, sizeof( native ) );

	 //
    // Initialize X3DAudio
    //  Speaker geometry configuration on the final mix, specifies assignment of channels
    //  to speaker positions, defined as per WAVEFORMATEXTENSIBLE.dwChannelMask
    //
    //  SpeedOfSound - speed of sound in user-defined world units/second, used
    //  only for doppler calculations, it must be >= FLT_MIN
    //
    const float SPEEDOFSOUND = X3DAUDIO_SPEED_OF_SOUND;

    X3DAudioInitialize( details.OutputFormat.dwChannelMask, SPEEDOFSOUND, x3DInstance );

	initialize3DSound(details);

	bInitialized = true;



}

void AudioWrapper::initialize3DSound(XAUDIO2_DEVICE_DETAILS& details){

	vListenerPos = D3DXVECTOR3( 0, 0, 0 );
    vEmitterPos = D3DXVECTOR3( 0, 0, float( ZMAX ) );

    fListenerAngle = 0;
    fUseListenerCone = TRUE;
    fUseInnerRadius = TRUE;
    fUseRedirectToLFE = ((details.OutputFormat.dwChannelMask & SPEAKER_LOW_FREQUENCY) != 0);

    //
    // Setup 3D audio structs
    //
    listener.Position = vListenerPos;
    listener.OrientFront = D3DXVECTOR3( 0, 0, 1 );
    listener.OrientTop = D3DXVECTOR3( 0, 1, 0 );
    listener.pCone = (X3DAUDIO_CONE*)&Listener_DirectionalCone;

    emitter.pCone = &emitterCone;
    emitter.pCone->InnerAngle = 0.0f;
    // Setting the inner cone angles to X3DAUDIO_2PI and
    // outer cone other than 0 causes
    // the emitter to act like a point emitter using the
    // INNER cone settings only.
    emitter.pCone->OuterAngle = 0.0f;
    // Setting the outer cone angles to zero causes
    // the emitter to act like a point emitter using the
    // OUTER cone settings only.
    emitter.pCone->InnerVolume = 0.0f;
    emitter.pCone->OuterVolume = 1.0f;
    emitter.pCone->InnerLPF = 0.0f;
    emitter.pCone->OuterLPF = 1.0f;
    emitter.pCone->InnerReverb = 0.0f;
    emitter.pCone->OuterReverb = 1.0f;

    emitter.Position = vEmitterPos;
    emitter.OrientFront = D3DXVECTOR3( 0, 0, 1 );
    emitter.OrientTop = D3DXVECTOR3( 0, 1, 0 );
    emitter.ChannelCount = INPUTCHANNELS;
    emitter.ChannelRadius = 1.0f;
    emitter.pChannelAzimuths = emitterAzimuths;

    // Use of Inner radius allows for smoother transitions as
    // a sound travels directly through, above, or below the listener.
    // It also may be used to give elevation cues.
    emitter.InnerRadius = 2.0f;
    emitter.InnerRadiusAngle = X3DAUDIO_PI/4.0f;;

    emitter.pVolumeCurve = (X3DAUDIO_DISTANCE_CURVE*)&X3DAudioDefault_LinearCurve;
    emitter.pLFECurve    = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_LFE_Curve;
    emitter.pLPFDirectCurve = NULL; // use default curve
    emitter.pLPFReverbCurve = NULL; // use default curve
    emitter.pReverbCurve    = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_Reverb_Curve;
    emitter.CurveDistanceScaler = 14.0f;
    emitter.DopplerScaler = 1.0f;

    dspSettings.SrcChannelCount = INPUTCHANNELS;
    dspSettings.DstChannelCount = nChannels;
    dspSettings.pMatrixCoefficients = matrixCoefficients;
}

HRESULT AudioWrapper::PrepareAudio(const LPCWSTR fileName){

	if(!bInitialized)
		return E_FAIL;

	// Se non ho inizializzato nessuna Voice
	if( pSourceVoice )
    {
        pSourceVoice->Stop( 0 );
        pSourceVoice->DestroyVoice();
        pSourceVoice = 0;
    }

	WCHAR strFilePath[ MAX_PATH ];
    

    wcscpy_s( wavFilePath, MAX_PATH, L"Media\\Wavs\\" );
    wcscat_s( wavFilePath, MAX_PATH, fileName );

	// Trovo il File Audio.
    if( FAILED( hr = FindMediaFileCch( strFilePath, MAX_PATH, wavFilePath ) ) )
	{
		wprintf( L"Failed to find media file: %s\n", wavFilePath );
		return hr;
	}

	// Carico il file audio
	CWaveFile wav;

	if( FAILED( hr = wav.Open( strFilePath, NULL, WAVEFILE_READ ) ) )
	{
		wprintf( L"Failed to open media file: %s\n", wavFilePath );
		return hr;
	}

	// Get format of wave file
    WAVEFORMATEX* pwfx = wav.GetFormat();

    // Calculate how many bytes and samples are in the wave
    DWORD cbWaveSize = wav.GetSize();

    // Read the sample data into memory
    SAFE_DELETE_ARRAY( pbSampleData );

    pbSampleData = new BYTE[ cbWaveSize ];


	if (FAILED(hr=wav.Read(pbSampleData,cbWaveSize,&cbWaveSize)) )
	{
		wprintf( L"Failed to read media file: %s\n", wavFilePath );
		return hr;
	}

	//
    // Play the wave using a source voice that sends to both the submix and mastering voices
    //
	XAUDIO2_SEND_DESCRIPTOR sendDescriptors[2];
	sendDescriptors[0].Flags = XAUDIO2_SEND_USEFILTER; // LOW PASS FILTER
    sendDescriptors[0].pOutputVoice = pMasteringVoice;
    sendDescriptors[1].Flags = XAUDIO2_SEND_USEFILTER; // LOW PASS FILTER reverb-path -- omit for better performance at the cost of less realistic occlusion
    sendDescriptors[1].pOutputVoice = pSubmixVoice;
    const XAUDIO2_VOICE_SENDS sendList = { 2, sendDescriptors };

	if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice,pwfx,0,2.0f,NULL,&sendList)))
	{
		wprintf( L"Failed to CreateSourceVoice of : %s\n", wavFilePath );
		return hr;
	}

	// Submit the wave sample data using an XAUDIO2_BUFFER structure
    XAUDIO2_BUFFER buffer = {0};
    buffer.pAudioData = pbSampleData;
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.AudioBytes = cbWaveSize;
    buffer.LoopCount = XAUDIO2_LOOP_INFINITE; // Numero di Loop max 255

    
	if (FAILED(hr = pSourceVoice->SubmitSourceBuffer( &buffer )))
	{
		wprintf( L"Failed to SubmitSourceBuffer of : %s\n", wavFilePath );
		return hr;
	}

  
    nFrameToApply3DAudio = 0;

    return S_OK;

}

HRESULT AudioWrapper::PlayAudio(){
	
	if (FAILED(hr = pSourceVoice->Start( 0 )))
	{
		wprintf( L"Failed to Start  : %s\n", wavFilePath );
		return hr;
	}

}

HRESULT AudioWrapper::UpdateAudio(float fElapsedTime){

	if( !bInitialized )
        return S_FALSE;

	if( nFrameToApply3DAudio == 0 ){


		// Cotrollo se io ascoltatore mi sono spostato
		if( vListenerPos.x != listener.Position.x || vListenerPos.z != listener.Position.z){

			D3DXVECTOR3 vDelta = vListenerPos - listener.Position;

            fListenerAngle = float( atan2( vDelta.x, vDelta.z ) );

            vDelta.y = 0.0f;
            D3DXVec3Normalize( &vDelta, &vDelta );

            listener.OrientFront.x = vDelta.x;
            listener.OrientFront.y = 0.f;
            listener.OrientFront.z = vDelta.z;

		}

		// TODO: Da togliere
		/*Controlli Vari*/
		if (fUseListenerCone) 
        {
            listener.pCone = (X3DAUDIO_CONE*)&Listener_DirectionalCone;
        }
        else
        {
            listener.pCone = NULL;
        }
        if (fUseInnerRadius)
        {
            emitter.InnerRadius = 2.0f;
            emitter.InnerRadiusAngle = X3DAUDIO_PI/4.0f;
        }
        else
        {
            emitter.InnerRadius = 0.0f;
            emitter.InnerRadiusAngle = 0.0f;
        }

		if( fElapsedTime > 0 )
        {
            D3DXVECTOR3 lVelocity = ( vListenerPos - listener.Position ) / fElapsedTime;
            listener.Position = vListenerPos;
            listener.Velocity = lVelocity;

            D3DXVECTOR3 eVelocity = ( vEmitterPos - emitter.Position ) / fElapsedTime;
            emitter.Position = vEmitterPos;
            emitter.Velocity = eVelocity;
        }

        DWORD dwCalcFlags = X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER
            | X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_LPF_REVERB
            | X3DAUDIO_CALCULATE_REVERB;
        if (fUseRedirectToLFE)
        {
            // On devices with an LFE channel, allow the mono source data
            // to be routed to the LFE destination channel.
            dwCalcFlags |= X3DAUDIO_CALCULATE_REDIRECT_TO_LFE;
        }

		 X3DAudioCalculate( x3DInstance, &listener, &emitter, dwCalcFlags,&dspSettings );

		IXAudio2SourceVoice* voice = pSourceVoice;
        if( voice )
        {
            // Apply X3DAudio generated DSP settings to XAudio2
            voice->SetFrequencyRatio( dspSettings.DopplerFactor );
            voice->SetOutputMatrix( pMasteringVoice, INPUTCHANNELS, nChannels,
                                    matrixCoefficients );

            voice->SetOutputMatrix(pSubmixVoice, 1, 1, &dspSettings.ReverbLevel);

            XAUDIO2_FILTER_PARAMETERS FilterParametersDirect = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * dspSettings.LPFDirectCoefficient), 1.0f }; // see XAudio2CutoffFrequencyToRadians() in XAudio2.h for more information on the formula used here
            voice->SetOutputFilterParameters(pMasteringVoice, &FilterParametersDirect);
            XAUDIO2_FILTER_PARAMETERS FilterParametersReverb = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI/6.0f * dspSettings.LPFReverbCoefficient), 1.0f }; // see XAudio2CutoffFrequencyToRadians() in XAudio2.h for more information on the formula used here
            voice->SetOutputFilterParameters(pSubmixVoice, &FilterParametersReverb);
        }

	}

	nFrameToApply3DAudio++;
    nFrameToApply3DAudio &= 1;

    return S_OK;


}

void AudioWrapper::PauseAudio( bool resume )
{
    if( !bInitialized )
        return;

    if( resume )
        pXAudio2->StartEngine();
    else
        pXAudio2->StopEngine();
}

//-----------------------------------------------------------------------------
// Releases XAudio2
//-----------------------------------------------------------------------------
void AudioWrapper::CleanupAudio()
{
    if( !bInitialized )
        return;

    if( pSourceVoice )
    {
        pSourceVoice->DestroyVoice();
        pSourceVoice = NULL;
    }

    if( pSubmixVoice )
    {
        pSubmixVoice->DestroyVoice();
        pSubmixVoice = NULL;
    }

    if( pMasteringVoice )
    {
        pMasteringVoice->DestroyVoice();
        pMasteringVoice = NULL;
    }

    pXAudio2->StopEngine();
    SAFE_RELEASE( pXAudio2 );
    SAFE_RELEASE( pReverbEffect );

    SAFE_DELETE_ARRAY( pbSampleData );

    CoUninitialize();

    bInitialized = false;
}

//HRESULT AudioWrapper::Play(LPCWSTR fileName){
//
//
//	wprintf( L"\nPlaying WAV PCM file..." );
//
//
//	hr = PlayPCM( pXAudio2, fileName );
//
//	if( FAILED( hr )  )
//	{
//		wprintf( L"Failed creating source voice: %#X\n", hr );
//		SAFE_RELEASE( pXAudio2 );
//		return 0;
//	}
//
//	return 1;
//}
//
