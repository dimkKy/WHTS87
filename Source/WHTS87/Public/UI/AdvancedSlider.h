// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AdvancedSlider.generated.h"

class UButton;
class USlider;
class UTextBlock;
class USizeBox;
/**
 * TODO
 */
UCLASS(Abstract)
class WHTS87_API UAdvancedSlider : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		USlider* slider;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		USizeBox* sizeBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* increaseValueButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* decreaseValueButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* sliderDescriptionText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* sliderValueText;
	UFUNCTION()
		void OnIncreaseValueButtonPressed();
	UFUNCTION()
		void OnDecreaseValueButtonPressed();
	UFUNCTION()
		void OnSliderValueChanged(float newValue);
	static const FNumberFormattingOptions valueFormattingOptions;
};
