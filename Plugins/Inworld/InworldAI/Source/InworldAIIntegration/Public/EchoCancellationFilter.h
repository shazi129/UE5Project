/**
 * Copyright 2022 Theai, Inc. (DBA Inworld)
 *
 * Use of this source code is governed by the Inworld.ai Software Development Kit License Agreement
 * that can be found in the LICENSE.md file or at https://www.inworld.ai/sdk-license
 */

#pragma once

#include "CoreMinimal.h"

#include "InworldPlayerAudioFilter.h"
#include "EchoCancellationFilter.generated.h"

UCLASS(BlueprintType, Blueprintable)
class INWORLDAIINTEGRATION_API UEchoCancellationFilter : public UInworldPlayerAudioFilter
{
	GENERATED_BODY()

public:
	UEchoCancellationFilter();

	virtual void BeginPlay_Implementation() override;

	virtual void EndPlay_Implementation() override;

	virtual std::vector<int16> FilterAudio(const struct FPlayerVoiceCaptureInfo& PlayerVoiceCaptureInfo) override;

private:
	void* AecHandle;
};
