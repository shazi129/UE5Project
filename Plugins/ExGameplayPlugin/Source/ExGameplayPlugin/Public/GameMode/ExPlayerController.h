#pragma once

#include "CoreMinimal.h"
#include "ExPlayerController.generated.h"

/**
 *
 */
UCLASS()
class AExPlayerController : public APlayerController
{
	GENERATED_BODY()
private:
	virtual void BeginPlay() override;
public:
	AExPlayerController();
};