// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputInteractComponent.h"
#include "InputReceiverComponent.generated.h"


/**
*  挂在Character身上，管理交互数据
*/
USTRUCT(BlueprintType)
struct EXINPUTSYSTEM_API FInputHandleResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
		bool IsHandled = false;
};

DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(FInputHandleResult, FInputHandleDelegate, const FGameplayTag&, InputTag);

USTRUCT(BlueprintType)
struct EXINPUTSYSTEM_API FInputHandleEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FGameplayTag InputTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FInputHandleDelegate InputHandleDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int Priority = 0;
};


UCLASS(Blueprintable, ClassGroup = (Interact), meta = (BlueprintSpawnableComponent))
class EXINPUTSYSTEM_API UInputReceiverComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	//交互范围
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
		FInteractRange InteractRange;

public:
	// Sets default values for this component's properties
	UInputReceiverComponent();

	virtual void BeginPlay() override;

	FInteractData* FindItem(UInputInteractComponent* Component, const FGameplayTag& InteractType);

	FInteractData* FindOrAddItem(UInputInteractComponent* Component, const FGameplayTag& InteractType);

	//整个响应系统是否可用
	UFUNCTION(BlueprintCallable)
		void SetEnable(bool Enable);

	UFUNCTION(BlueprintCallable)
		bool GetEnable();

	//某个响应项是否可用
	UFUNCTION(BlueprintCallable)
		void SetItemEnable(UInputInteractComponent* Component, const FGameplayTag& InteractType, bool Enable);

	//某个component的所有响应项是否可用
	UFUNCTION(BlueprintCallable)
		void SetComponentEnable(UInputInteractComponent* Component, bool Enable);

	UFUNCTION(BlueprintCallable)
		bool GetItemEnable(UInputInteractComponent* Component, const FGameplayTag& InteractType);

	UFUNCTION(BlueprintCallable)
		bool ReceiveInput(const FGameplayTag& InputTag);

	UFUNCTION(BlueprintNativeEvent)
		bool BP_ReceiveInput(const FGameplayTag& InputTag);

	void UpdateItem(FInteractData& IteractData);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//对可响应的列表进行排序，选出最高优先级的物体
	void SortItems();

	//RPC交互
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
		void ServerInteractItem(const FInteractData& InteractData);

	UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable)
		void ClientInteractItem(const FInteractData& InteractData);

	bool InternalInteractItem(const FInteractData& InteractData);

public:
	//周边组件可交互的信息
	UPROPERTY(BlueprintReadOnly)
		TArray<FInteractData> InteractList;

private:
	//对InteractList进行的排序
	TArray<FInteractData*> InteractOrderList;

private:
	void IgnoreVectorAxis(FVector& Vector, EInteractAngleAxis IgnoreAxis);
};
