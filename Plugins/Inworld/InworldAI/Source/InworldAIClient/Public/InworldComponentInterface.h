#pragma once

#include "CoreMinimal.h"
#include "InworldState.h"
#include "NDK/Packets.h"

namespace Inworld
{
	class ICharacterComponent
	{
	public:
		virtual void SetAgentId(const FString& InAgentId) = 0;
		virtual const FString& GetAgentId() const = 0;
		virtual void SetGivenName(const FString& InGivenName) = 0;
		virtual const FString& GetGivenName() const = 0;
		virtual const FString& GetBrainName() const = 0;
		virtual void HandlePacket(std::shared_ptr<Inworld::Packet> Packet) = 0;
		virtual AActor* GetComponentOwner() const = 0;
	};

	class IPlayerComponent
	{
	public:
		virtual ICharacterComponent* GetTargetCharacter() = 0;
	};
}
