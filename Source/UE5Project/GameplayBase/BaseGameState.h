#pragma once

#include "GameFramework/GameState.h"
#include "BaseGameState.generated.h"


UCLASS(Blueprintable)
class ABaseGameState : public AGameState
{
	GENERATED_BODY()

public:

	ABaseGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostInitializeComponents() override;
};