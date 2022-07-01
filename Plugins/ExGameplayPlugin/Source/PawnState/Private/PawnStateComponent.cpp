#include "PawnStateComponent.h"

bool UPawnStateComponent::CanEnterState(FGameplayTag NewState)
{
	return true;
}

bool UPawnStateComponent::EnterState(FGameplayTag State)
{
	return true;
}

bool UPawnStateComponent::LeaveState(FGameplayTag State)
{
	return true;
}