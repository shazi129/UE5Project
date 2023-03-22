/**
 * Copyright 2022 Theai, Inc. (DBA Inworld)
 *
 * Use of this source code is governed by the Inworld.ai Software Development Kit License Agreement
 * that can be found in the LICENSE.md file or at https://www.inworld.ai/sdk-license
 */

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <vector>
#include "InworldPlayerAudioFilter.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract)
class INWORLDAIINTEGRATION_API UInworldPlayerAudioFilter : public UObject
{
	GENERATED_BODY()
	
public:
	UInworldPlayerAudioFilter();

	/**
	 * Use for initializing
	 */
	UFUNCTION(BlueprintNativeEvent)
	void BeginPlay();

	/**
	 * Use for deinitializing
	 */
	UFUNCTION(BlueprintNativeEvent)
	void EndPlay();

	virtual std::vector<int16> FilterAudio(const struct FPlayerVoiceCaptureInfo& PlayerVoiceCaptureInfo);
};
