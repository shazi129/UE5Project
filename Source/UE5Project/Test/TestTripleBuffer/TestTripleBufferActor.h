#pragma once

#include "CoreMinimal.h"
#include "Containers/TripleBuffer.h"
#include "TestTripleBufferActor.generated.h"

struct FMessage
{
	int32 seq;
	int32 value;
};

UCLASS()
class ATestTripleBufferActor : public AActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

private:
	void StartTest();

private:
	TTripleBuffer<FMessage> networkTripleBuffer;
	double startTime;
};