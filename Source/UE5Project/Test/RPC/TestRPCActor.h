#pragma once

#include "CoreMinimal.h"

#include "TestRPCActor.generated.h"


UCLASS(BlueprintType, Blueprintable)
class ATestRPCActor : public AActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
		void ServerTest();

	UFUNCTION(Server, Reliable, BlueprintCallable)
		void RPC_ServerTest();

};