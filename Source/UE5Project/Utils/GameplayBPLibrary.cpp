#include "./GameplayBPLibrary.h"

#include "HAL/PlatformApplicationMisc.h"

UActorComponent* UGameplayBPLibrary::FindComponentByClass(AActor* Actor, TSubclassOf<UActorComponent> ComponentClass)
{
	if (Actor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UGameplayBPLibrary::UGameplayBPLibrary: Actor is null"));
		return nullptr;
	}

	if (ComponentClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UGameplayBPLibrary::UGameplayBPLibrary: ComponentClass is null"));
		return nullptr;
	}

	return Actor->FindComponentByClass(ComponentClass);
}

void UGameplayBPLibrary::SetComponentQuat(USceneComponent* Component, const FQuat& NewRotation, bool bSweep, FHitResult& SweepHitResult, bool bTeleport)
{
	if (!Component)
	{
		UE_LOG(LogTemp, Error, TEXT("UGameplayBPLibrary::SetComponentQuat: Component is null"));
		return;
	}

	Component->SetWorldRotation(NewRotation, bSweep, (bSweep ? &SweepHitResult : nullptr), TeleportFlagToEnum(bTeleport));
}

FQuat UGameplayBPLibrary::GetComponentQuat(USceneComponent* Component)
{
	if (Component == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UGameplayBPLibrary::GetComponentQuat: Component is null"));
		return FQuat();
	}

	return Component->GetComponentQuat();
}

bool UGameplayBPLibrary::IsNetMode(AActor* Actor, EBPNetMode Mode)
{
	return Actor->IsNetMode((ENetMode)Mode);
}

UWorld* UGameplayBPLibrary::GetCurrentWorld()
{
	UWorld* World = GWorld->GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("UGameplayBPLibrary::GetCurrentWorld: World is null"));
		return nullptr;
	}

	//有时会出现上下文不对的情况，例如第一调用是实在ts的Promise内
	//考虑到导出的函数都是给业务层用的，一般都用在游戏中，所以认为这是异常情况
	if (!World->IsGameWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("UGameplayBPLibrary::GetCurrentWorld: Curent is Not A Game World!!!!"));
		return nullptr;
	}

	return World;
}

UTexture2D* UGameplayBPLibrary::CreateGrayTexture(const UObject* WorldContextObject, const TArray<uint8>& TextureData, int Width, int Height)
{
	if (TextureData.Num() < Width * Height)
	{
		UE_LOG(LogTemp, Error, TEXT("UGameplayBPLibrary::CreateGrayTexture: data size not match %d"), TextureData.Num());
		return nullptr;
	}

	UTexture2D* Texture2D = UTexture2D::CreateTransient(Width, Height, PF_A8);
	uint8* Pixels = new uint8[Width * Height];
	for (int i = 0; i < Height; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			int PixelsIndex = Width * i + j;
			if (TextureData[PixelsIndex] <= 255)
			{
				Pixels[PixelsIndex] = TextureData[PixelsIndex];
			}
			else
			{
				Pixels[PixelsIndex] = 255;
			}
		}
	}

	void* TextRawData = Texture2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(TextRawData, Pixels, sizeof(uint8) * Width * Height);
	Texture2D->PlatformData->Mips[0].BulkData.Unlock();

	Texture2D->UpdateResource();

	return Texture2D;
}

bool UGameplayBPLibrary::GetHitResultAtScreenPosition(APlayerController* PlayerController, const FVector2D ScreenPosition, const ECollisionChannel TraceChannel, bool bTraceComplex, FHitResult& HitResult)
{
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UGameplayBPLibrary::GetHitResultAtScreenPosition: PlayerController is null"));
		return false;
	}

	return PlayerController->GetHitResultAtScreenPosition(ScreenPosition, TraceChannel, bTraceComplex, HitResult);
}

AActor* UGameplayBPLibrary::GetAttachmentRootActor(USceneComponent* SceneComponent)
{
	if (SceneComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("UGameplayBPLibrary::GetAttachmentRootActor: SceneComponent is null"));
		return nullptr;
	}

	return SceneComponent->GetAttachmentRootActor();
}

FString UGameplayBPLibrary::ArrayToString(const TArray<uint8>& ArrayData)
{
	FString result(ArrayData.Num(), (const char*)ArrayData.GetData());
	return result;
}

TArray<uint8> UGameplayBPLibrary::StringToArray(const FString& StringData)
{
	uint8* data = (uint8*)TCHAR_TO_UTF8(*StringData);
	return TArray<uint8>(data, StringData.Len());
}

APlayerController* UGameplayBPLibrary::GetFirstLocalPlayerController(UObject* WorldContextObject)
{
	return WorldContextObject->GetWorld()->GetGameInstance()->GetFirstLocalPlayerController();
}

void UGameplayBPLibrary::ClipboardCopy(const FString& Str)
{
	FPlatformApplicationMisc::ClipboardCopy(*Str);
}

bool UGameplayBPLibrary::IsClient(AActor* Actor)
{
	if (!Actor)
	{
		return false;
	}

	UWorld* world = Actor->GetWorld();
	if (!world)
	{
		return false;
	}
	if (world->IsNetMode(NM_Standalone))
	{
		return true;
	}
	else
	{
		return !Actor->HasAuthority();
	}
}
