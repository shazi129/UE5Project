#pragma once
#include "CoreMinimal.h"

template<class T>
T To(const TArray<int8>& Bytes)
{
	T Result;
	size_t size = sizeof(Result);
	check(Bytes.Num() == size)
	FMemory::Memcpy(&Result, Bytes.GetData(), size);
	return Result;
}

template<>
inline FString To(const TArray<int8>& Bytes)
{
	const char* StrData = (const char*)Bytes.GetData();
	return FString(StrData, Bytes.Num());
}

template<class T>
void SetAddressValue(void* DataAddress, T& Value)
{
	*(T*)DataAddress = Value;
}