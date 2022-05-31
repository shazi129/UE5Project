#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FExEditorToolsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();
	void RegisterSettings();
	void UnregisterSettings();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

	void DoConfigButtonAction(int ID);

	TSharedRef<SWidget> FillComboButton(TSharedPtr<class FUICommandList> Commands);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};