#include "TestRPCActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

void ATestRPCActor::BeginPlay()
{
	Super::BeginPlay();
}

void ATestRPCActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ATestRPCActor::ServerTest()
{
	UE_LOG(LogTemp, Log, TEXT("ATestRPCActor::ServerTest :%d"), GetLocalRole());

	if (!GetOwner() && !HasAuthority())
	{
		ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, 0);
		if (Character)
		{
			SetOwner(Character);
		}
	}

	RPC_ServerTest();
}

void ATestRPCActor::RPC_ServerTest_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("ATestRPCActor::RPC_ServerTest_Implementation :%d"), GetLocalRole());
}