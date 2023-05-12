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

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void StartServerTest();

	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable)
		void ServerTest(const int IntValue, const FString& StringValue, const FVector& VectorValue);

	UFUNCTION(Reliable, NetMultiCast, WithValidation, BlueprintCallable)
		void NotifyServerTestResult(const FRPCParamater& Paramater);

	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable)
		void ServerJump();

	UFUNCTION()
		void OnRep_RepVector();


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_RepVector)
		FVector RepVector;
};