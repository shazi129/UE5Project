#pragma

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ExAbilitySystemComponent.h"
#include "AttributeDisplayWidget.generated.h"

UCLASS()
class UAttributeDisplayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category=Attribute)
		FGameplayAttribute HealthAttribute;

	UPROPERTY(EditAnywhere, Category = Attribute)
		FGameplayAttribute StaminaAttribute;

	UPROPERTY(EditAnywhere, Category = Attribute)
		FGameplayAttribute ManaAttribute;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* Text_Health;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* Text_Stamina;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* Text_Mana;

protected:
	virtual void NativeConstruct();
	virtual void NativeDestruct();

	UFUNCTION()
	void OnHealthChange(const FExOnAttributeChangeData& ChangeData);

	UFUNCTION()
	void OnStaminaChange(const FExOnAttributeChangeData& ChangeData);

	UFUNCTION()
	void OnManaChange(const FExOnAttributeChangeData& ChangeData);
private:
	UExAbilitySystemComponent* ASC;
};