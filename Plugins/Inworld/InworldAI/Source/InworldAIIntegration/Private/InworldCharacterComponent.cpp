/**
 * Copyright 2022 Theai, Inc. (DBA Inworld)
 *
 * Use of this source code is governed by the Inworld.ai Software Development Kit License Agreement
 * that can be found in the LICENSE.md file or at https://www.inworld.ai/sdk-license
 */

#include "InworldCharacterComponent.h"
#include "NDK/Proto/ProtoDisableWarning.h"
#include "InworldApi.h"
#include "Engine/EngineBaseTypes.h"
#include "NDK/Utils/Utils.h"
#include "NDK/Utils/Log.h"

UInworldCharacterComponent::UInworldCharacterComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    bWantsInitializeComponent = true;
}

void UInworldCharacterComponent::InitializeComponent()
{
    Super::InitializeComponent();

    for (auto& Type : PlaybackTypes)
    {
        auto* Pb = NewObject<UInworldCharacterPlayback>(this, Type);
        Pb->SetOwnerActor(GetOwner());
        Pb->SetCharacterComponent(this);
        Playbacks.Add(Pb);
    }
}

void UInworldCharacterComponent::BeginPlay()
{
    Super::BeginPlay();

    Register();

    for (auto* Pb : Playbacks)
    {
        Pb->BeginPlay();
    }
}

void UInworldCharacterComponent::EndPlay(EEndPlayReason::Type Reason)
{
    for (auto* Pb : Playbacks)
    {
        Pb->EndPlay();
    }

    Unregister();

    Super::EndPlay(Reason);
}

void UInworldCharacterComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    bool bUnqueueNextMessage = true;
    for (auto* Pb : Playbacks)
    {
        bUnqueueNextMessage &= Pb->Update();
    }

    if (!bUnqueueNextMessage)
    {
        return;
    }

	CurrentMessage = nullptr;

	if (PendingMessages.Num() == 0 || !PendingMessages[0]->IsValid())
	{
        return;
	}

	CurrentMessage = PendingMessages[0];
	PendingMessages.RemoveAt(0);

	for (auto* Pb : Playbacks)
	{
		Pb->HandleMessage(CurrentMessage);
	}
}

UInworldCharacterPlayback* UInworldCharacterComponent::GetPlayback(TSubclassOf<UInworldCharacterPlayback> Class) const
{
    for (auto* Pb : Playbacks)
    {
        if (Pb->GetClass()->IsChildOf(Class.Get()))
        {
            return Pb;
        }
    }
    return nullptr;
}

void UInworldCharacterComponent::HandlePacket(std::shared_ptr<Inworld::Packet> Packet)
{
    if (ensure(Packet))
	{
		Packet->Accept(*this);
    }
}

void UInworldCharacterComponent::HandlePlayerTalking(const Inworld::TextEvent& Event)
{
    if (CurrentMessage.IsValid() && CurrentMessage->InteractionId != UTF8_TO_TCHAR(Event._PacketId._InteractionId.c_str()))
    {
        CancelCurrentInteraction();
    }

    Inworld::FCharacterMessageUtterance Message;
    Message.InteractionId = UTF8_TO_TCHAR(Event._PacketId._InteractionId.c_str());
    Message.UtteranceId = UTF8_TO_TCHAR(Event._PacketId._UtteranceId.c_str());
    Message.Text = UTF8_TO_TCHAR(Event.GetText().c_str());
    Message.bTextFinal = Event.IsFinal();

    for (auto* Pb : Playbacks)
    {
        Pb->HandlePlayerTalking(Message);
    }
}

void UInworldCharacterComponent::HandlePlayerInteraction(bool bInteracting)
{
	bInteractingWithPlayer = bInteracting;
	OnPlayerInteractionStateChanged.Broadcast(bInteractingWithPlayer);
}

