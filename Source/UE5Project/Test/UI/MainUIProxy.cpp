/*
#include "MainUIProxy.h"
#include "ExpandedUserWidget.h"

void UMainUIProxy::NativeOnInitialized()
{
	FObjectProperty * Property = FindFieldChecked<FObjectProperty>(UserWidget->GetClass(), "TestGFBtn");
	if (Property != nullptr)
	{
		UObject* Object = Property->GetPropertyValue_InContainer(UserWidget);
		TestGFBtn = Cast<UButton>(Object);

		if (TestGFBtn == nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("UMainUIProxy::NativeOnInitialized: Cannot Get TestGFBtn"));
		}
		else
		{
			//FScriptDelegate TestGFBtnDelegate;
			//TestGFBtnDelegate.BindUFunction(this, "OnTestGFBtnClick");
			TestGFBtn->OnClicked.__Internal_AddDynamic(this, &UMainUIProxy::OnTestGFBtnClick, "OnTestGFBtnClick");
		}
	}
}

void UMainUIProxy::OnTestGFBtnClick()
{
	UE_LOG(LogTemp, Log, TEXT("UMainUIProxy::NativeOnInitialized: OnTestGFBtnClick"));
}
*/