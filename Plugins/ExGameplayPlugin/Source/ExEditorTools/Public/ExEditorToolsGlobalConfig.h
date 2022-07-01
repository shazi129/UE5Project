//ExEditorToolsGlobalConfig.h
#pragma once

#include "CoreMinimal.h"
#include "ExEditorToolsGlobalConfig.generated.h"

USTRUCT(BlueprintType)
struct FExEditorToolsDefine
{
	GENERATED_BODY()
public:
	// 用于注册UICommand时的CommandID
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FString CommandName;
	// 用于注册UICommand时的展示名
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FString FriendlyName;
	// 用于注册UICommand时的描述
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FString Description;
	// 该按钮点击时的事件DA路径
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(AllowedClasses="ExEditorActionDefine"))
	FSoftObjectPath Action;
};


UCLASS(config=Game, defaultconfig)
class EXEDITORTOOLS_API UExEditorToolsGlobalConfig : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(Config, EditDefaultsOnly, Category = ExEditorTools)
	TArray<FExEditorToolsDefine> ExEditorToolsDefines;
};