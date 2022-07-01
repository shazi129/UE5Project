
#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ExGameMode.generated.h"

UCLASS(minimalapi)
class AExGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AExGameMode();

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	UFUNCTION(BlueprintImplementableEvent)
    void OnPreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage);
	

	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal) override;

	virtual void Logout(AController* Exiting) override;

	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveBeforeLogOut(AController* Exiting);

	UFUNCTION(BlueprintImplementableEvent)
    void PostInitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal);
};


