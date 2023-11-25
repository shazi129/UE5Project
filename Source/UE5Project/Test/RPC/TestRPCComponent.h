#pragma once

#include "CoreMinimal.h"
#include "GameplayTags.h"
#include "InstancedStruct.h"
#include "AbilitySystemComponent.h"
#include "TestRPCComponent.generated.h"


UCLASS(BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent))
class UTestRPCComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

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

	UFUNCTION()
		void OnRep_RepIntArray();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable)
	void ServerTestInstancedStruct(const FGameplayTag& MsgTag, const FInstancedStruct& MsgBody);


	UFUNCTION(BlueprintCallable)
		void UpdateGameplayTag(FGameplayTag GameplayTag);

	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable)
		void ServerUpdateGameplayTag(const FGameplayTag& GameplayTag);

public:
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_RepVector)
		FVector RepVector;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_RepIntArray)
		TArray<float> RepIntArray;

public:
	UPROPERTY(EditAnywhere)
		FGameplayTag SendMsgTag;

private:
	UAbilitySystemComponent* ASC;
};