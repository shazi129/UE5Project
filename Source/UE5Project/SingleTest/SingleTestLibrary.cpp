#include "SingleTestLibrary.h"
#include "TemplatHeader.h"

void Encode(TArray<int8>& Buffer, const void* Value, size_t Size)
{
	Buffer.SetNumUninitialized(Size);
	FMemory::Memcpy(Buffer.GetData(), Value, Size);
}

void USingleTestLibrary::TestDataConvert()
{
	TArray<int8> Buffer; 
	size_t BufferSize = 0;

	double DoubleValue = 1.23423452f;
	Encode(Buffer, &DoubleValue, sizeof(DoubleValue));
	UE_LOG(LogTemp, Log, TEXT("%s, double: %f -> %f"), *FString(__FUNCTION__), DoubleValue, To<double>(Buffer));

	const char StrValue[] = "hello world";
	Encode(Buffer, StrValue, sizeof(StrValue));
	UE_LOG(LogTemp, Log, TEXT("%s, string: %s -> %s"), *FString(__FUNCTION__), *FString(StrValue), *(To<FString>(Buffer)));

	EMovementMode MovementMode = EMovementMode::MOVE_Falling;
	Encode(Buffer, &MovementMode, sizeof(MovementMode));
	UE_LOG(LogTemp, Log, TEXT("%s, EMovementMode: %d -> %d"), *FString(__FUNCTION__), MovementMode, To<EMovementMode>(Buffer));
}
