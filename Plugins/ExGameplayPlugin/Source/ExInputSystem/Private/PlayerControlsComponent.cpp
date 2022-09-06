// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerControlsComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "Engine/InputDelegateBinding.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "ExInputSystemModule.h"

void UPlayerControlsComponent::OnRegister()
{
	Super::OnRegister();

	UWorld* World = GetWorld();
	APawn* MyOwner = GetPawn<APawn>();

	if (ensure(MyOwner) && World->IsGameWorld())
	{
		MyOwner->ReceiveRestartedDelegate.AddDynamic(this, &UPlayerControlsComponent::OnPawnRestarted);
		MyOwner->ReceiveControllerChangedDelegate.AddDynamic(this, &UPlayerControlsComponent::OnControllerChanged);

		// If our pawn has an input component we were added after restart
		if (MyOwner->InputComponent)
		{
			OnPawnRestarted(MyOwner);
		}
	}
}

void UPlayerControlsComponent::OnUnregister()
{
	UWorld* World = GetWorld();
	if (World && World->IsGameWorld())
	{
		ReleaseInputComponent();

		APawn* MyOwner = GetPawn<APawn>();
		if (MyOwner)
		{
			MyOwner->ReceiveRestartedDelegate.RemoveAll(this);
			MyOwner->ReceiveControllerChangedDelegate.RemoveAll(this);
		}
	}

	Super::OnUnregister();
}

void UPlayerControlsComponent::OnPawnRestarted(APawn* Pawn)
{
	if (ensure(Pawn && Pawn == GetOwner()) && Pawn->InputComponent)
	{
		ReleaseInputComponent();

		if (Pawn->InputComponent && SetupInputWhenPawnStart)
		{
			SetupInputComponent(Pawn);
		}
	}
}

void UPlayerControlsComponent::OnControllerChanged(APawn* Pawn, AController* OldController, AController* NewController)
{
	// Only handle releasing, restart is a better time to handle binding
	if (ensure(Pawn && Pawn == GetOwner()) && OldController)
	{
		ReleaseInputComponent(OldController);
	}
}

int UPlayerControlsComponent::BindAction(const FInputBindingConfig& BindingConfig)
{
	if (InputComponent == nullptr)
	{
		EXINPUTSYSTEM_LOG(Error, TEXT("UPlayerControlsComponent::BindAction error, Input Component is null"));
		return -1;
	}

	FEnhancedInputActionEventBinding& Binding = InputComponent->BindAction(
		BindingConfig.InputAction,
		BindingConfig.TriggerEvent,
		BindingConfig.InputHandler,
		&UInputBindingActionHandler::NativeExecute);

	int BindingHandle = Binding.GetHandle();
	BindingActionHandles.AddUnique(BindingHandle);

	return BindingHandle;
}

bool UPlayerControlsComponent::UnbindAction(int BindingHandle)
{
	if (InputComponent == nullptr)
	{
		EXINPUTSYSTEM_LOG(Error, TEXT("UPlayerControlsComponent::BindAction error, Input Component is null"));
		return false;
	}

	bool Result = InputComponent->RemoveBindingByHandle(BindingHandle);
	if (Result)
	{
		BindingActionHandles.Remove(BindingHandle);
	}
	else
	{
		EXINPUTSYSTEM_LOG(Error, TEXT("UPlayerControlsComponent::BindAction error, RemoveBindingByHandle return false"));
	}

	return Result;
}

void UPlayerControlsComponent::SetupInputComponent(APawn* Pawn)
{
	InputComponent = CastChecked<UEnhancedInputComponent>(Pawn->InputComponent);
	if (InputComponent == nullptr)
	{
		EXINPUTSYSTEM_LOG(Error, TEXT("UPlayerControlsComponent::SetupInputComponent error, Project must use EnhancedInputComponent to support PlayerControlsComponent"));
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = GetEnhancedInputSubsystem();
	check(Subsystem);

	if (InputMappingContext)
	{
		Subsystem->AddMappingContext(InputMappingContext, InputPriority);
	}

	for (int i = 0; i < BindingActionConfig.Num(); i++)
	{
		FInputBindingConfig& ConfigItem = BindingActionConfig[i];
		if (ConfigItem.InputAction && ConfigItem.InputHandler)
		{
			ConfigItem.InputHandler->SetControlsComponent(this);
			BindAction(ConfigItem);
		}
	}

	isRegister = true;
	SetupPlayerControls(InputComponent);
}

void UPlayerControlsComponent::ReleaseInputComponent(AController* OldController)
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = GetEnhancedInputSubsystem(OldController);
	if (Subsystem && InputComponent)
	{
		TeardownPlayerControls(InputComponent);

		for (int i = 0; i < BindingActionHandles.Num(); i++)
		{
			UnbindAction(BindingActionHandles[i]);
		}

		if (InputMappingContext)
		{
			Subsystem->RemoveMappingContext(InputMappingContext);
		}
	}
	InputComponent = nullptr;
}

void UPlayerControlsComponent::SetupPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent)
{

}

void UPlayerControlsComponent::TeardownPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent)
{

}

UEnhancedInputLocalPlayerSubsystem* UPlayerControlsComponent::GetEnhancedInputSubsystem(AController* OldController) const
{
	const APlayerController* PC = GetController<APlayerController>();
	if (!PC)
	{
		PC = Cast<APlayerController>(OldController);
		if (!PC)
		{
			return nullptr;
		}
	}

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	if (!LP)
	{
		return nullptr;
	}

	return LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
}