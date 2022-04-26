// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AutoPlayComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AUTOPLAY_API UAutoPlayComponent : public UActorComponent
{
	GENERATED_BODY()

	enum class EAutoPlayState : int32
	{
		Stand = 0,
		Move,
		Turn
	};

	static const TCHAR* EnumToString(EAutoPlayState InCurrentState);

public:	
	// Sets default values for this component's properties
	UAutoPlayComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	void AutoPlay();

	EAutoPlayState CurrentPlayState;

	FTimerHandle AutoPlayTimeHandle;

	float LastDistance = 0.0f;

	FVector OriginLocation;

	UPROPERTY(EditDefaultsOnly, Category="AutoPlay")
	float DistanceLimit = 1000.0f;
};
