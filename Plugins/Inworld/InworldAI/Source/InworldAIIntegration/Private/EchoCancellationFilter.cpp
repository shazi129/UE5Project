/**
 * Copyright 2022 Theai, Inc. (DBA Inworld)
 *
 * Use of this source code is governed by the Inworld.ai Software Development Kit License Agreement
 * that can be found in the LICENSE.md file or at https://www.inworld.ai/sdk-license
 */

#include "EchoCancellationFilter.h"
#include "webrtc/aec.h"
#include "InworldPlayerAudioCaptureComponent.h"
#include <GenericPlatform/GenericPlatformMath.h>

UEchoCancellationFilter::UEchoCancellationFilter()
    : Super()
	, AecHandle(nullptr)
{

}

void UEchoCancellationFilter::BeginPlay_Implementation()
{
#ifdef INWORLD_WEB_RTC
	AecHandle = WebRtcAec3_Create(16000);
#endif
}

void UEchoCancellationFilter::EndPlay_Implementation()
{
#ifdef INWORLD_WEB_RTC
	WebRtcAec3_Free(AecHandle);
#endif
}

std::vector<int16> UEchoCancellationFilter::FilterAudio(const FPlayerVoiceCaptureInfo& PlayerVoiceCaptureInfo)
{
#ifdef INWORLD_WEB_RTC
	std::vector<int16> MicData, OutputData;
	if (Inworld::Utils::SoundWaveToVec(PlayerVoiceCaptureInfo.MicSoundWave, MicData) && Inworld::Utils::SoundWaveToVec(PlayerVoiceCaptureInfo.OutputSoundWave, OutputData))
	{
		std::vector<int16> FilteredAudio = MicData;
		constexpr int32 NumSamples = 160;
		const int32 MaxSamples = FMath::Min(MicData.size(), OutputData.size()) / NumSamples * NumSamples;

		for (int32 i = 0; i < MaxSamples; i += NumSamples)
		{
			WebRtcAec3_BufferFarend(AecHandle, OutputData.data() + i);
			WebRtcAec3_Process(AecHandle, MicData.data() + i, FilteredAudio.data() + i);
		}
		return FilteredAudio;
	}
	else
	{
		return Super::FilterAudio(PlayerVoiceCaptureInfo);
	}
#else
	return Super::FilterAudio(PlayerVoiceCaptureInfo);
#endif
}
