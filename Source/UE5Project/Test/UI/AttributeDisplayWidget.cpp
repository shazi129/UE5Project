#include "AttributeDisplayWidget.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

#define LLOCTEXT_NAMESPACE "AttributeDisplay"

void UAttributeDisplayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (Character)
	{
		ASC = Cast<UExAbilitySystemComponent>(Character->GetComponentByClass(UExAbilitySystemComponent::StaticClass()));
		if (ASC)
		{
			float CurrentHealth = ASC->GetAttributeData(HealthAttribute).GetBaseValue();
			Text_Health->SetText(FText::AsNumber(FMath::RoundToInt(CurrentHealth)));
			ASC->GetAttribuiteChangedDelegate(HealthAttribute)->Delegate.AddDynamic(this, &UAttributeDisplayWidget::OnHealthChange);

			float CurrentStamina = ASC->GetAttributeData(StaminaAttribute).GetBaseValue();
			Text_Stamina->SetText(FText::AsNumber(FMath::RoundToInt(CurrentStamina)));
			ASC->GetAttribuiteChangedDelegate(StaminaAttribute)->Delegate.AddDynamic(this, &UAttributeDisplayWidget::OnStaminaChange);

			float CurrentMana = ASC->GetAttributeData(ManaAttribute).GetBaseValue();
			Text_Mana->SetText(FText::AsNumber(FMath::RoundToInt(CurrentMana)));
			ASC->GetAttribuiteChangedDelegate(ManaAttribute)->Delegate.AddDynamic(this, &UAttributeDisplayWidget::OnManaChange);
		}
	}
}

void UAttributeDisplayWidget::NativeDestruct()
{
	Super::NativeDestruct();

	ASC->GetAttribuiteChangedDelegate(HealthAttribute)->Delegate.RemoveDynamic(this, &UAttributeDisplayWidget::OnHealthChange);
	ASC->GetAttribuiteChangedDelegate(StaminaAttribute)->Delegate.RemoveDynamic(this, &UAttributeDisplayWidget::OnStaminaChange);
	ASC->GetAttribuiteChangedDelegate(ManaAttribute)->Delegate.RemoveDynamic(this, &UAttributeDisplayWidget::OnManaChange);
}

void UAttributeDisplayWidget::OnHealthChange(const FExOnAttributeChangeData& ChangeData)
{
	Text_Health->SetText(FText::AsNumber(FMath::RoundToInt(ChangeData.NewValue)));
}

void UAttributeDisplayWidget::OnStaminaChange(const FExOnAttributeChangeData& ChangeData)
{
	Text_Stamina->SetText(FText::AsNumber(FMath::RoundToInt(ChangeData.NewValue)));
}

void UAttributeDisplayWidget::OnManaChange(const FExOnAttributeChangeData& ChangeData)
{
	Text_Mana->SetText(FText::AsNumber(FMath::RoundToInt(ChangeData.NewValue)));
}

#undef LLOCL_NAMESPACE
