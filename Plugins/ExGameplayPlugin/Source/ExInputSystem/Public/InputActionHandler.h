#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputActionHandler.generated.h"


UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class EXINPUTSYSTEM_API UInputActionHandler : public UObject
{
	GENERATED_BODY()

public:
	virtual UWorld* GetWorld() const override;

public:
	virtual void NativeExecute(const FInputActionValue& inputValue)
	{
		Execute(inputValue);
	}

	UFUNCTION(BlueprintImplementableEvent)
	void Execute(const FInputActionValue& inputValue);

	UFUNCTION(BlueprintPure)
	inline UObject* GetSourceObject()
	{
		return SourceObject;
	}

	UFUNCTION(BlueprintCallable)
	inline void SetSourceObject(UObject* Object)
	{
		SourceObject = Object;
	}

private:
	UPROPERTY()
		class UPlayerControlsComponent* ControlsComponent;

	UPROPERTY()
	UObject* SourceObject;
};