#pragma once

#include "CoreMinimal.h"

#include "TestRPCComponent.generated.h"

USTRUCT(BlueprintType)
struct FRPCParamater
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int ErrCode = 0;

	UPROPERTY(BlueprintReadWrite)
	FString ErrMsg;
};

UCLASS(BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent))
class UTestRPCComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
		void StartServerTest();

	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable)
		void ServerTest();

	UFUNCTION(Reliable, Client, WithValidation, BlueprintCallable)
		void NotifyServerTestResult(const FRPCParamater& Paramater);

};