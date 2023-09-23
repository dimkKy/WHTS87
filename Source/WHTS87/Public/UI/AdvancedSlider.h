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

	void SetSliderValue(float inValue);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		USlider* slider;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& context) const override;
#endif

	constexpr static float minValue = 0.f;
	constexpr static float stepSize = 1.f;
	constexpr static float maxValue = 100.f;
	constexpr static float minXYratio = 10.f;
protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		USizeBox* sizeBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* increaseButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* decreaseButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* description;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* valueText;
	UFUNCTION()
		void OnIncreaseButtonPressed();
	UFUNCTION()
		void OnDecreaseButtonPressed();
	UFUNCTION()
		void OnSliderValueChanged(float newValue);

	static const FNumberFormattingOptions formattingOptions;
};
