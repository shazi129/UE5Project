#pragma once

#include "CoreMinimal.h"
#include "SingleTestLibrary.generated.h"


UCLASS()
class USingleTestLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Sigle Test")
	static void TestDataConvert();
};