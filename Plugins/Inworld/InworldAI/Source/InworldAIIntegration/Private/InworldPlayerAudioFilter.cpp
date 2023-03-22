/**
 * Copyright 2022 Theai, Inc. (DBA Inworld)
 *
 * Use of this source code is governed by the Inworld.ai Software Development Kit License Agreement
 * that can be found in the LICENSE.md file or at https://www.inworld.ai/sdk-license
 */

#include "InworldPlayerAudioFilter.h"
#include "InworldPlayerAudioCaptureComponent.h"

#include "InworldUtils.h"

UInworldPlayerAudioFilter::UInworldPlayerAudioFilter()
	: Super()
{

}

void UInworldPlayerAudioFilter::BeginPlay_Implementation()
{

}

void UInworldPlayerAudioFilter::EndPlay_Implementation()
{

}

std::vector<int16> UInworldPlayerAudioFilter::FilterAudio(const FPlayerVoiceCaptureInfo& PlayerVoiceCaptureInfo)
{
	std::vector<int16> MicData;
	if (PlayerVoiceCaptureInfo.MicSoundWave)
	{
		Inworld::Utils::SoundWaveToVec(PlayerVoiceCaptureInfo.MicSoundWave, MicData);
	}
	return MicData;
}
