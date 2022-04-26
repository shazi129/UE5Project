// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFeatureBPLibrary.h"
#include "GameFeaturesSubsystem.h"
#include "Interfaces/IPluginManager.h"
#include "Kismet/GameplayStatics.h"

UGameFeaturesSubsystem* UGameFeatureBPLibrary::GetGameFeatureSubsystem()
{
	return &UGameFeaturesSubsystem::Get();
}

bool UGameFeatureBPLibrary::AddToPluginsList(const FString& PluginFilename)
{
	return IPluginManager::Get().AddToPluginsList(PluginFilename);
}

bool UGameFeatureBPLibrary::GetPluginURLForBuiltInPluginByName(class UGameFeaturesSubsystem* Subsystem,const FString& PluginName, FString& OutPluginURL)
{
	return Subsystem->GetPluginURLForBuiltInPluginByName(PluginName,OutPluginURL);
}

void UGameFeatureBPLibrary::LoadGameFeature(UGameFeaturesSubsystem* Subsystem, const FString& PluginURL)
{
	if(!PluginURL.IsEmpty())
	{
		Subsystem->LoadGameFeaturePlugin(PluginURL, FGameFeaturePluginChangeStateComplete::CreateStatic(&UGameFeatureBPLibrary::OnStatus));
	}	
}

void UGameFeatureBPLibrary::ActiveGameFeature(UGameFeaturesSubsystem* Subsystem, const FString& PluginURL)
{
	if(!PluginURL.IsEmpty())
	{
		Subsystem->LoadAndActivateGameFeaturePlugin(PluginURL,FGameFeaturePluginDeactivateComplete::CreateStatic(&UGameFeatureBPLibrary::OnStatus));
	}
}

void UGameFeatureBPLibrary::LoadBuiltInGameFeaturePlugin(UGameFeaturesSubsystem* Subsystem,const FString& PluginName)
{
	TSharedPtr<IPlugin> FoundModule = IPluginManager::Get().FindPlugin(PluginName);

	if (FoundModule.IsValid())
	{
		UGameFeaturesSubsystem::FBuiltInPluginAdditionalFilters Filters = [](const FString& PluginFilename, const FGameFeaturePluginDetails& Details, FBuiltInGameFeaturePluginBehaviorOptions& OutOptions)->bool
		{
			UE_LOG(LogTemp,Log,TEXT("Load Game Feature Plugin %s"),*PluginFilename);
			return true;
		};
		Subsystem->LoadBuiltInGameFeaturePlugin(FoundModule.ToSharedRef(),Filters);
	}
}

void UGameFeatureBPLibrary::LoadBuiltInGameFeaturePlugins(UGameFeaturesSubsystem* Subsystem)
{
	UGameFeaturesSubsystem::FBuiltInPluginAdditionalFilters Filters = [](const FString& PluginFilename, const FGameFeaturePluginDetails& Details, FBuiltInGameFeaturePluginBehaviorOptions& OutOptions)->bool
	{
		UE_LOG(LogTemp, Log, TEXT("Load Game Feature Plugin %s"),*PluginFilename);
		return true;
	};
	Subsystem->LoadBuiltInGameFeaturePlugins(Filters);
}

void UGameFeatureBPLibrary::UnloadGameFeature(UGameFeaturesSubsystem* Subsystem,const FString& PluginURL, bool bKeepRegistered)
{
	Subsystem->UnloadGameFeaturePlugin(PluginURL, bKeepRegistered);
}

void UGameFeatureBPLibrary::DeactivateGameFeature(UGameFeaturesSubsystem* Subsystem, const FString& PluginURL)
{
	Subsystem->DeactivateGameFeaturePlugin(PluginURL);
}


void UGameFeatureBPLibrary::OnStatus(const UE::GameFeatures::FResult& InStatus)
{
	if(InStatus.IsValid() && InStatus.HasError())
	{
		UE_LOG(LogTemp,Log,TEXT("Load Status %s"),**InStatus.TryGetError());
	}	
}


void UGameFeatureBPLibrary::SetGameFeatureState(const FString PluginURL, const EBPGameFeatureState State)
{
	if (State == EBPGameFeatureState::Deactivated)
	{
		UGameFeaturesSubsystem::Get().DeactivateGameFeaturePlugin(PluginURL);
	}

	if (State == EBPGameFeatureState::Activated)
	{
		UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin(PluginURL, FGameFeaturePluginLoadComplete());
	}

	if (State == EBPGameFeatureState::Unloaded)
	{
		UGameFeaturesSubsystem::Get().UnloadGameFeaturePlugin(PluginURL);
	}

	if (State == EBPGameFeatureState::Loaded)
	{
		UGameFeaturesSubsystem::Get().LoadGameFeaturePlugin(PluginURL, FGameFeaturePluginLoadComplete());
	}
}