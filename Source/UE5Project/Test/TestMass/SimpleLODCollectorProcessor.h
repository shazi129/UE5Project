#pragma once

#include "CoreMinimal.h"
#include "MassLODCollectorProcessor.h"
#include "SimpleLODCollectorProcessor.generated.h"

UCLASS()
class USimpleLODCollectorProcessor : public UMassLODCollectorProcessor
{
	GENERATED_BODY()

public:
	USimpleLODCollectorProcessor();
};