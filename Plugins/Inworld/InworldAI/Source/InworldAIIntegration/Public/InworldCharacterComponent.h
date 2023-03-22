/**
 * Copyright 2022 Theai, Inc. (DBA Inworld)
 *
 * Use of this source code is governed by the Inworld.ai Software Development Kit License Agreement
 * that can be found in the LICENSE.md file or at https://www.inworld.ai/sdk-license
 */

#pragma once

#include "CoreMinimal.h"
#include "InworldApi.h"
#include "Components/ActorComponent.h"
#include "InworldComponentInterface.h"
#include "InworldCharacterPlayback.h"
#include "InworldCharacterMessage.h"
#include "InworldCharacterEnums.h"
#include "NDK/Packets.h"

#include "InworldCharacterComponent.generated.h"

UCLASS(ClassGroup = (Inworld), meta = (BlueprintSpawnableComponent))
class INWORLDAIINTEGRATION_API UInworldCharacterComponent : public UActorComponent, public Inworld::PacketVisitor, public Inworld::ICharacterComponent
{
	GENERATED_BODY()

public:
	UInworldCharacterComponent();

	virtual void InitializeComponent() override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInworldCharacterEmotionalBehaviorChanged, EInworldCharacterEmotionalBehavior, EmotionalBehavior, EInworldCharacterEmotionStrength, Strength);
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FOnInworldCharacterEmotionalBehaviorChanged OnEmotionalBehaviorChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInworldCharacterPlayerInteractionStateChanged, bool, bInteracting); 
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FInworldCharacterPlayerInteractionStateChanged OnPlayerInteractionStateChanged;

    virtual void BeginPlay() override;
    virtual void EndPlay(EEndPlayReason::Type Reason);
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);
    
    UFUNCTION(BlueprintCallable, Category = "Inworld")
	virtual const FString& GetAgentId() const override { return AgentId; }
    virtual void SetAgentId(const FString& InAgentId) override { AgentId = InAgentId; }

    UFUNCTION(BlueprintCallable, Category = "Inworld")
    virtual const FString& GetGivenName() const override { return GivenName; }
    virtual void SetGivenName(const FString& InGivenName) override { GivenName = InGivenName; }

    virtual AActor* GetComponentOwner() const override { return GetOwner(); }

    UFUNCTION(BlueprintCallable, Category = "Inworld")
    const FString& GetUiName() const { return UiName; }
    UFUNCTION(BlueprintCallable, Category = "Inworld")
    void SetUiName(const FString& Name) { UiName = Name; }

	UFUNCTION(BlueprintCallable, Category = "Inworld", meta = (DeterminesOutputType = "Class"))
	UInworldCharacterPlayback* GetPlayback(TSubclassOf<UInworldCharacterPlayback> Class) const;

    virtual const FString& GetBrainName() const override { return BrainName; }

    virtual void HandlePacket(std::shared_ptr<Inworld::Packet> Packet) override;

	void HandlePlayerTalking(const Inworld::TextEvent& Event);
	void HandlePlayerInteraction(bool bInteracting);

	UFUNCTION(BlueprintCallable, Category = "Interactions")
	bool IsInteractingWithPlayer() const { return bInteractingWithPlayer; }

	UFUNCTION(BlueprintCallable, Category = "Emotions")
	EInworldCharacterEmotionalBehavior GetEmotionalBehavior() const { return EmotionalBehavior; }

	UFUNCTION(BlueprintPure, Category = "Emotions")
	EInworldCharacterEmotionStrength GetEmotionStrength() const { return EmotionStrength; }

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SendTextMessage(const FString& Text) const;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SendCustomEvent(const FString& Name) const;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SendAudioMessage(USoundWave* SoundWave) const;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void StartAudioSession() const;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void StopAudioSession() const;

	UFUNCTION(BlueprintCallable, Category = "Brain")
	void SetBrainName(const FString& Name) { BrainName = Name; }

    UFUNCTION(BlueprintCallable, Category = "Interaction")
	void CancelCurrentInteraction();

	UFUNCTION(BlueprintCallable, Category = "Events")
	bool Register();

	UFUNCTION(BlueprintCallable, Category = "Events")
	bool Unregister();

	const TSharedPtr<Inworld::FCharacterMessage> GetCurrentMessage() const 
	{ 
		return CurrentMessage; 
	}

	template<class T>
	T* GetPlaybackNative()
	{
		for (auto* Pb : Playbacks)
		{
			if (auto* Playback = Cast<T>(Pb))
			{
				return Playback;
			}
		}
		return nullptr;
	}

	UPROPERTY(EditAnywhere, Category = "Inworld")
	TArray<TSubclassOf<UInworldCharacterPlayback>> PlaybackTypes;

protected:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	FString UiName = "Character";

private:

	virtual void Visit(const Inworld::TextEvent& Event) override;
	virtual void Visit(const Inworld::AudioDataEvent& Event) override;
	virtual void Visit(const Inworld::SilenceEvent& Event) override;
	virtual void Visit(const Inworld::ControlEvent& Event) override;
	virtual void Visit(const Inworld::EmotionEvent& Event) override;
	virtual void Visit(const Inworld::CustomEvent& Event) override;

	template<class T>
	TSharedPtr<T> FindOrAddMessage(const std::string& InteractionId, const std::string& UtteranceId)
	{
		FString InteractionId_ = UTF8_TO_TCHAR(InteractionId.c_str());
		FString UtteranceId_ = UTF8_TO_TCHAR(UtteranceId.c_str());
		if (CancelledInteractions.Find(InteractionId_) != INDEX_NONE)
		{
			GetWorld()->GetSubsystem<UInworldApiSubsystem>()->CancelResponse(AgentId, InteractionId, { UtteranceId });
			return nullptr;
		}

		if (auto* Message = PendingMessages.FindByPredicate([&InteractionId_, &UtteranceId_](const auto& U) { return U->InteractionId == InteractionId_ && U->UtteranceId == UtteranceId_; }))
		{
			return StaticCastSharedPtr<T>(*Message);
		}

		TSharedPtr<T> Message = MakeShared<T>();
		Message->InteractionId = InteractionId_;
		Message->UtteranceId = UtteranceId_;
		PendingMessages.Add(Message);
		return Message;
	}

	bool IsCustomGesture(const FString& CustomEventName) const;

    UPROPERTY(EditAnywhere, Category = "Inworld")
	FString BrainName;

	UPROPERTY()
	TArray<UInworldCharacterPlayback*> Playbacks;

	TArray<TSharedPtr<Inworld::FCharacterMessage>> PendingMessages;
	TArray<FString> CancelledInteractions;

	TSharedPtr<Inworld::FCharacterMessage> CurrentMessage;

    EInworldCharacterEmotionalBehavior EmotionalBehavior = EInworldCharacterEmotionalBehavior::NEUTRAL;
    EInworldCharacterEmotionStrength EmotionStrength = EInworldCharacterEmotionStrength::UNSPECIFIED;

	FString AgentId;
	
	FString GivenName;

	bool bInteractingWithPlayer = false;
    bool bRegistered = false;
};
