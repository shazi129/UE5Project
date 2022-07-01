#include "ExGameplayLibrary.h"
#include "ExGameplayLibraryModule.h"
#include "Misc/DateTime.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/PostProcessVolume.h"

bool UExGameplayLibrary::IsClient(AActor* Actor)
{
	if (!Actor)
	{
		EXLIBRARY_LOG(Error, TEXT("UExGameplayLibrary::IsClient error, Actor is null"));
		return false;
	}

	UWorld* World = Actor->GetWorld();
	if (!World)
	{
		return false;
	}

	ENetMode NetMode = World->GetNetMode();
	if (NetMode == ENetMode::NM_Standalone || NetMode == ENetMode::NM_Client)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int64 UExGameplayLibrary::GetTimestamp()
{
	return FDateTime::Now().ToUnixTimestamp();
}

bool UExGameplayLibrary::ExecCommand(const UObject* WorldContextObject, const FString& Command)
{
	if (WorldContextObject == nullptr)
	{
		UE_LOG(LogExGameplayLibrary, Error, TEXT("UExGameplayLibrary::ExecCommand error, WorldContextObject is null"));
		return false;
	}

	UWorld* World = WorldContextObject->GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogExGameplayLibrary, Error, TEXT("UExGameplayLibrary::ExecCommand error, World is null"));
		return false;
	}

	UEngine* Engine = UGameplayStatics::GetGameInstance(WorldContextObject)->GetEngine();
	if (Engine == nullptr)
	{
		UE_LOG(LogExGameplayLibrary, Error, TEXT("UExGameplayLibrary::ExecCommand error, Engine is null"));
		return false;
	}
	return Engine->Exec(World, *Command);
}

void UExGameplayLibrary::SetLumenEnable(const UObject* WorldContextObject, bool Enable)
{
	TArray<AActor*> PostProcessVolumeActors;
	UGameplayStatics::GetAllActorsOfClass(WorldContextObject, APostProcessVolume::StaticClass(), PostProcessVolumeActors);
	for (int i = 0; i < PostProcessVolumeActors.Num(); i++)
	{
		APostProcessVolume* PostProcessVolume = Cast<APostProcessVolume>(PostProcessVolumeActors[i]);
		if (PostProcessVolume)
		{
			if (Enable)
			{
				PostProcessVolume->Settings.ReflectionMethod = EReflectionMethod::Type::Lumen;
				PostProcessVolume->Settings.DynamicGlobalIlluminationMethod = EDynamicGlobalIlluminationMethod::Type::Lumen;
			}
			else
			{
				PostProcessVolume->Settings.ReflectionMethod = EReflectionMethod::Type::None;
				PostProcessVolume->Settings.DynamicGlobalIlluminationMethod = EDynamicGlobalIlluminationMethod::Type::None;
			}
		}
	}
}