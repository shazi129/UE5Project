#pragma once

#include "GameplayBPLibrary.generated.h"

UENUM(Blueprintable)
enum class EBPNetMode : uint8
{
	Standalone = 0					UMETA(DisplayName = "Standalone"),
	DedicatedServer = 1			UMETA(DisplayName = "DedicatedServer"),
	ListenServer = 2				UMETA(DisplayName = "ListenServer"),
	Client = 3						UMETA(DisplayName = "Client"),

	MAX = 4							UMETA(DisplayName = "MAX"),
};

UCLASS()
class UGameplayBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "GameplayBPLibrary")
		static UActorComponent* FindComponentByClass(AActor* Actor, TSubclassOf<UActorComponent> ComponentClass);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameplayBPLibrary")
		static void SetComponentQuat(USceneComponent* Component, const FQuat& NewRotation, bool bSweep, FHitResult& SweepHitResult, bool bTeleport);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameplayBPLibrary")
		static FQuat GetComponentQuat(USceneComponent* Component);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameplayBPLibrary")
		static bool IsNetMode(AActor* Actor, EBPNetMode Mode);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameplayBPLibrary")
		static UWorld* GetCurrentWorld();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameplayBPLibrary", meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
		static UTexture2D* CreateGrayTexture(const UObject* WorldContextObject, const TArray<uint8>& TextureData, int Width, int Height);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameplayBPLibrary")
		static bool GetHitResultAtScreenPosition(APlayerController* PlayerController, const FVector2D ScreenPosition, const ECollisionChannel TraceChannel, bool bTraceComplex, FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameplayBPLibrary")
		static AActor* GetAttachmentRootActor(USceneComponent* SceneComponent);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameplayBPLibrary")
		static FString ArrayToString(const TArray<uint8>& ArrayData);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameplayBPLibrary")
		static TArray<uint8> StringToArray(const FString& StringData);

	UFUNCTION(BlueprintCallable, Category = "GameplayBPLibrary")
		static APlayerController* GetFirstLocalPlayerController(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "GameplayBPLibrary")
		static void ClipboardCopy(const FString& Str);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GameplayBPLibrary")
		static bool IsClient(AActor* Actor);
};
