#include "ExpandedUserWidgetProxy.h"
#include "ExpandedUserWidget.h"

UExpandedUserWidgetProxy::UExpandedUserWidgetProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

UExpandedUserWidget* UExpandedUserWidgetProxy::GetTarget()
{
	return UserWidget;
}

void UExpandedUserWidgetProxy::SetTarget(UExpandedUserWidget* Widget)
{
	UserWidget = Widget;
}