#pragma once

#include "GameFramework/GameState.h"
#include "ExGameState.generated.h"


UCLASS(Blueprintable)
class AExGameState : public AGameState
{
	GENERATED_BODY()

public:

	AExGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostInitializeComponents() override;
};