void UInworldCharacterComponent::CancelCurrentInteraction()
{
    if (!ensure(CurrentMessage.IsValid()))
    {
        return;
    }

	TArray<FString> Utterances;
	Utterances.Reserve(PendingMessages.Num() + 1);
    Utterances.Add(CurrentMessage->UtteranceId);
	for (auto& Message : PendingMessages)
	{
		if (CurrentMessage->InteractionId == Message->InteractionId)
		{
			Utterances.Add(Message->UtteranceId);
		}

		if (!Message->IsSkippable())
		{
			for (auto* Pb : Playbacks)
			{
				Pb->HandleMessage(Message);
			}
		}
	}

	if (Utterances.Num() > 0)
	{
		GetWorld()->GetSubsystem<UInworldApiSubsystem>()->CancelResponse(AgentId, CurrentMessage->InteractionId, Utterances);
	}

	PendingMessages.Empty();
}

void UInworldCharacterComponent::SendTextMessage(const FString& Text) const
{
    if (ensure(!AgentId.IsEmpty()))
    {
        GetWorld()->GetSubsystem<UInworldApiSubsystem>()->SendTextMessage(AgentId, Text);
    }
}

void UInworldCharacterComponent::SendCustomEvent(const FString& Name) const
{
    if (ensure(!AgentId.IsEmpty()))
    {
        GetWorld()->GetSubsystem<UInworldApiSubsystem>()->SendCustomEvent(AgentId, Name);
    }
}

void UInworldCharacterComponent::SendAudioMessage(USoundWave* SoundWave) const
{
    if (ensure(!AgentId.IsEmpty()))
    {
        GetWorld()->GetSubsystem<UInworldApiSubsystem>()->SendAudioMessage(AgentId, SoundWave);
    }
}

void UInworldCharacterComponent::StartAudioSession() const
{
    if (ensure(!AgentId.IsEmpty()))
    {
        GetWorld()->GetSubsystem<UInworldApiSubsystem>()->StartAudioSession(AgentId);
    }
}

void UInworldCharacterComponent::StopAudioSession() const
{
    if (ensure(!AgentId.IsEmpty()))
    {
        GetWorld()->GetSubsystem<UInworldApiSubsystem>()->StopAudioSession(AgentId);
    }
}

bool UInworldCharacterComponent::Register()
{
    if (bRegistered)
    {
        return false;
    }

    if (BrainName.IsEmpty())
    {
        return false;
    }

	auto* InworldSubsystem = GetWorld()->GetSubsystem<UInworldApiSubsystem>();
	if (!ensure(InworldSubsystem))
	{
        return false;
	}

    InworldSubsystem->RegisterCharacterComponent(this);

    bRegistered = true;

    return true;
}

bool UInworldCharacterComponent::Unregister()
{
	if (!bRegistered)
	{
		return false;
	}

	auto* InworldSubsystem = GetWorld()->GetSubsystem<UInworldApiSubsystem>();
	if (!ensure(InworldSubsystem))
	{
		return false;
	}

    InworldSubsystem->UnregisterCharacterComponent(this);

    bRegistered = false;

    return true;
}

bool UInworldCharacterComponent::IsCustomGesture(const FString& CustomEventName) const
{
    return CustomEventName.Find("gesture") == 0;
}

