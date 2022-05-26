#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PawnStateComponent.generated.h"

UCLASS(BlueprintType, Blueprintable)
class UPawnStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		bool CanEnterState(FGameplayTag State);

	UFUNCTION(BlueprintCallable)
		bool EnterState(FGameplayTag State);

	UFUNCTION(BlueprintCallable)
		bool LeaveState(FGameplayTag State);
};