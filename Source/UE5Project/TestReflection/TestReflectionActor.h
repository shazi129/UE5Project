#pragma once

#include "CoreMinimal.h"
#include "TestReflectionActor.generated.h"


UCLASS(BlueprintType)
class ATestReflectionActor : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		static void SetObjectIntValue(UObject* Target, FName PropertyName, int Value);

	UFUNCTION(BlueprintCallable)
		static void SetObjectStringValue(UObject* Target, FName PropertyName, FString Value);

	UFUNCTION(BlueprintCallable)
		static void SetObjectVectorValue(UObject* Target, FName PropertyName, FVector Value);

	UFUNCTION(BlueprintCallable)
		static void SetObjectIntArrayValue(UObject* Target, FName PropertyName, TArray<int> Value);

	UFUNCTION(BlueprintCallable)
		static void DebugObject(UObject* Target);

public:
	UFUNCTION()
	void Debug();

	UPROPERTY(BlueprintReadWrite)
		int IntValue;

	UPROPERTY(BlueprintReadWrite)
		FString StrValue;

	UPROPERTY(BlueprintReadWrite)
		FVector VecValue;

	UPROPERTY(BlueprintReadWrite)
		TArray<int> ArrayValue;
};