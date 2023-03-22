/**
 * Copyright 2022 Theai, Inc. (DBA Inworld)
 *
 * Use of this source code is governed by the Inworld.ai Software Development Kit License Agreement
 * that can be found in the LICENSE.md file or at https://www.inworld.ai/sdk-license
 */

#include "InworldClient.h"
#include "CoreMinimal.h"

#include "SocketSubsystem.h"
#include "IPAddress.h"

#include "InworldUtils.h"

#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"

#include "NDK/Utils/Log.h"

#include <string>


static TAutoConsoleVariable<bool> CVarEnableMicSoundDump(TEXT("Inworld.Debug.EnableSoundDump"), false, TEXT(""));

static TAutoConsoleVariable<FString> CVarSoundDumpPath(TEXT("Inworld.Debug.SoundDumpPath"), TEXT("C:/Tmp/AudioDump.wav"), TEXT(""));


void Inworld::FClient::InitClient(std::string UserId, std::string ClientId, std::string ClientVer, std::function<void(ConnectionState)> ConnectionStateCallback, std::function<void(std::shared_ptr<Inworld::Packet>)> PacketCallback)
{
	ClientBase::InitClient(GenerateUserId(), ClientId, ClientVer, ConnectionStateCallback, PacketCallback);

	if (CVarEnableMicSoundDump.GetValueOnGameThread())
	{
		AsyncAudioDumper.Start("InworldAudioDumper", std::make_unique<FRunnableAudioDumper>(AudioChunksToDump, CVarSoundDumpPath.GetValueOnGameThread()));
	}
}

void Inworld::FClient::DestroyClient()
{
	AsyncAudioDumper.Stop();
	ClientBase::DestroyClient();
}

std::shared_ptr<Inworld::DataEvent> Inworld::FClient::SendSoundMessage(const std::string& AgentId, const std::string& Data)
{
	auto Packet = ClientBase::SendSoundMessage(AgentId, Data);

	if (CVarEnableMicSoundDump.GetValueOnGameThread())
	{
		AudioChunksToDump.Enqueue(Data);
	}

	return Packet;
}

void Inworld::FClient::AddTaskToMainThread(std::function<void()> Task)
{
	AsyncTask(ENamedThreads::GameThread, [Task]() {
		Task();
	});
}

std::string Inworld::FClient::GenerateUserId()
{
	bool CanBindAll;
	TSharedRef<FInternetAddr> LocalIp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, CanBindAll);
	if (!LocalIp->IsValid())
	{
		Inworld::LogError("Couldn't generate user id, local ip isn't valid");
		return "";
	}

	const std::string LocalIpStr = TCHAR_TO_UTF8(*LocalIp->ToString(false));

	TArray<uint8> Data;
	Data.SetNumZeroed(LocalIpStr.size());
	FMemory::Memcpy(Data.GetData(), LocalIpStr.data(), LocalIpStr.size());

	Data = Inworld::Utils::HmacSha256(Data, Data);

	return Utils::ToHex(Data);
}

void Inworld::FRunnableAudioDumper::Run()
{
    AudioDumper.OnSessionStart(TCHAR_TO_UTF8(*FileName));

    while (!_IsDone)
    {
        FPlatformProcess::Sleep(0.1f);

		std::string Chunk;
		while (AudioChuncks.Dequeue(Chunk))
		{
			AudioDumper.OnMessage(Chunk);
		}
    }

    AudioDumper.OnSessionStop();
}
