#include "ExAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/SkeletalMeshComponent.h"
#include "ExGameplayAbilitiesModule.h"
#include "ExGameplayLibrary.h"

void UExAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if (AbilityActorInfo)
	{
		if (AbilityActorInfo->AnimInstance == nullptr)
		{
			AbilityActorInfo->AnimInstance = AbilityActorInfo->GetAnimInstance();
		}

		if (UGameInstance* GameInstance = InOwnerActor->GetGameInstance())
		{
			// Sign up for possess/unpossess events so that we can update the cached AbilityActorInfo accordingly
			//GameInstance->GetOnPawnControllerChanged().AddDynamic(this, &UExAbilitySystemComponent::OnPawnControllerChanged);
		}
	}

	//GrantDefaultAbilitiesAndAttributes();
}

void UExAbilitySystemComponent::BeginDestroy()
{
	if (AbilityActorInfo && AbilityActorInfo->OwnerActor.IsValid())
	{
		if (UGameInstance* GameInstance = AbilityActorInfo->OwnerActor->GetGameInstance())
		{
			GameInstance->GetOnPawnControllerChanged().RemoveAll(this);
		}
	}

	Super::BeginDestroy();
}

void UExAbilitySystemComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(UExAbilitySystemComponent, InputTags, COND_AutonomousOnly);
}

// FGameplayAbilitySpecHandle UExAbilitySystemComponent::GrantAbilityOfType(TSubclassOf<UGameplayAbility> AbilityType, bool bRemoveAfterActivation)
// {
// 	FGameplayAbilitySpecHandle AbilityHandle;
// 	if (AbilityType)
// 	{
// 		FGameplayAbilitySpec AbilitySpec(AbilityType);
// 		AbilitySpec.RemoveAfterActivation = bRemoveAfterActivation;
// 
// 		AbilityHandle = GiveAbility(AbilitySpec);
// 	}
// 	return AbilityHandle;
// }
// 
// void UExAbilitySystemComponent::GrantDefaultAbilitiesAndAttributes()
// {
// 	// Reset/Remove abilities if we had already added them
// 	{
// 		for (UAttributeSet* AttribSetInstance : AddedAttributes)
// 		{
// 			GetSpawnedAttributes_Mutable().Remove(AttribSetInstance);
// 		}
// 
// 		for (FGameplayAbilitySpecHandle AbilityHandle : DefaultAbilityHandles)
// 		{
// 			SetRemoveAbilityOnEnd(AbilityHandle);
// 		}
// 
// 		AddedAttributes.Empty(DefaultAttributes.Num());
// 		DefaultAbilityHandles.Empty(DefaultAbilities.Num());
// 	}
// 
// 	// Default abilities
// 	{
// 		DefaultAbilityHandles.Reserve(DefaultAbilities.Num());
// 		for (const TSubclassOf<UGameplayAbility>& Ability : DefaultAbilities)
// 		{
// 			if (*Ability)
// 			{
// 				DefaultAbilityHandles.Add(GiveAbility(FGameplayAbilitySpec(Ability)));
// 			}
// 		}
// 	}
// 
// 	// Default attributes
// 	{
// 		for (const FExAttributeApplication& Attributes : DefaultAttributes)
// 		{
// 			if (Attributes.AttributeSetType)
// 			{
// 				UAttributeSet* NewAttribSet = NewObject<UAttributeSet>(this, Attributes.AttributeSetType);
// 				if (Attributes.InitializationData)
// 				{
// 					NewAttribSet->InitFromMetaDataTable(Attributes.InitializationData);
// 				}
// 				AddedAttributes.Add(NewAttribSet);
// 				AddAttributeSetSubobject(NewAttribSet);
// 			}
// 		}
// 	}
// }

void UExAbilitySystemComponent::OnPawnControllerChanged(APawn* Pawn, AController* NewController)
{
	if (AbilityActorInfo && AbilityActorInfo->OwnerActor == Pawn && AbilityActorInfo->PlayerController != NewController)
	{
		// Reinit the cached ability actor info (specifically the player controller)
		AbilityActorInfo->InitFromActor(AbilityActorInfo->OwnerActor.Get(), AbilityActorInfo->AvatarActor.Get(), this);
	}
}

