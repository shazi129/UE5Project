#pragma once

#include "CoreMinimal.h"
#include "ExGameplayLibrary.h"

#define DECLARE_LOG_TAG() \
private:\
	FString LogTag; \
	inline void InitLogTag(const TCHAR* ClassName) \
	{\
		this->LogTag = FString::Printf(TEXT("[%s] %s"), *(UExGameplayLibrary::GetLogPrefix(this)), ClassName);\
	}
	
#define  DEFINE_LOG_TAG(ClassName) \
	this->InitLogTag(TEXT(#ClassName))