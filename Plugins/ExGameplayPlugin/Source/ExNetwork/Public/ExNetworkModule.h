#pragma once

#include "UObject/UObjectGlobals.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogExNetwork, Log, All);

class FExNetworkModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;

	virtual void ShutdownModule() override;
};