#include "StreamingSourceActor.h"
#include "WorldPartition/WorldPartitionSubsystem.h"

void AStreamingSourceActor::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (UWorldPartitionSubsystem* WorldPartitionSubsystem = World->GetSubsystem<UWorldPartitionSubsystem>())
	{
		WorldPartitionSubsystem->RegisterStreamingSourceProvider(this);
	}
}

void AStreamingSourceActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UWorld* World = GetWorld();
	if (UWorldPartitionSubsystem* WorldPartitionSubsystem = World->GetSubsystem<UWorldPartitionSubsystem>())
	{
		WorldPartitionSubsystem->UnregisterStreamingSourceProvider(this);
	}
}

bool AStreamingSourceActor::GetStreamingSource(FWorldPartitionStreamingSource& OutStreamingSource) const
{
	OutStreamingSource.Name = GetFName();
	OutStreamingSource.Location = GetActorLocation();
	OutStreamingSource.Rotation = GetActorRotation();
	OutStreamingSource.TargetState = EStreamingSourceTargetState::Activated;
	OutStreamingSource.bBlockOnSlowLoading = false;
	OutStreamingSource.DebugColor = FColor::Red;
	return true;
}