void UInworldCharacterComponent::Visit(const Inworld::TextEvent& Event)
{
	const Inworld::Actor& FromActor = Event._Routing._Source;
	const Inworld::Actor& ToActor = Event._Routing._Target;

    if (ToActor._Type == ai::inworld::packets::Actor_Type_AGENT)
    {
        if (Event.IsFinal())
        {
            Inworld::Log("%s to %s: %s", UTF8_TO_TCHAR(FromActor._Name.c_str()), UTF8_TO_TCHAR(ToActor._Name.c_str()), UTF8_TO_TCHAR(Event.GetText().c_str()));
        }

        HandlePlayerTalking(Event);
    }

    if (FromActor._Type == ai::inworld::packets::Actor_Type_AGENT)
    {
        if (Event.IsFinal())
        {
            Inworld::Log("%s to %s: %s", UTF8_TO_TCHAR(FromActor._Name.c_str()), UTF8_TO_TCHAR(ToActor._Name.c_str()), UTF8_TO_TCHAR(Event.GetText().c_str()));
        }

        if (auto Message = FindOrAddMessage<Inworld::FCharacterMessageUtterance>(Event._PacketId._InteractionId, Event._PacketId._UtteranceId))
        {
            Message->Text = UTF8_TO_TCHAR(Event.GetText().c_str());
            Message->bTextFinal = Event.IsFinal();
        }
    }
}

void UInworldCharacterComponent::Visit(const Inworld::AudioDataEvent& Event)
{
	if (auto Message = FindOrAddMessage<Inworld::FCharacterMessageUtterance>(Event._PacketId._InteractionId, Event._PacketId._UtteranceId))
	{
        Message->AudioData = Event.GetDataChunk();

        auto& PhonemeInfos = Event.GetPhonemeInfos();
        Message->VisemeInfos.Reserve(PhonemeInfos.size());
        for (auto& PhonemeInfo : PhonemeInfos)
        {
            Inworld::FCharacterMessageUtterance::FVisemeInfo& VisemeInfo = Message->VisemeInfos.AddDefaulted_GetRef();
            VisemeInfo.Code = FString(UTF8_TO_TCHAR(Inworld::Utils::PhonemeToViseme(PhonemeInfo.Code).c_str()));
            VisemeInfo.Timestamp = PhonemeInfo.Timestamp;
        }
	}
}

void UInworldCharacterComponent::Visit(const Inworld::SilenceEvent& Event)
{
	if (auto Message = FindOrAddMessage<Inworld::FCharacterMessageSilence>(Event._PacketId._InteractionId, Event._PacketId._UtteranceId))
	{
        Message->Duration = Event.GetDuration();
	}
}

void UInworldCharacterComponent::Visit(const Inworld::ControlEvent& Event)
{
    if (Event.GetControlAction() == ai::inworld::packets::ControlEvent_Action_INTERACTION_END)
    {
		FindOrAddMessage<Inworld::FCharacterMessageInteractionEnd>(Event._PacketId._InteractionId, Event._PacketId._UtteranceId);
    }
}

void UInworldCharacterComponent::Visit(const Inworld::EmotionEvent& Event)
{
    EmotionStrength = static_cast<EInworldCharacterEmotionStrength>(Event.GetStrength());

    const auto Behavior = static_cast<EInworldCharacterEmotionalBehavior>(Event.GetEmotionalBehavior());
    if (Behavior != EmotionalBehavior)
    {
        EmotionalBehavior = Behavior;
        OnEmotionalBehaviorChanged.Broadcast(EmotionalBehavior, EmotionStrength);
    }
}

void UInworldCharacterComponent::Visit(const Inworld::CustomEvent& Event)
{
    if (IsCustomGesture(UTF8_TO_TCHAR(Event.GetName().c_str())))
    {
        if (auto Message = FindOrAddMessage<Inworld::FCharacterMessageUtterance>(Event._PacketId._InteractionId, Event._PacketId._UtteranceId))
        {
            Message->CustomGesture = UTF8_TO_TCHAR(Event.GetName().c_str());
        }
    }
    else
    {
		if (auto Message = FindOrAddMessage<Inworld::FCharacterMessageTrigger>(Event._PacketId._InteractionId, Event._PacketId._UtteranceId))
		{
			Message->Name = UTF8_TO_TCHAR(Event.GetName().c_str());
		}

        Inworld::Log("CustomEvent arrived: %s - %s", UTF8_TO_TCHAR(Event.GetName().c_str()), UTF8_TO_TCHAR(Event._PacketId._InteractionId.c_str()));
    }
}