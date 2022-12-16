#include "TestTypeSystem.h"

template<typename... TReturns, typename... TArgs>
void InvokeFunction(UClass* objClass, UObject* obj, UFunction* func, TTuple<TReturns...>& outParams, TArgs&&... args)
{
	objClass = obj != nullptr ? obj->GetClass() : objClass;
	UObject* context = obj != nullptr ? obj : objClass;
	uint8* outPramsBuffer = (uint8*)&outParams;

	if (func->HasAnyFunctionFlags(FUNC_Native)) //quick path for c++ functions
	{
		TTuple<TArgs..., TReturns...> params(Forward<TArgs>(args)..., TReturns()...);
		context->ProcessEvent(func, &params);
		//copy back out params
		for (TFieldIterator<UProperty> i(func); i; ++i)
		{
			UProperty* prop = *i;
			if (prop->PropertyFlags & CPF_OutParm)
			{
				void* propBuffer = prop->ContainerPtrToValuePtr<void*>(&params);
				prop->CopyCompleteValue(outPramsBuffer, propBuffer);
				outPramsBuffer += prop->GetSize();
			}
		}
		return;
	}

	TTuple<TArgs...> inParams(Forward<TArgs>(args)...);
	void* funcPramsBuffer = (uint8*)FMemory_Alloca(func->ParmsSize);
	uint8* inPramsBuffer = (uint8*)&inParams;

	for (TFieldIterator<UProperty> i(func); i; ++i)
	{
		UProperty* prop = *i;
		if (prop->GetFName().ToString().StartsWith("__"))
		{
			//ignore private param like __WolrdContext of function in blueprint funcion library
			continue;
		}
		void* propBuffer = prop->ContainerPtrToValuePtr<void*>(funcPramsBuffer);
		if (prop->PropertyFlags & CPF_OutParm)
		{
			prop->CopyCompleteValue(propBuffer, outPramsBuffer);
			outPramsBuffer += prop->GetSize();
		}
		else if (prop->PropertyFlags & CPF_Parm)
		{
			prop->CopyCompleteValue(propBuffer, inPramsBuffer);
			inPramsBuffer += prop->GetSize();
		}
	}

	context->ProcessEvent(func, funcPramsBuffer);   //call function
	outPramsBuffer = (uint8*)&outParams;    //reset to begin

	//copy back out params
	for (TFieldIterator<UProperty> i(func); i; ++i)
	{
		UProperty* prop = *i;
		if (prop->PropertyFlags & CPF_OutParm)
		{
			void* propBuffer = prop->ContainerPtrToValuePtr<void*>(funcPramsBuffer);
			prop->CopyCompleteValue(outPramsBuffer, propBuffer);
			outPramsBuffer += prop->GetSize();
		}
	}
}


void ATestTypeSystem::BeginPlay()
{
	UE_LOG(LogTemp, Log, TEXT("=========ATestTypeSystem::BeginPlay========="));
	myName = TEXT("myName: TypeSystem");
	ParseAllClass();
}

void ATestTypeSystem::ParseAllClass()
{
	TArray<FString> content;
	//DumpUClass(content, 0);
	//DumpEnum(content, 0);
	//DumpStruct(content, 0);
	DumpObject(content, 0);

	FString filePath = TEXT("E:/dump.txt");// ::ProjectDir() + TEXT("dump/dump.txt");
	//filePath = FPaths::ConvertRelativePathToFull(filePath);

	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();
	if (!file.FileExists(*filePath))
	{
		UE_LOG(LogTemp, Log, TEXT("ATestTypeSystem::SaveToFile, file[%s] not exist"), *filePath);
	}

	bool saveResult = FFileHelper::SaveStringArrayToFile(content, *filePath);
	if (!saveResult)
	{
		UE_LOG(LogTemp, Log, TEXT("ATestTypeSystem::SaveToFile save error!"));
	}
}

void ATestTypeSystem::DumpUClass(TArray<FString>& content, int dumpLevel)
{
	TArray<UObject*> result;
	GetObjectsOfClass(UClass::StaticClass(), result);

	content.Add(FString::Printf(TEXT("===========Dump UClass, num:%d==========="), result.Num()));
	for (int i = 0; i < result.Num(); i++)
	{
		content.Add(result[i]->GetFName().ToString());
	}
}

void ATestTypeSystem::DumpEnum(TArray<FString>& content, int dumpLevel)
{
	TArray<UObject*> result;
	GetObjectsOfClass(UEnum::StaticClass(), result);

	content.Add(FString::Printf(TEXT("===========Dump Enum, num:%d==========="), result.Num()));
	for (int i = 0; i < result.Num(); i++)
	{
		content.Add(result[i]->GetFName().ToString());
	}
}

void ATestTypeSystem::DumpStruct(TArray<FString>& content, int dumpLevel)
{
	TArray<UObject*> result;
	GetObjectsOfClass(UScriptStruct::StaticClass(), result);

	content.Add(FString::Printf(TEXT("===========Dump Struct, num:%d==========="), result.Num()));
	for (int i = 0; i < result.Num(); i++)
	{
		content.Add(result[i]->GetFName().ToString());
	}
}

void ATestTypeSystem::DumpObject(TArray<FString>& content, int dumpLevel)
{
	/*
	AActor* obj = FindObject<AActor>(ANY_PACKAGE, TEXT("TestTypeSystem_1"));
	UClass* uclass = obj->GetClass();
	for (TFieldIterator<UProperty>i(uclass); i; ++i)
	{
		FString propertyName = i->GetName();
		content.Add(propertyName);


		if (i->IsA(UStrProperty::StaticClass()))
		{
			FString* value = i->ContainerPtrToValuePtr<FString>(obj);
			if (value != nullptr)
			{
				content.Add(FString::Printf(TEXT("%s: %s"), *(*propertyName), *(*value)));
			}
		}

		if (propertyName == "myName")
		{
			FString* value = i->ContainerPtrToValuePtr<FString>(obj);
			if (value != nullptr)
			{
				content.Add(FString::Printf(TEXT("%s: %s"), *(*propertyName), *(*value)));
			}
		}
	}

	for (TFieldIterator<UFunction> i(uclass); i; ++i)
	{
		content.Add(i->GetName());
	}
	*/
}