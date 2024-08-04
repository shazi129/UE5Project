#pragma once

#include "CoreMinimal.h"

#include "TestRPCActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorDynamicMultiDelegate, AActor*, Actor);

UCLASS(BlueprintType, Blueprintable)
class ATestRPCActor : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static ATestRPCActor* SpawnTestRPCActor(UObject* ContextObject, TSubclassOf<ATestRPCActor> Class);

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents();

public:
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_Health)
	int Health = 0;

	UFUNCTION()
	void OnRep_Health(int OldValue);

	UFUNCTION(BlueprintCallable)
		void ServerTest();

	UFUNCTION(Server, Reliable, BlueprintCallable)
		void RPC_ServerTest();

	TFunction<void(AActor* Actor)> PostInitializeComponentsCallback;

	UPROPERTY(BlueprintAssignable)
	FActorDynamicMultiDelegate PostInitializeComponentsDelegate;
};