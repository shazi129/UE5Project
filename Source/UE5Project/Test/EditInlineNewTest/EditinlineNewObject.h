#pragma once

#include "CoreMinimal.h"
#include "EditinlineNewObject.generated.h"

UCLASS(EditInlineNew, BlueprintType, Blueprintable)
class UEditinlineNewObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ObjectName;

	UFUNCTION(BlueprintCallable)
		bool MyBoolBPFunc(int intParam, const FText& strParam);

	bool MyboolNativeFunc();
};
