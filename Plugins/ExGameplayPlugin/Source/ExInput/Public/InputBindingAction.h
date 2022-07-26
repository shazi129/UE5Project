#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputBindingAction.generated.h"


USTRUCT(BlueprintType)
struct EXINPUT_API FInputBindingConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TObjectPtr<UInputAction> InputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ETriggerEvent TriggerEvent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
		UInputBindingActionHandler* InputHandler;
};


UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class EXINPUT_API UInputBindingActionHandler : public UObject
{
	GENERATED_BODY()

public:
	virtual void NativeExecute(const FInputActionValue& inputValue)
	{
		Execute(inputValue);
	}

	UFUNCTION(BlueprintImplementableEvent)
	void Execute(const FInputActionValue& inputValue);
};