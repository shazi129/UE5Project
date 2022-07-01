// Fill out your copyright notice in the Description page of Project Settings.
#include "GameMode/ExCharacter.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/GameFrameworkComponentManager.h"
#include "InputActionValue.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

// Sets default values
AExCharacter::AExCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

// Called when the game starts or when spawned
void AExCharacter::BeginPlay()
{
	Super::BeginPlay();

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	UGameFrameworkComponentManager* ComponentMgr = GameInstance->GetSubsystem<UGameFrameworkComponentManager>();
	ComponentMgr->AddReceiver(this);
}

void AExCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	UGameFrameworkComponentManager* ComponentMgr = GameInstance->GetSubsystem<UGameFrameworkComponentManager>();
	ComponentMgr->RemoveReceiver(this);
}

// Called every frame
void AExCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AExCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(APlayerController* pc = CastChecked<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer()))
		{
			subsystem->AddMappingContext(InputMappingContext, 100);
		}
	}
	
	if(UEnhancedInputComponent* enhancedInputComp = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if(IA_Jump)
		{
			enhancedInputComp->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
			enhancedInputComp->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		}
		
		if(IA_MoveForward)
		{
			enhancedInputComp->BindAction(IA_MoveForward, ETriggerEvent::Triggered, this, &AExCharacter::MoveForward);
			enhancedInputComp->BindAction(IA_MoveRight, ETriggerEvent::Triggered, this, &AExCharacter::MoveRight);
		}
		
		if(IA_Turn)
		{
			enhancedInputComp->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &AExCharacter::PassControllerYawInput);
		}

		if(IA_LookUp)
		{
			enhancedInputComp->BindAction(IA_LookUp, ETriggerEvent::Triggered, this, &AExCharacter::PassControllerPitchInput);
		}
		
		if(IA_TurnRate)
		{
			enhancedInputComp->BindAction(IA_TurnRate, ETriggerEvent::Triggered, this, &AExCharacter::TurnAtRate);
		}
		
		if(IA_LookUpRate)
		{
			enhancedInputComp->BindAction(IA_LookUpRate, ETriggerEvent::Triggered, this, &AExCharacter::LookUpAtRate);
		}
	}
}

void AExCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}


UAbilitySystemComponent* AExCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AExCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AbilitySystemComponent = FindComponentByClass<UAbilitySystemComponent>();
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void AExCharacter::TurnAtRate(const FInputActionValue& inputValue)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(inputValue.GetMagnitude() * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AExCharacter::LookUpAtRate(const FInputActionValue& inputValue)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(inputValue.GetMagnitude() * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AExCharacter::PassControllerYawInput(const FInputActionValue& inputValue)
{
	Super::AddControllerYawInput(inputValue.GetMagnitude());
}

void AExCharacter::PassControllerPitchInput(const FInputActionValue& inputValue)
{
	Super::AddControllerPitchInput(inputValue.GetMagnitude());
}

void AExCharacter::MoveForward(const FInputActionValue& inputValue)
{
	if ((Controller != nullptr) && (inputValue.GetMagnitude() != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, inputValue.GetMagnitude());
	}
}

void AExCharacter::MoveRight(const FInputActionValue& inputValue)
{
	if ((Controller != nullptr) && (inputValue.GetMagnitude() != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, inputValue.GetMagnitude());
	}
}