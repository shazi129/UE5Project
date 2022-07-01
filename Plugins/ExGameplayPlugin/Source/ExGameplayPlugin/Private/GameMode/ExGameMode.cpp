#include "GameMode/ExGameMode.h"

AExGameMode::AExGameMode()
{

}

void AExGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	OnPreLogin(Options, Address, UniqueId, ErrorMessage);
}

FString AExGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
	const FString& Options, const FString& Portal)
{
	auto Result = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

	PostInitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

	return Result;
}

void AExGameMode::Logout(AController* Exiting)
{
	UE_LOG(LogTemp, Log, TEXT("Controller LogOut called"));
	ReceiveBeforeLogOut(Exiting);
	
	Super::Logout(Exiting);
}
