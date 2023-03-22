/**
 * Copyright 2022 Theai, Inc. (DBA Inworld)
 *
 * Use of this source code is governed by the Inworld.ai Software Development Kit License Agreement
 * that can be found in the LICENSE.md file or at https://www.inworld.ai/sdk-license
 */

#pragma once

#include <string>
#include <memory>
#include <functional>

#include "InworldState.h"
#include "InworldRunnable.h"
#include "AudioSessionDumper.h"

#include "NDK/Utils/SharedQueue.h"
#include "NDK/Packets.h"
#include "NDK/Client.h"
#include "NDK/RunnableCommand.h"

namespace Inworld
{

	class FRunnableAudioDumper : public Inworld::Runnable
	{
	public:
		FRunnableAudioDumper(TQueue<std::string>& InAudioChuncks, const FString& InFileName)
			: AudioChuncks(InAudioChuncks)
			, FileName(InFileName)
		{}

		virtual void Run() override;

	private:

		FAudioSessionDumper AudioDumper;
		TQueue<std::string>& AudioChuncks;
		FString FileName;
	};

	class INWORLDAICLIENT_API FClient : public Inworld::ClientBase
	{
	public:
		FClient()
		{
			CreateAsyncRoutines<FAsyncRoutine>();
		}

		virtual void InitClient(std::string UserId, std::string ClientId, std::string ClientVer, std::function<void(ConnectionState)> ConnectionStateCallback, std::function<void(std::shared_ptr<Inworld::Packet>)> PacketCallback) override;
		virtual void DestroyClient() override;

		virtual std::shared_ptr<DataEvent> SendSoundMessage(const std::string& AgentId, const std::string& Data) override;

	protected:
		virtual void AddTaskToMainThread(std::function<void()> Task) override;

	private:
		std::string GenerateUserId();

		FAsyncRoutine AsyncAudioDumper;
		TQueue<std::string> AudioChunksToDump;
	};
}
