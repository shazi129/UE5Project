#include "TestIntervalTaskActor.h"

void ATestIntervalTaskActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimeHandle, this, &ATestIntervalTaskActor::IntervalTask, 1.0f, true, 2.0f);
}

void ATestIntervalTaskActor::IntervalTask()
{
	UE_LOG(LogTemp, Log, TEXT("ATestIntervalTaskActor::IntervalTask"));
}