#pragma once

#include "GameplayBPLibrary.generated.h"


UCLASS()
class UGameplayBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "GameplayBPLibrary")
		static void PrintMenuPath();
};
