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

inline FString ConstructFString(const char* StrData)
{
	FString Result;
	auto& Data = Result.GetCharArray();

	int32 SrcLen = TCString<char>::Strlen(StrData) + 1;
	int32 DestLen = FPlatformString::ConvertedLength<TCHAR>(StrData, SrcLen);
	Data.Reserve(DestLen + 10);
	Data.AddUninitialized(DestLen);
	FPlatformString::Convert(Data.GetData(), DestLen, StrData, SrcLen);

	return Result;
}

template<>
inline FString To(const TArray<int8>& Bytes)
{
	const char* StrData = nullptr;
	int StrNum = 0;

	if (Bytes.Num() == 0 || Bytes.Last() != 0)
	{
		TArray<int8> NewBytes(Bytes.GetData(), Bytes.Num() + 1);
		NewBytes[Bytes.Num()] = 0;
		StrNum = NewBytes.Num();
		StrData = reinterpret_cast<const char*>(NewBytes.GetData());
	}
	else
	{
		StrNum = Bytes.Num();
		StrData = reinterpret_cast<const char*>(Bytes.GetData());
	}
	return FString(StrData, Bytes.Num());
}

template<class T>
void SetAddressValue(void* DataAddress, T& Value)
{
	*(T*)DataAddress = Value;
}