#pragma once

#include "ExGameplayLibrary.generated.h"

UCLASS()
class EXGAMEPLAYLIBRARY_API UExGameplayLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "ExGameplayLibrary")
		static bool IsClient(AActor* Actor);

	UFUNCTION(BlueprintPure, Category = "ExGameplayLibrary")
		static int64 GetTimestamp();

	UFUNCTION(BlueprintCallable, Category = "ExGameplayLibrary", meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
		static bool ExecCommand(const UObject* WorldContextObject, const FString& Command);

	UFUNCTION(BlueprintCallable, Category = "ExGameplayLibrary", meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
		static void SetLumenEnable(const UObject* WorldContextObject, bool Enable);
};
