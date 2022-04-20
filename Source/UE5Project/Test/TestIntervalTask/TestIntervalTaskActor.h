#pragma once

#include "CoreMinimal.h"
#include "TestIntervalTaskActor.generated.h"

UCLASS()
class ATestIntervalTaskActor : public AActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void IntervalTask();

private:
	FTimerHandle TimeHandle;
};