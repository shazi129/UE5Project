#include "TestRPCComponent.h"
#include "GameFramework/Character.h"

void UTestRPCComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* Owner = GetOwner();
	UE_LOG(LogTemp, Log, TEXT("UTestRPCComponent::BeginPlay, role[%d], NetMode[%d]"), Owner->GetLocalRole(), Owner->GetNetMode());
}

void UTestRPCComponent::StartServerTest()
{
	AActor* Owner = GetOwner();
	UE_LOG(LogTemp, Log, TEXT("UTestRPCComponent::StartServerTest, role[%d]"), Owner->GetLocalRole());

	ServerTest();
}

bool UTestRPCComponent::ServerTest_Validate()
{
	AActor* Owner = GetOwner();
	UE_LOG(LogTemp, Log, TEXT("UTestRPCComponent::ServerTest_Validate, role[%d]"), Owner->GetLocalRole());
	return true;
}

void UTestRPCComponent::ServerTest_Implementation()
{
	AActor* Owner = GetOwner();
	UE_LOG(LogTemp, Log, TEXT("UTestRPCComponent::ServerTest_Implementation, role[%d]"), Owner->GetLocalRole());

	FRPCParamater Result;
	Result.ErrCode = (int)GetOwner()->GetLocalRole();
	Result.ErrMsg = "ServerTest_Implementation return";

	NotifyServerTestResult(Result);
}

bool UTestRPCComponent::NotifyServerTestResult_Validate(const FRPCParamater& Paramater)
{
	AActor* Owner = GetOwner();
	UE_LOG(LogTemp, Log, TEXT("UTestRPCComponent::NotifyServerTestResult_Validate, role[%d]"), Owner->GetLocalRole());
	return true;
}

void UTestRPCComponent::NotifyServerTestResult_Implementation(const FRPCParamater& Paramater)
{
	AActor* Owner = GetOwner();
	UE_LOG(LogTemp, Log, TEXT("UTestRPCComponent::NotifyServerTestResult_Implementation, role[%d], ErrMsg[%d], ErrCode[%s]"), Owner->GetLocalRole(), Paramater.ErrCode, *Paramater.ErrMsg);

}

bool UTestRPCComponent::ServerJump_Validate()
{
	return true;
}

void UTestRPCComponent::ServerJump_Implementation()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		OwnerCharacter->Jump();
	}
}