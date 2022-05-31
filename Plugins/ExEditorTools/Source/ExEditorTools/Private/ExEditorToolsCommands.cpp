#include "ExEditorToolsCommands.h"
#include "ExEditorToolsGlobalConfig.h"

#define LOCTEXT_NAMESPACE "FExEditorToolsModule"

void FExEditorToolsCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "ExEditorTools", "Bring up ExEditorTools window", EUserInterfaceActionType::Button, FInputGesture());
	RegisterConfigCommands();
}

void FExEditorToolsCommands::RegisterConfigCommands()
{
	const UExEditorToolsGlobalConfig* Config = GetDefault<UExEditorToolsGlobalConfig>();
	for (const auto& Define : Config->ExEditorToolsDefines)
	{
		TSharedPtr<FUICommandInfo>& Command = ConfigCommands.Add_GetRef(TSharedPtr<FUICommandInfo>());
		MakeUICommand_InternalUseOnly(this, Command, TEXT(LOCTEXT_NAMESPACE), *Define.CommandName, *(Define.CommandName + "_ToolTip"),
			TCHAR_TO_ANSI(*("." + Define.CommandName)), *Define.FriendlyName, *Define.Description, EUserInterfaceActionType::Button, FInputGesture());
	}
}

#undef LOCTEXT_NAMESPACE