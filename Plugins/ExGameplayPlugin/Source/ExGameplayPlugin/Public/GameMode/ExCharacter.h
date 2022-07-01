// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "ExCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class EXGAMEPLAYPLUGIN_API AExCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AExCharacter();

public:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Ability
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PreInitializeComponents() override;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** EnhancedInput */
	UPROPERTY(EditDefaultsOnly, Category = Input)
		TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Action")
		TObjectPtr<UInputAction> IA_Jump;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Action")
		TObjectPtr<UInputAction> IA_MoveForward;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Action")
		TObjectPtr<UInputAction> IA_MoveRight;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Action")
		TObjectPtr<UInputAction> IA_Turn;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Action")
		TObjectPtr<UInputAction> IA_TurnRate;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Action")
		TObjectPtr<UInputAction> IA_LookUp;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Action")
		TObjectPtr<UInputAction> IA_LookUpRate;

protected:
	virtual void PostInitializeComponents() override;

	/** Called for forwards/backward input */
	void MoveForward(const FInputActionValue& inputValue);

	/** Called for side to side input */
	void MoveRight(const FInputActionValue& inputValue);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(const FInputActionValue& inputValue);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(const FInputActionValue& inputValue);

	void PassControllerYawInput(const FInputActionValue& inputValue);

	void PassControllerPitchInput(const FInputActionValue& inputValue);


protected:
	class UAbilitySystemComponent* AbilitySystemComponent;

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
};
