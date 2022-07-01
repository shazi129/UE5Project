// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "Delegates/DelegateCombinations.h"
#include "GameFramework/Actor.h"
#include "ExGameplayAbility.h"
#include "ExAttributeSet.h"
#include "ExAbilitySystemComponent.generated.h"

USTRUCT()
struct FExAttributeApplication
{
	GENERATED_BODY()

		// Ability set to grant
		UPROPERTY(EditAnywhere)
		TSubclassOf<UExAttributeSet> AttributeSetType;

	// Data table reference to initialize the attributes with, if any (can be left unset)
	UPROPERTY(EditAnywhere)
		UDataTable* InitializationData = nullptr;
};

UCLASS(Blueprintable, ClassGroup = AbilitySystem, editinlinenew, meta = (BlueprintSpawnableComponent))
class EXGAMEPLAYABILITIES_API UExAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	virtual void BeginDestroy() override;

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:

	UFUNCTION()
		void OnPawnControllerChanged(APawn* Pawn, AController* NewController);

	UFUNCTION(BlueprintCallable, Category = ExAbility)
		FGameplayAbilitySpecHandle GiveAbilityByCase(const FExAbilityCase& AbilityCase, UObject* AbilityProvider = nullptr);

	UFUNCTION(Server, reliable, WithValidation)
		virtual void TryActivateAbilityOnceWithEventData(const FExAbilityCase& AbilityCase, const FGameplayEventData& TriggerEventData, UObject* SourceObj);

private:
	FGameplayAbilitySpecHandle GiveAbilityByCaseInternal(const FExAbilityCase& AbilityCase, UObject* AbilityProvider);


#pragma region /////////////////////////////////////Code for handling Ability Input ////////////////////////////////////////

public:
	UFUNCTION(BlueprintCallable)
	TArray<FGameplayTag> GetInputTags() const
	{
		return InputTags;
	}

protected:
	virtual int InputTagToIDOrGenerate(const FGameplayTag& InputTag);

protected:
	UPROPERTY(Replicated)
		TArray<FGameplayTag> InputTags;

#pragma endregion


};
