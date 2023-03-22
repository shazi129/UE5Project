/**
 * Copyright 2022 Theai, Inc. (DBA Inworld)
 *
 * Use of this source code is governed by the Inworld.ai Software Development Kit License Agreement
 * that can be found in the LICENSE.md file or at https://www.inworld.ai/sdk-license
 */

#pragma once

#include "CoreMinimal.h"
#include "InworldUtils.h"
#include "InworldPlayerAudioFilter.h"
#include "InworldPlayerAudioCaptureComponent.generated.h"

class UInworldApiSubsystem;
class UInworldPlayerComponent;
class UInworldCharacterComponent;
class USoundWave;
class UAudioCaptureComponent;

struct FPlayerVoiceCaptureInfo
{
    FPlayerVoiceCaptureInfo()
        : MicSoundWave(nullptr)
        , OutputSoundWave(nullptr)
    {}

    FPlayerVoiceCaptureInfo(USoundWave* InMicSoundWave, USoundWave* InOutputSoundWave)
        : MicSoundWave(InMicSoundWave)
        , OutputSoundWave(InOutputSoundWave)
    {}

    USoundWave* MicSoundWave = nullptr;
    USoundWave* OutputSoundWave = nullptr;
};

UCLASS(ClassGroup = (Inworld), meta = (BlueprintSpawnableComponent))
class INWORLDAIINTEGRATION_API UInworldPlayerAudioCaptureComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    UInworldPlayerAudioCaptureComponent();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    void StartVoiceCapture();
    void StopVoiceCapture();
    void StartVoiceChunkCapture();
    void StopVoiceChunkCapture();
    void UpdateVoiceCapture();
    void ProcessVoiceCaptureChunk(FPlayerVoiceCaptureInfo& PlayerVoiceCaptureInfo);

private:
    void OnTargetChanged(UInworldCharacterComponent* Target);

	UPROPERTY(EditAnywhere, Category = "Performance")
	float NotifyVoiceVolumeThreshold = 2.f;

    UPROPERTY(EditAnywhere, Category = "Filter")
    TSubclassOf<UInworldPlayerAudioFilter> PlayerAudioFilterClass;

    UPROPERTY()
    UInworldPlayerAudioFilter* PlayerAudioFilter;

	TWeakObjectPtr<UInworldApiSubsystem> InworldSubsystem;
    TWeakObjectPtr<UInworldPlayerComponent> PlayerComponent;
    TWeakObjectPtr<UAudioCaptureComponent> AudioCaptureComponent;

    FDelegateHandle TargetChangeHandle;

	Inworld::Utils::FWorldTimer SendSoundMessageTimer = Inworld::Utils::FWorldTimer(0.1f);

    TAtomic<bool> bCapturingVoice = false;

    TQueue<FPlayerVoiceCaptureInfo> OutgoingPlayerVoiceCaptureQueue;
	bool bNotifyingVoice = false;
};
