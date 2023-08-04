#include "TestReflectionActor.h"

void ATestReflectionActor::Debug()
{
	FString Result = "\n";
	Result += FString::Printf(TEXT("  IntValue:%d \n"), IntValue);
	Result += FString::Printf(TEXT("  StrValue:%s \n"), *StrValue);
	Result += FString::Printf(TEXT("  VecValue:%s \n"), *VecValue.ToString());
	Result += FString::Printf(TEXT("ArrayValue:%d \n"), ArrayValue.Num());
	UE_LOG(LogTemp, Log, TEXT("%s"), *Result);
}

#define CHECK_TARGET() \
	if (!Target) \
	{ \
		UE_LOG(LogTemp, Log, TEXT("%s error, target is null"), *FString(__FUNCTION__)); \
		return; \
	}

void ATestReflectionActor::SetObjectIntValue(UObject* Target, FName PropertyName, int Value)
{
	CHECK_TARGET()

	FIntProperty* Property = FindFieldChecked<FIntProperty>(Target->GetClass(), PropertyName);
	if (Property != nullptr)
	{
		void* PropertyAddress = Property->ContainerPtrToValuePtr<void>(Target);
		Property->SetIntPropertyValue(PropertyAddress, (int64)Value);
	}
}

void ATestReflectionActor::SetObjectStringValue(UObject* Target, FName PropertyName, FString Value)
{
	CHECK_TARGET()
	FStrProperty* Property = FindFieldChecked<FStrProperty>(Target->GetClass(), PropertyName);
	if (Property != nullptr)
	{
		void* PropertyAddress = Property->ContainerPtrToValuePtr<void>(Target);
		Property->SetPropertyValue(PropertyAddress, (FString)Value);
	}
}

void ATestReflectionActor::SetObjectVectorValue(UObject* Target, FName PropertyName, FVector Value)
{
	CHECK_TARGET()
	FStructProperty* Property = FindFieldChecked<FStructProperty>(Target->GetClass(), PropertyName);
	if (Property != nullptr)
	{
		void* PropertyAddress = Property->ContainerPtrToValuePtr<void>(Target);
		*(FVector*)PropertyAddress = Value;
	}
}

void ATestReflectionActor::SetObjectIntArrayValue(UObject* Target, FName PropertyName, TArray<int> Value)
{
	CHECK_TARGET()
	FArrayProperty* Property = FindFieldChecked<FArrayProperty>(Target->GetClass(), PropertyName);
	if (Property != nullptr)
	{
		if (FNumericProperty* NumericProperty = Cast<FNumericProperty>(Property->Inner))
		{
			if (NumericProperty->IsInteger())
			{
				void* PropertyAddress = Property->ContainerPtrToValuePtr<void>(Target);
				*((TArray<int32>*)PropertyAddress) = Value;
			}
		}
	}
}

void ATestReflectionActor::DebugObject(UObject* Target)
{
	if (UFunction* Function = Target->FindFunction(FName("Debug")))
	{
		if (Function)
		{
			// 如果函数没有参数，你可以直接调用
			Target->ProcessEvent(Function, nullptr);
		}
	}
}