#pragma once

#include "TestTypeSystem.generated.h"

UCLASS(BlueprintType, Blueprintable)
class ATestTypeSystem : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay();

private:

	UPROPERTY();
	FString myName;

	UFUNCTION()
	void ParseAllClass();

	UFUNCTION()
	void DumpUClass(TArray<FString>& content, int dumpLevel);

	UFUNCTION()
	void DumpEnum(TArray<FString>& content, int dumpLevel);

	UFUNCTION()
	void DumpStruct(TArray<FString>& content, int dumpLevel);

	UFUNCTION()
	void DumpObject(TArray<FString>& content, int dumpLevel);
};
