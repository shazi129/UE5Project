#include "TestRPCActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "TestRPCComponent.h"


ATestRPCActor* ATestRPCActor::SpawnTestRPCActor(UObject* ContextObject, TSubclassOf<ATestRPCActor> Class)
{
	FTransform Transform;
	ATestRPCActor* RPCActor = Cast<ATestRPCActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(ContextObject, Class, Transform));
	if (!RPCActor)
	{
		UE_LOG(LogTemp, Error, TEXT("ATestRPCActor::SpawnTestRPCActor error"));
		return nullptr;
	}
	RPCActor->Health = 1234;
	RPCActor->PostInitializeComponentsCallback = ([RPCActor](AActor* Actor)
	{
		UTestRPCComponent* RPCComponent = Cast<UTestRPCComponent>(RPCActor->GetComponentByClass(UTestRPCComponent::StaticClass()));
		if (!RPCComponent)
		{
			UE_LOG(LogTemp, Error, TEXT("ATestRPCActor::SpawnTestRPCActor error, cannot find RPCComponent"));
			return;
		}
		RPCComponent->Stamina = 5678;
	});

	return Cast<ATestRPCActor>(UGameplayStatics::FinishSpawningActor(RPCActor, Transform));
}

void ATestRPCActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PostInitializeComponentsDelegate.Broadcast(this);
	if (PostInitializeComponentsCallback)
	{
		PostInitializeComponentsCallback(this);
	}
}

void ATestRPCActor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("ATestRPCActor::BeginPlay :%d"), GetLocalRole());
}

void ATestRPCActor::OnRep_Health(int OldValue)
{
	UE_LOG(LogTemp, Log, TEXT("ATestRPCActor::OnRep_Health :%d --> %d"), OldValue, Health);
}

void ATestRPCActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ATestRPCActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ATestRPCActor, Health, COND_None)
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