#include "ExpandedUserWidget.h"
#include "GameUIManagerModule.h"

UExpandedUserWidget::UExpandedUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UExpandedUserWidget::SetInteractorActor(AActor* Actor)
{
	InteractActor = Actor;
	if (Proxy != nullptr)
	{
		Proxy->OnInteractActorSet(Actor);
	}
}

AActor* UExpandedUserWidget::GetInteractActor()
{
	return InteractActor;
}

bool UExpandedUserWidget::Initialize()
{
	if (Proxy != nullptr)
	{
		Proxy->SetTarget(this);
	}
	return Super::Initialize();
}

void UExpandedUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Proxy != nullptr)
	{
		Proxy->OnWidgetInitialized();
	}
}
void UExpandedUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (Proxy != nullptr)
	{
		Proxy->PreWidgetConstruct();
	}
}

void UExpandedUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Proxy != nullptr)
	{
		Proxy->WidgetConstruct();
	}
}

void UExpandedUserWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (Proxy != nullptr)
	{
		Proxy->WidgetDestruct();
	}
}

void UExpandedUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (Proxy != nullptr)
	{
		Proxy->WidgetTick(MyGeometry, InDeltaTime);
	}
}

void UExpandedUserWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
	if (Proxy != nullptr)
	{
		Proxy->OnLevelRemovedFromWorld(InLevel, InWorld);
	}
}
void UExpandedUserWidget::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();
	if (Proxy != nullptr)
	{
		Proxy->OnWidgetRebuilt();
	}
}

FReply UExpandedUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Replay = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (Replay.IsEventHandled() || Proxy == nullptr)
	{
		return Replay;
	}
	return Proxy->OnMouseButtonDown(InGeometry, InMouseEvent).NativeReply;
}

FReply UExpandedUserWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Replay = Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
	if (Replay.IsEventHandled() || Proxy == nullptr)
	{
		return Replay;
	}
	return Proxy->OnPreviewMouseButtonDown(InGeometry, InMouseEvent).NativeReply;
}

FReply UExpandedUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Replay = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	if (Replay.IsEventHandled() || Proxy == nullptr)
	{
		return Replay;
	}
	return Proxy->OnMouseButtonUp(InGeometry, InMouseEvent).NativeReply;
}

FReply UExpandedUserWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetPressedButtons().Num() > 0)
	{
		UE_LOG(LogGameUIManager, Log, TEXT("---UGameUIManager GetGameUIManager error, WorldContextObject is null"));
	}

	FReply Replay = Super::NativeOnMouseMove(InGeometry, InMouseEvent);
	if (Replay.IsEventHandled() || Proxy == nullptr)
	{
		return Replay;
	}
	return Proxy->OnMouseMove(InGeometry, InMouseEvent).NativeReply;
}

void UExpandedUserWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (Proxy != nullptr)
	{
		Proxy->OnMouseMove(InGeometry, InMouseEvent);
	}
}
void UExpandedUserWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (Proxy != nullptr)
	{
		Proxy->OnMouseLeave(InMouseEvent);
	}
}

FReply UExpandedUserWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Replay = Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
	if (Replay.IsEventHandled() || Proxy == nullptr)
	{
		return Replay;
	}
	return Proxy->OnMouseWheel(InGeometry, InMouseEvent).NativeReply;
}

FReply UExpandedUserWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Replay = Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
	if (Replay.IsEventHandled() || Proxy == nullptr)
	{
		return Replay;
	}
	return Proxy->OnMouseButtonDoubleClick(InGeometry, InMouseEvent).NativeReply;
}