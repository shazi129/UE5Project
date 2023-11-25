#include "TestRPCComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "PawnStateTypes.h"

void UTestRPCComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* Owner = GetOwner();
	UE_LOG(LogTemp, Log, TEXT("UTestRPCComponent::BeginPlay, role[%d], NetMode[%d]"), Owner->GetLocalRole(), Owner->GetNetMode());

	ASC = Cast<UAbilitySystemComponent>(GetOwner()->GetComponentByClass(UAbilitySystemComponent::StaticClass()));

	SetIsReplicated(true);
}

void UTestRPCComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTestRPCComponent::StartServerTest()
{
	AActor* Owner = GetOwner();
	UE_LOG(LogTemp, Log, TEXT("UTestRPCComponent::StartServerTest, role[%d]"), Owner->GetLocalRole());

	RepVector.Set(RepVector.X +1, 0, 0);
	ServerTest(1, "Hello World", FVector(RepVector.X + 1, RepVector.Y + 1, RepVector.Z + 1));

	FInstancedStruct MsgBody;
	MsgBody.InitializeAs<FRPCParamater>();
	FRPCParamater& Paramter = MsgBody.GetMutable<FRPCParamater>();
	Paramter.ErrCode = 9527;
	Paramter.ErrMsg = "asdfasdfasdfasdfasdf";
	ServerTestInstancedStruct(SendMsgTag, MsgBody);
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

bool UTestRPCComponent::ServerTestInstancedStruct_Validate(const FGameplayTag& MsgTag, const FInstancedStruct& MsgBody)
{
	return true;
}

void UTestRPCComponent::ServerTestInstancedStruct_Implementation(const FGameplayTag& MsgTag, const FInstancedStruct& MsgBody)
{
	UE_LOG(LogTemp, Log, TEXT("%s MsgTag:%s, MsgBody:%s"), *FString(__FUNCTION__), *GetNameSafe(MsgBody.GetScriptStruct()));
}

void UTestRPCComponent::OnRep_RepVector()
{
	UE_LOG(LogTemp, Log, TEXT("UTestRPCComponent::OnRep_RepVector"));
}

void UTestRPCComponent::OnRep_RepIntArray()
{
	UE_LOG(LogTemp, Log, TEXT("UTestRPCComponent::OnRep_RepIntArray"));
}

void UTestRPCComponent::UpdateGameplayTag(FGameplayTag GameplayTag)
{
	ENetRole Role = GetOwner()->GetLocalRole();

	if (ASC && Role == ENetRole::ROLE_AutonomousProxy)
	{
		ServerUpdateGameplayTag(GameplayTag);
	}
}

bool UTestRPCComponent::ServerUpdateGameplayTag_Validate(const FGameplayTag& GameplayTag)
{
	return true;
}

void UTestRPCComponent::ServerUpdateGameplayTag_Implementation(const FGameplayTag& GameplayTag)
{
	if (ASC)
	{
		ASC->UpdateTagMap(GameplayTag, 1);
	}
}
