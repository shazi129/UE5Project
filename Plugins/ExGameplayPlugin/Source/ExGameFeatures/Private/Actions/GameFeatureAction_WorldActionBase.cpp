// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameFeatures/Actions/GameFeatureAction_WorldActionBase.h"
#include "GameFeaturesSubsystemSettings.h"
#include "Engine/Engine.h" // for FWorldContext
#include "GameFeatures/GameFeaturesWorldManager.h"

bool UGameFeatureAction_WorldActionBase::IsWorldHandled(UWorld* InWorld)
{
	if (InWorld == nullptr)
	{
		return false;
	}

	for (int i = 0; i < HandledWorlds.Num(); i++)
	{
		FGameFeatureActionWorldInfo& WorldInfo = HandledWorlds[i];
		if (WorldInfo.WorldEntry == InWorld && WorldInfo.WorldName == InWorld->GetName())
		{
			return true;
		}
	}
	return false;
}

void UGameFeatureAction_WorldActionBase::AddHandledWorld(UWorld* InWorld)
{
	if (InWorld == nullptr)
	{
		return;
	}

	//如果有相同实体，那么world改名字了
	for (int i = 0; i < HandledWorlds.Num(); i++)
	{
		if (HandledWorlds[i].WorldEntry == InWorld)
		{
			HandledWorlds[i].WorldName = InWorld->GetName();
			return;
		}
	}

	//
	FGameFeatureActionWorldInfo* WorldInfo = new (HandledWorlds)FGameFeatureActionWorldInfo();
	WorldInfo->WorldEntry = InWorld;
	WorldInfo->WorldName = InWorld->GetName();
}


void UGameFeatureAction_WorldActionBase::OnGameFeatureActivating()
{
	UE_LOG(LogTemp, Log, TEXT("UGameFeatureAction_WorldActionBase.OnGameFeatureActivating[%s]"), *(GetName()));
	
	const TIndirectArray<FWorldContext>& WorldContextList = GEngine->GetWorldContexts();
	// Add to any worlds with associated game instances that have already been initialized
	for (const FWorldContext& WorldContext : WorldContextList)
	{
		UWorld* World = WorldContext.World();
		if (World->HasBegunPlay())
		{
			HandleWorld(World);
		}
	}

	WorldBeginplayHandle = UGameFeaturesWorldManager::OnWorldBeginplayDelegate.AddUObject(this, &UGameFeatureAction_WorldActionBase::OnWorldBeginplay);
	WorldTearDownHandle = FWorldDelegates::OnWorldBeginTearDown.AddUObject(this, &UGameFeatureAction_WorldActionBase::OnWorldTearDown);
}

void UGameFeatureAction_WorldActionBase::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	UE_LOG(LogTemp, Log, TEXT("UGameFeatureAction_WorldActionBase.OnGameFeatureDeactivating[%s]"), *(GetName()));
	FWorldDelegates::OnStartGameInstance.Remove(GameInstanceStartHandle);
	FWorldDelegates::OnPostWorldInitialization.Remove(WorldCreateHandle);
	FWorldDelegates::OnWorldBeginTearDown.Remove(WorldTearDownHandle);
	UGameFeaturesWorldManager::OnWorldBeginplayDelegate.Remove(WorldBeginplayHandle);
	HandledWorlds.Empty();
}

void UGameFeatureAction_WorldActionBase::HandleWorld(UWorld* InWorld)
{
	if (InWorld == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UGameFeatureAction_WorldActionBase.HandleWorld, InWorld is null"));
		return;
	}

	if (IsWorldHandled(InWorld))
	{
		UE_LOG(LogTemp, Error, TEXT("UGameFeatureAction_WorldActionBase.HandleWorld, InWorld[%s|%p] was Handled"), *(InWorld->GetName()), InWorld);
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("UGameFeatureAction_WorldActionBase.HandleWorld, InWorld[%s|%p]"), *(InWorld->GetName()), InWorld);

	FWorldContext* WorldContext = GEngine->GetWorldContextFromWorld(InWorld);
	AddToWorld(*WorldContext);
	AddHandledWorld(InWorld);
}

void UGameFeatureAction_WorldActionBase::OnWorldBeginplay(UWorld* InWorld)
{
	UE_LOG(LogTemp, Log, TEXT("UGameFeatureAction_WorldActionBase.OnWorldBeginplay[%s], World[%s]"), *(this->GetName()), *(InWorld->GetName()));
	HandleWorld(InWorld);
}

void UGameFeatureAction_WorldActionBase::OnWorldTearDown(UWorld* InWorld)
{
	UE_LOG(LogTemp, Log, TEXT("UGameFeatureAction_WorldActionBase.HandleWorldTearDown[%s], World[%s]"), *(this->GetName()), *(InWorld->GetName()));
}
