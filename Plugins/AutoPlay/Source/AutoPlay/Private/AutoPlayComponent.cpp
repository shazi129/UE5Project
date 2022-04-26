// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoPlayComponent.h"
#include "GameFramework/Character.h"
#include "AutoPlayModule.h"

const TCHAR* UAutoPlayComponent::EnumToString(EAutoPlayState InCurrentState)
{
	switch (InCurrentState)
	{
	case EAutoPlayState::Stand:
		return TEXT("Stand");
	case EAutoPlayState::Move:
		return TEXT("Move");
	case EAutoPlayState::Turn:
		return TEXT("Turn");
	}
	ensure(false);
	return TEXT("Unknown");
}

// Sets default values for this component's properties
UAutoPlayComponent::UAutoPlayComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAutoPlayComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* Character = this->GetOwner<ACharacter>();
	if (Character == nullptr)
	{
		UE_LOG(LogAutoPlay, Error, TEXT("BeginPlay error, Owner Character is null"));
		return;
	}

	ENetRole Role = Character->GetLocalRole();
	if ((Character->IsNetMode(NM_Standalone) && Role == ROLE_Authority) || Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		OriginLocation = Character->GetActorLocation();
		OriginLocation.Z = 0.0f;
		GetWorld()->GetTimerManager().SetTimer(AutoPlayTimeHandle, this, &UAutoPlayComponent::AutoPlay, 3.0f, false, 3.0f);
	}
}


void UAutoPlayComponent::BeginDestroy()
{
	Super::BeginDestroy();

	//if (AutoPlayTimeHandle.IsValid())
	//{
	//	GetWorld()->GetTimerManager().ClearTimer(AutoPlayTimeHandle);
	//}
}

// Called every frame
void UAutoPlayComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ACharacter* Character = this->GetOwner<ACharacter>();
	if (!Character)
	{
		return;
	}
	AController* Controller = Character->GetController();
	if (!Controller)
	{
		return;
	}
	switch (this->CurrentPlayState)
	{
		case EAutoPlayState::Stand:
		break;
		case EAutoPlayState::Move:
		{
			FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
			Character->AddMovementInput(Direction, 1.0f);
		}
		break;
		case EAutoPlayState::Turn:
		{
			Character->AddControllerYawInput(1.0f * 45.f * GetWorld()->GetDeltaSeconds());
		}
		break;
		default:
		break;
	}
}

void UAutoPlayComponent::AutoPlay()
{
	EAutoPlayState NextState = EAutoPlayState(FMath::RandRange(0, 2));
	this->CurrentPlayState = NextState;

	ACharacter* Character = this->GetOwner<ACharacter>();
	if (Character)
	{
		FVector Location = Character->GetActorLocation();
		Location.Z = 0.0f;
		float Distance = (this->OriginLocation-Location).Size();
		if (this->DistanceLimit < Distance)
		{
			if (this->LastDistance < Distance)
			{
				this->CurrentPlayState = EAutoPlayState::Turn;
			}
			else
			{
				this->CurrentPlayState = EAutoPlayState::Move;
			}
		}
		this->LastDistance = Distance;
	}

	UE_LOG(LogAutoPlay, Log, TEXT("CurrentPlayState: %s"), UAutoPlayComponent::EnumToString(this->CurrentPlayState));

	switch (this->CurrentPlayState)
	{
		case EAutoPlayState::Stand:
		{
			GetWorld()->GetTimerManager().SetTimer(AutoPlayTimeHandle, this, &UAutoPlayComponent::AutoPlay, 2.0f, false);
		}
		break;
		case EAutoPlayState::Move:
		{
			GetWorld()->GetTimerManager().SetTimer(AutoPlayTimeHandle, this, &UAutoPlayComponent::AutoPlay, 3.0f, false);
		}
		break;
		case EAutoPlayState::Turn:
		{
			GetWorld()->GetTimerManager().SetTimer(AutoPlayTimeHandle, this, &UAutoPlayComponent::AutoPlay, 1.0f, false);
		}
		break;
		default:
		{
			GetWorld()->GetTimerManager().SetTimer(AutoPlayTimeHandle, this, &UAutoPlayComponent::AutoPlay, 3.0f, false);
		}
		break;
	}
}

