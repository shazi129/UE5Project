#pragma  once

#include "CoreMinimal.h"
#include "StreamingSourceActor.generated.h"

UCLASS(BlueprintType, Blueprintable)
class AStreamingSourceActor : public AActor, public IWorldPartitionStreamingSourceProvider
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual bool GetStreamingSource(FWorldPartitionStreamingSource& OutStreamingSource) const;
};