FGameplayAbilitySpecHandle UExAbilitySystemComponent::GiveAbilityByCase(const FExAbilityCase& AbilityCase, UObject* AbilityProvider)
{
	return GiveAbilityByCaseInternal(AbilityCase, AbilityProvider);
}

FGameplayAbilitySpecHandle UExAbilitySystemComponent::GiveAbilityByCaseInternal(const FExAbilityCase& AbilityCase, UObject* AbilityProvider)
{
	if (!AbilityProvider)
	{
		AbilityProvider = GetOwnerActor();
	}

	UExGameplayAbility* const GameplayAbility = AbilityCase.AbilityTemplate ? AbilityCase.AbilityTemplate : AbilityCase.AbilityClass.GetDefaultObject();

	if (ensureMsgf(GameplayAbility != nullptr, TEXT("GiveAbilityByCase Error! AbilityCase.AbilityClass is empty")))
	{
		//如果Case没填，则使用实例上默认配置的InputTag
		FGameplayTag InputTag = AbilityCase.InputTag;
		if (!InputTag.IsValid())
		{
			InputTag = GameplayAbility->InputTag;
		}

		FGameplayAbilitySpec Spec(const_cast<UExGameplayAbility*>(GameplayAbility), AbilityCase.AbilityLevel, InputTagToIDOrGenerate(InputTag), (AbilityProvider));
		const FGameplayAbilitySpecHandle& SpecHandle = GiveAbility(Spec);

		for (int i = 0; i < GameplayAbility->AcceptInputTag.Num(); i++)
		{
			InputTags.AddUnique(GameplayAbility->AcceptInputTag.GetByIndex(i));
		}

		if (AbilityCase.AbilitySpecHandlePtr)
		{
			*AbilityCase.AbilitySpecHandlePtr = SpecHandle;
		}
		if (AbilityCase.ActivateWhenGiven)
		{
			TryActivateAbility(SpecHandle);
		}
		return SpecHandle;
	}
	return FGameplayAbilitySpecHandle();
}

void UExAbilitySystemComponent::TryActivateAbilityOnceWithEventData_Implementation(const FExAbilityCase& AbilityCase, const FGameplayEventData& TriggerEventData, UObject* SourceObj)
{
	//带EventData的不在客户端激活
	if (!GetOwner()->HasAuthority())
	{
		EXABILITY_LOG(Error, TEXT("UExAbilitySystemComponent::TryActivateAbilityOnceWithEventData error, Cannot Activate in Client"));
		return;
	}

	//通过class 找到handler
	UClass* AbilityClass = AbilityCase.AbilityClass;
	if (AbilityCase.AbilityTemplate != nullptr)
	{
		AbilityClass = AbilityCase.AbilityTemplate->StaticClass();
	}
	if (AbilityClass == nullptr)
	{
		EXABILITY_LOG(Error, TEXT("UExAbilitySystemComponent::TryActivateAbilityOnceWithEventData error, Ability Class is NULL"));
		return;
	}

	FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromClass(AbilityClass);
	if (AbilitySpec == nullptr || AbilitySpec->Handle.IsValid() == false)
	{
		EXABILITY_LOG(Error, TEXT("UExAbilitySystemComponent::TryActivateAbilityOnceWithEventData error, Ability[%s] was not given"), *GetNameSafe(AbilityClass));
		return;
	}

	bool Activated = InternalTryActivateAbility(AbilitySpec->Handle, FPredictionKey(), nullptr, nullptr, &TriggerEventData);
	EXABILITY_LOG(Verbose, TEXT("TryActivateAbilityOnce Ability:%s Activated:%d"), *GetNameSafe(AbilityCase.AbilityClass.Get()), (int)Activated);
}

bool UExAbilitySystemComponent::TryActivateAbilityOnceWithEventData_Validate(const FExAbilityCase& AbilityCase, const FGameplayEventData& TriggerEventData, UObject* SourceObj)
{
	return true;
}

#pragma region /////////////////////////////////////Code for handling Ability Input ////////////////////////////////////////

int UExAbilitySystemComponent::InputTagToIDOrGenerate(const FGameplayTag& InputTag)
{
	return InputTags.AddUnique(InputTag);
}

#pragma endregion

