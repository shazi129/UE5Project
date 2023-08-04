#include "TestRPCComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"

void UTestRPCComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* Owner = GetOwner();
	UE_LOG(LogTemp, Log, TEXT("UTestRPCComponent::BeginPlay, role[%d], NetMode[%d]"), Owner->GetLocalRole(), Owner->GetNetMode());

	SetIsReplicated(true);
}

void UTestRPCComponent::StartServerTest()
{
	AActor* Owner = GetOwner();
	UE_LOG(LogTemp, Log, TEXT("UTestRPCComponent::StartServerTest, role[%d]"), Owner->GetLocalRole());

	RepVector.Set(RepVector.X +1, 0, 0);
	ServerTest(1, "Hello World", FVector(RepVector.X + 1, RepVector.Y + 1, RepVector.Z + 1));
}

bool UTestRPCComponent::ServerTest_Validate(const int IntValue, const FString& StringValue, const FVector& VectorValue)
{
	AActor* Owner = GetOwner();
	UE_LOG(LogTemp, Log, TEXT("UTestRPCComponent::ServerTest_Validate, role[%d]"), Owner->GetLocalRole());
	return true;
}

void UTestRPCComponent::ServerTest_Implementation(const int IntValue, const FString& StringValue, const FVector& VectorValue)
{
	AActor* Owner = GetOwner();
	UE_LOG(LogTemp, Log, TEXT("UTestRPCComponent::ServerTest_Implementation, role[%d]"), Owner->GetLocalRole());

	FRPCParamater Result;
	Result.ErrCode = (int)GetOwner()->GetLocalRole();
	Result.ErrMsg = "ServerTest_Implementation return";

	NotifyServerTestResult(Result);

	RepVector = VectorValue;
	RepIntArray.Add(RepVector.X);
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

void UTestRPCComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(UTestRPCComponent, RepVector, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UTestRPCComponent, RepIntArray, COND_OwnerOnly);
}

void UTestRPCComponent::OnRep_RepVector()
{
	UE_LOG(LogTemp, Log, TEXT("UTestRPCComponent::OnRep_RepVector"));
}

void UTestRPCComponent::OnRep_RepIntArray()
{
	UE_LOG(LogTemp, Log, TEXT("UTestRPCComponent::OnRep_RepIntArray"));
}