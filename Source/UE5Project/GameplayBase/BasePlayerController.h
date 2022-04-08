#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

/**
 *
 */
UCLASS()
class ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
private:
	virtual void BeginPlay() override;
public:
	ABasePlayerController();

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeforeNetCleanUp(UNetConnection* Connection);

	virtual void OnNetCleanup(UNetConnection* Connection) override;
};