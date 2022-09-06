#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputBindingAction.generated.h"


USTRUCT(BlueprintType)
struct EXINPUTSYSTEM_API FInputBindingConfig
{
	GENERATED_BODY()

public:
	//需要绑定的Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TObjectPtr<UInputAction> InputAction;

	//如何触发
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ETriggerEvent TriggerEvent;

	//响应逻辑
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
		UInputBindingActionHandler* InputHandler;
};


UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class EXINPUTSYSTEM_API UInputBindingActionHandler : public UObject
{
	GENERATED_BODY()

public:
	virtual void NativeExecute(const FInputActionValue& inputValue)
	{
		Execute(inputValue);
	}

	UFUNCTION(BlueprintImplementableEvent)
	void Execute(const FInputActionValue& inputValue);

	UFUNCTION(BlueprintCallable)
	inline class UPlayerControlsComponent* GetControlsComponent()
	{
		return ControlsComponent;
	}

	inline void SetControlsComponent(UPlayerControlsComponent* Component)
	{
		ControlsComponent = Component;
	}

private:
	UPROPERTY()
		class UPlayerControlsComponent* ControlsComponent;
};