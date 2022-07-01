#pragma once

#include "GameplayAbilitySpec.h"
#include "Abilities/GameplayAbility.h"
#include "ExGameplayAbility.generated.h"

USTRUCT(BlueprintType)
struct EXGAMEPLAYABILITIES_API FExAbilityCase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		int AbilityLevel = 0;

	UPROPERTY(EditAnywhere, Category = "Abilities")
		FGameplayTag InputTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		TSubclassOf<class UExGameplayAbility> AbilityClass;

	UPROPERTY(instanced, EditAnywhere, AdvancedDisplay, Category = "Abilities")
		class UExGameplayAbility* AbilityTemplate = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ActivateWhenGiven = false;

	//Useful receive FGameplayAbilitySpecHandle after CollectAbilityCases if not null
	//default is null
	FGameplayAbilitySpecHandle* AbilitySpecHandlePtr = nullptr;

};

UCLASS(ClassGroup = (ExAbility), BlueprintType, Blueprintable, abstract, editinlinenew)
class EXGAMEPLAYABILITIES_API UExGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	//本技能需要响应的输入
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FGameplayTagContainer AcceptInputTag;

	UPROPERTY(EditAnywhere, Category = "Abilities")
		FGameplayTag InputTag;
};