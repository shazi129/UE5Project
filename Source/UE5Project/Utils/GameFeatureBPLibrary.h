// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFeaturesSubsystem.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameFeatureBPLibrary.generated.h"


UENUM()
enum class EBPGameFeatureState : uint8
{
	Unloaded,
	Loaded,
	Deactivated,
	Activated,

	EGameFeatureState_Max UMETA(Hidden)
};

/**
 * 
 */
UCLASS()
class UGameFeatureBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "GameFeatureBPLibrary")
	static UGameFeaturesSubsystem* GetGameFeatureSubsystem();

	UFUNCTION(BlueprintCallable, Category = "GameFeatureBPLibrary")
	static bool AddToPluginsList( const FString& PluginFilename );
	
	static bool GetPluginURLForBuiltInPluginByName(class UGameFeaturesSubsystem* Subsystem,const FString& PluginName, FString& OutPluginURL);

	UFUNCTION(BlueprintCallable, Category = "GameFeatureBPLibrary")
	static void LoadGameFeature(class UGameFeaturesSubsystem* Subsystem,const FString& PluginURL);

	UFUNCTION(BlueprintCallable, Category = "GameFeatureBPLibrary")
	static void ActiveGameFeature(class UGameFeaturesSubsystem* Subsystem,const FString& PluginURL);
	
	UFUNCTION(BlueprintCallable, Category = "GameFeatureBPLibrary")
	static void LoadBuiltInGameFeaturePlugin(UGameFeaturesSubsystem* Subsystem,const FString& PluginName);

	UFUNCTION(BlueprintCallable, Category = "GameFeatureBPLibrary")
	static void LoadBuiltInGameFeaturePlugins(UGameFeaturesSubsystem* Subsystem);

	UFUNCTION(BlueprintCallable, Category = "GameFeatureBPLibrary")
	static void UnloadGameFeature(UGameFeaturesSubsystem* Subsystem,const FString& PluginURL, bool bKeepRegistered = false);

	UFUNCTION(BlueprintCallable, Category = "GameFeatureBPLibrary")
	static void DeactivateGameFeature(UGameFeaturesSubsystem* Subsystem,const FString& PluginURL);
	
	static void OnStatus(const UE::GameFeatures::FResult& InStatus);

	/* Enable or disable a game feature. */
	UFUNCTION(BlueprintCallable, Category = "GameFeatureBPLibrary")
	static void SetGameFeatureState(FString PluginURL, EBPGameFeatureState State);

};
