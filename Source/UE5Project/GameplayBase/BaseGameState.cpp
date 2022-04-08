#include "./BaseGameState.h"

ABaseGameState::ABaseGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ABaseGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}