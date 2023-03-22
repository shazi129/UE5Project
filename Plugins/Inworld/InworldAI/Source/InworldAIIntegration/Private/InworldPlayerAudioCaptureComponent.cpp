/**
 * Copyright 2022 Theai, Inc. (DBA Inworld)
 *
 * Use of this source code is governed by the Inworld.ai Software Development Kit License Agreement
 * that can be found in the LICENSE.md file or at https://www.inworld.ai/sdk-license
 */

#include "InworldPlayerAudioCaptureComponent.h"
#include "InworldPlayerComponent.h"
#include "AudioCaptureComponent.h"
#include "AudioMixerBlueprintLibrary.h"
#include "AudioMixerDevice.h"
#include "AudioMixerSubmix.h"
#include "InworldApi.h"
#include "InworldAIPlatformModule.h"
#include "NDK/Utils/Log.h"
#include "GenericPlatform/GenericPlatformApplicationMisc.h"

UInworldPlayerAudioCaptureComponent::UInworldPlayerAudioCaptureComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bTickEvenWhenPaused = true;
}

void UInworldPlayerAudioCaptureComponent::BeginPlay()
{
    Super::BeginPlay();

	InworldSubsystem = GetWorld()->GetSubsystem<UInworldApiSubsystem>();

    PlayerComponent = Cast<UInworldPlayerComponent>(GetOwner()->GetComponentByClass(UInworldPlayerComponent::StaticClass()));
    if (ensureMsgf(PlayerComponent.IsValid(), TEXT("UInworldPlayerAudioCaptureComponent::BeginPlay: add InworldPlayerComponent.")))
    {
        TargetChangeHandle = PlayerComponent->OnTargetChange.AddUObject(this, &UInworldPlayerAudioCaptureComponent::OnTargetChanged);
    }
    
    FInworldAIPlatformModule& PlatformModule = FModuleManager::Get().LoadModuleChecked<FInworldAIPlatformModule>("InworldAIPlatform");
    Inworld::Platform::IMicrophone* Microphone = PlatformModule.GetMicrophone();
    if(Microphone->GetPermission() == Inworld::Platform::Permission::UNDETERMINED)
    {
        Microphone->RequestAccess([](bool Granted)
        {
            ensureMsgf(Granted, TEXT("UInworldPlayerAudioCaptureComponent::BeginPlay: Platform has denied access to microphone."));
        });
    }
    else
    {
        const bool Granted = Microphone->GetPermission() == Inworld::Platform::Permission::GRANTED;
        ensureMsgf(Granted, TEXT("UInworldPlayerAudioCaptureComponent::BeginPlay: Platform has denied access to microphone."));
    }

    AudioCaptureComponent = Cast<UAudioCaptureComponent>(GetOwner()->GetComponentByClass(UAudioCaptureComponent::StaticClass()));

    if (PlayerAudioFilterClass)
    {
        PlayerAudioFilter = NewObject<UInworldPlayerAudioFilter>(this, PlayerAudioFilterClass);
        PlayerAudioFilter->BeginPlay();
    }
}

void UInworldPlayerAudioCaptureComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (PlayerComponent.IsValid())
    {
        PlayerComponent->OnTargetChange.Remove(TargetChangeHandle);
    }

    if (PlayerAudioFilter)
    {
        PlayerAudioFilter->EndPlay();
        PlayerAudioFilter = nullptr;
    }

    Super::EndPlay(EndPlayReason);
}

void UInworldPlayerAudioCaptureComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (TickType == ELevelTick::LEVELTICK_PauseTick || InworldSubsystem->GetConnectionState() != EInworldConnectionState::Connected)
    {
        StopVoiceCapture();
        return;
    }

    // restore voice capture after pause
	if (PlayerComponent->GetTargetCharacter() && !bCapturingVoice)
	{
        StartVoiceCapture();
	}

	UpdateVoiceCapture();
}

