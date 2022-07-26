#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "ExGameplayAbilityTargetTypes.generated.h"

USTRUCT(BlueprintType)
struct EXGAMEPLAYABILITIES_API FGameplayAbilityTargetData_Int64Array : public FGameplayAbilityTargetData
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<int64> Int64Array;

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FGameplayAbilityTargetData_Int64Array::StaticStruct();
	}

	virtual FString ToString() const override {
		return TEXT("FGameplayAbilityTargetData_Int64Array");
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		SafeNetSerializeTArray_Default<64>(Ar, Int64Array);
		bOutSuccess = true;
		return true;
	}
};


template<>
struct TStructOpsTypeTraits<FGameplayAbilityTargetData_Int64Array> : public TStructOpsTypeTraitsBase2<FGameplayAbilityTargetData_Int64Array>
{
	enum {
		WithNetSerializer = true
	};
};