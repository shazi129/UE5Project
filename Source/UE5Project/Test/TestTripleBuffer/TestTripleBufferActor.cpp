#include "TestTripleBufferActor.h"


void ATestTripleBufferActor::BeginPlay()
{
	Super::BeginPlay();

	startTime = FPlatformTime::Seconds();
	StartTest();
}



void ATestTripleBufferActor::StartTest()
{
	
	auto readFunc = [&]()
	{
		while (true)
		{
			const FMessage& msg = networkTripleBuffer.SwapAndRead();
			UE_LOG(LogTemp, Log, TEXT("read seq[%d] value[%d]"), msg.seq, msg.value);

			if (FPlatformTime::Seconds() - startTime > 10)
			{
				break;
			}

			FPlatformProcess::Sleep(0.1);
		}
	};

	auto writeFunc = [&]()
	{
		int seq = 0;
		while (true)
		{
			FMessage& msg = networkTripleBuffer.GetWriteBuffer();
			seq++;
			msg.seq = seq;
			msg.value = seq;

			UE_LOG(LogTemp, Log, TEXT("write seq[%d] value[%d]"), msg.seq, msg.value);
			networkTripleBuffer.SwapWriteBuffers();

			if (FPlatformTime::Seconds() - startTime > 10)
			{
				break;
			}

			FPlatformProcess::Sleep(0.1);
		}
	};

	Async(EAsyncExecution::Thread, MoveTemp(readFunc));
	Async(EAsyncExecution::Thread, MoveTemp(writeFunc));
}