void UInworldPlayerAudioCaptureComponent::StartVoiceCapture()
{
    if (bCapturingVoice)
    {
        return;
    }

    if (!ensure(PlayerComponent->GetTargetCharacter()))
    {
        Inworld::LogError("UInworldPlayerAudioCaptureComponent::StartVoiceCapture: TargetAgentId is empty.");
        return;
    }

	if (!ensure(AudioCaptureComponent.IsValid()))
	{
		Inworld::LogError("UInworldPlayerAudioCaptureComponent::StartVoiceCapture: AudioCaptureComponent is empty.");
		return;
	}

	AudioCaptureComponent->Start();
    PlayerComponent->StartAudioSessionWithTarget();
    bCapturingVoice = true;
}

void UInworldPlayerAudioCaptureComponent::StopVoiceCapture()
{
    if (!bCapturingVoice)
    {
        return;
    }

    bCapturingVoice = false;
	AudioCaptureComponent->Stop();
    PlayerComponent->StopAudioSessionWithTarget();
}

void UInworldPlayerAudioCaptureComponent::StartVoiceChunkCapture()
{
    UAudioMixerBlueprintLibrary::StartRecordingOutput(this, SendSoundMessageTimer.GetThreshold(), Cast<USoundSubmix>(AudioCaptureComponent->SoundSubmix));
    
    if (PlayerAudioFilter)
    {
        UAudioMixerBlueprintLibrary::StartRecordingOutput(this, SendSoundMessageTimer.GetThreshold());
    }
}

void UInworldPlayerAudioCaptureComponent::StopVoiceChunkCapture()
{
    USoundWave* SoundWaveMic = UAudioMixerBlueprintLibrary::StopRecordingOutput(this, EAudioRecordingExportType::SoundWave, "InworldVoiceCapture", "InworldVoiceCapture", Cast<USoundSubmix>(AudioCaptureComponent->SoundSubmix));
    if (SoundWaveMic && SoundWaveMic->GetDuration() < SendSoundMessageTimer.GetThreshold() * 2.f)
    {
        if (PlayerAudioFilter)
        {
            USoundWave* SoundWaveOutput = UAudioMixerBlueprintLibrary::StopRecordingOutput(this, EAudioRecordingExportType::SoundWave, "InworldOutputCapture", "InworldOutputCapture");
            OutgoingPlayerVoiceCaptureQueue.Enqueue({ SoundWaveMic, SoundWaveOutput });
        }
        else
        {
            OutgoingPlayerVoiceCaptureQueue.Enqueue({ SoundWaveMic, nullptr });
        }
    }
}

void UInworldPlayerAudioCaptureComponent::UpdateVoiceCapture()
{
    if (bCapturingVoice && SendSoundMessageTimer.CheckPeriod(GetWorld()))
    {
        FAudioThread::RunCommandOnAudioThread([this]()
        {
            StopVoiceChunkCapture();
            StartVoiceChunkCapture();
        });
    }

    FPlayerVoiceCaptureInfo NextPlayerAudioChunk;
    if (OutgoingPlayerVoiceCaptureQueue.Dequeue(NextPlayerAudioChunk))
    {
        ProcessVoiceCaptureChunk(NextPlayerAudioChunk);
        
    }
}

void UInworldPlayerAudioCaptureComponent::ProcessVoiceCaptureChunk(FPlayerVoiceCaptureInfo& PlayerVoiceCaptureInfo)
{
    if (PlayerAudioFilter)
    {
        std::vector<int16> SoundData = PlayerAudioFilter->FilterAudio(PlayerVoiceCaptureInfo);

        std::string StrData;
        StrData.resize(SoundData.size() * sizeof(int16));
        FMemory::Memcpy((void*)StrData.data(), (void*)SoundData.data(), StrData.size());

        PlayerComponent->SendAudioDataMessageToTarget(StrData);
    }
    else
    {
        PlayerComponent->SendAudioMessageToTarget(PlayerVoiceCaptureInfo.MicSoundWave);
    }
}

void UInworldPlayerAudioCaptureComponent::OnTargetChanged(UInworldCharacterComponent* Target)
{
    if (Target)
    {
        StartVoiceCapture();
    }
    else
    {
        StopVoiceCapture();
    }
}

