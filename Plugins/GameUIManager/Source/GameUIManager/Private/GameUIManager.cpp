#include "GameUIManager.h"
#include "GameUIManagerModule.h"
#include "GameDelegates.h"

TArray<UGameUIManager*>UGameUIManager::Instances;

UGameUIManager* UGameUIManager::GetGameUIManager(const UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		UE_LOG(LogGameUIManager, Error, TEXT("---UGameUIManager GetGameUIManager error, WorldContextObject is null"));
		return nullptr;
	}

	UWorld* World = WorldContextObject->GetWorld();
	//有时会出现上下文不对的情况，例如第一调用是实在ts的Promise内
	if (!World || !World->IsGameWorld())
	{
		UE_LOG(LogGameUIManager, Error, TEXT("---UGameUIManager GetGameUIManager error, Cannot get game world"));
		return nullptr;
	}

	for (int i = Instances.Num() - 1; i >= 0; i--)
	{
		UGameUIManager* Instance = Instances[i];
		if (!Instance || Instance->HasAnyFlags(RF_BeginDestroyed | RF_FinishDestroyed))
		{
			Instances.RemoveAt(i);
		}
		else if (Instance->GetWorld() == World)
		{
			return Instance;
		}
	}

	//
	UGameUIManager* Instance = NewObject<UGameUIManager>(World);
	Instances.Add(Instance);
	return Instance;
}

UGameUIManager::UGameUIManager(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	AddToRoot();

	FWorldDelegates::OnWorldBeginTearDown.AddUObject(this, &UGameUIManager::OnWorldTearingDown);
}

void UGameUIManager::OnWorldTearingDown(UWorld* World)
{
	if (World == this->GetWorld())
	{
		RemoveFromRoot();
	}
}

void UGameUIManager::BeginDestroy()
{
	Super::BeginDestroy();

	if (CurrentPanel != nullptr)
	{
		RemoveCurrentPanel();
	}
	PanelCache.Empty();

	for (int i = Instances.Num() - 1; i >= 0; i--)
	{
		if (this == Instances[i])
		{
			Instances.RemoveAt(i);
			break;
		}
	}
}

void UGameUIManager::SetOwningPlayer(APlayerController* LocalPlayerController)
{
	PlayerController = MakeWeakObjectPtr(LocalPlayerController);
}

void UGameUIManager::SetPanelTemplate(TSubclassOf<UGameUIPanel> panelTemplate)
{
	PanelTemplateCls = panelTemplate;
}

UGameUIPanel* UGameUIManager::FindOrCreateUIPanel(TSubclassOf<UUserWidget> widgetClass, FName panelName, EUIPaneCacheType cacheType)
{
	//如果没有设置，就加载一个默认模板
	if (PanelTemplateCls == nullptr)
	{
		FString defaultPath = TEXT("Blueprint'/GameUIManager/Template/WB_PanelTemplate.WB_PanelTemplate_C'");
		UClass* defaultCls = LoadClass<UGameUIPanel>(this, *defaultPath);
		if (defaultCls != nullptr)
		{
			PanelTemplateCls = defaultCls;
		}
		else
		{
			UE_LOG(LogGameUIManager, Warning, TEXT("---UGameUIManager FindOrCreateUIPanel Error, Panel Template Class is NULL"));
			return nullptr;
		}
	}

	
	if (panelName.IsNone())
	{
		if (widgetClass != nullptr)
		{
			panelName = widgetClass->GetFName();
		}
	}
	if (panelName.IsNone())
	{
		UE_LOG(LogGameUIManager, Warning, TEXT("---UGameUIManager FindOrCreateUIPanel Error, Cannot get panel name"));
		return nullptr;
	}

	UGameUIPanel* panel = nullptr;

	//先通过名字查找
	if (PanelCache.Contains(panelName))
	{
		panel = PanelCache[panelName];

		//可能存在在外部被干掉的情况
		if (panel == nullptr || panel->CacheType == E_NO_CACHE || panel->PanelName.IsNone()
			|| panel->HasAnyFlags(RF_BeginDestroyed | RF_FinishDestroyed))
		{
			PanelCache.Remove(panelName);
			panel = nullptr;
		}
	}
	if (panel)
	{
		return panel;
	}

	//没找到，就要开始创建了
	if (widgetClass == nullptr)
	{
		UE_LOG(LogGameUIManager, Error, TEXT("---UGameUIManager FindOrCreateUIPanel, IN Widget Class is none"));
		return nullptr;
	}

	//创建模板对象
	UWidget* widget = nullptr;
	if (PlayerController != nullptr)
	{
		widget = CreateWidget(PlayerController.Get(), PanelTemplateCls, panelName);
	}
	else
	{
		widget = CreateWidget(GetWorld(), PanelTemplateCls);
	}
	
	if (widget == nullptr)
	{
		UE_LOG(LogGameUIManager, Error, TEXT("---UGameUIManager CreateUIPanel, CreateWidget return null"));
		return nullptr;
	}
	panel = Cast<UGameUIPanel>(widget);
	if (panel == nullptr)
	{
		widget = nullptr;
		UE_LOG(LogGameUIManager, Error, TEXT("---UGameUIManager CreateUIPanel, template is not a UGameUIPanel"));
		return nullptr;
	}

	//panel的名字
	panel->PanelName = panelName;
	panel->CacheType = cacheType;

	//添加内容
	int result = panel->SetPanel(widgetClass);
	if (result != 0) //如果添加失败，删除已创建的模板
	{
		panel->RemoveFromParent();
		panel = nullptr;
		return nullptr;
	}

	//加入缓存
	if (panel->CacheType != E_NO_CACHE)
	{
		PanelCache.Add(panelName, panel);
	}
	return panel;
}

