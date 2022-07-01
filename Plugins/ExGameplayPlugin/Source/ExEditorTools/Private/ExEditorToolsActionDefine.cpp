#include "ExEditorToolsActionDefine.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "IPythonScriptPlugin.h"
#include "AssetRegistry/AssetRegistryModule.h"

void UExEditorAction_OpenWidget::DoAction() const
{
	UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
	if (EditorUtilitySubsystem)
	{
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		UEditorUtilityWidgetBlueprint* Widget = Cast<UEditorUtilityWidgetBlueprint>(WidgetBlueprintPath.TryLoad());
		if (Widget)
		{
			EditorUtilitySubsystem->SpawnAndRegisterTab(Widget);
		}
	}
}

void UExEditorAction_RunPython::DoAction() const
{
	IPythonScriptPlugin::Get()->ExecPythonCommand(*(Script.ToString()));
}