UGameUIPanel* UGameUIManager::FindUIPanelByContentWidget(UUserWidget* content)
{
	for (TPair<FName, UGameUIPanel*> item : PanelCache)
	{
		if (item.Value->PanelContent == content)
		{
			return item.Value;
		}
	}
	return nullptr;
}

UGameUIPanel* UGameUIManager::ShowPanel(TSubclassOf<UUserWidget> widgetCls, FName panelName, EUIPaneCacheType cacheType)
{
	UGameUIPanel* panel = FindOrCreateUIPanel(widgetCls, panelName, cacheType);
	return ShowPanelObject(panel);
}

UGameUIPanel* UGameUIManager::ShowPanelObject(UGameUIPanel* panel)
{
	if (panel == nullptr)
	{
		return nullptr;
	}

	if (panel == CurrentPanel)
	{
		ShowCurrentPanel();
		return panel;
	}

	if (CurrentPanel && CurrentPanel->PanelContent)
	{
		panel->PreviousPanelName = CurrentPanel->PanelName;
		panel->PreviousPanelClass = CurrentPanel->PanelContent->GetClass();
		CurrentPanel->RemoveFromViewport();
		CurrentPanel->RemoveAllPop();
	}
	CurrentPanel = panel;
	CurrentPanel->AddToViewport();

	return panel;
}

void UGameUIManager::RemovePanelObject(UGameUIPanel* panel)
{
	if (panel == nullptr)
	{
		return;
	}

	if (panel == CurrentPanel)
	{
		RemoveCurrentPanel();
	}

	//在cache中找到，删掉它
	FName panelName = "";
	for (TPair<FName, UGameUIPanel*> item : PanelCache)
	{
		if (item.Value->PanelContent == panel)
		{
			panelName = item.Key;
			break;
		}
	}

	if (!panelName.IsNone())
	{
		PanelCache.Remove(panelName);
	}
}

void UGameUIManager::RemoveCurrentPanel()
{
	if (CurrentPanel != nullptr)
	{
		CurrentPanel->RemoveFromViewport();
		CurrentPanel->RemoveAllPop();
		CurrentPanel = nullptr;
	}
}

void UGameUIManager::HideCurrentPanel()
{
	if (CurrentPanel != nullptr)
	{
		CurrentPanel->RemoveFromViewport();
		CurrentPanel->RemoveAllPop();
	}
}

void UGameUIManager::ShowCurrentPanel()
{
	if (CurrentPanel != nullptr && !CurrentPanel->GetParent())
	{
		CurrentPanel->AddToViewport();
	}
}

bool UGameUIManager::HasPreviousPanel()
{
	if (CurrentPanel != nullptr
		&& CurrentPanel->PreviousPanelClass != nullptr)
	{
		return true;
	}
	return false;
}

UGameUIPanel* UGameUIManager::ShowPrevioursPanel()
{
	if (HasPreviousPanel())
	{
		return ShowPanel(CurrentPanel->PreviousPanelClass, CurrentPanel->PreviousPanelName);
	}
	return nullptr;
}

UGameUIPopLayout* UGameUIManager::ShowPopLayout(TSubclassOf<UUserWidget> widgetCls, FPopLayoutParam popParam)
{
	if (CurrentPanel != nullptr)
	{
		return CurrentPanel->ShowPopLayout(widgetCls, popParam);
	}
	return nullptr;
}