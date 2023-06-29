// by Dmitry Kolontay


#include "UI/AdvancedSlider.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "Fonts/FontMeasure.h"
#if WITH_EDITOR
#include "Blueprint/WidgetTree.h"
#endif
#include "WHTS87Utils.h"

const FNumberFormattingOptions UAdvancedSlider::formattingOptions{ 
	FNumberFormattingOptions().SetMaximumFractionalDigits(0) };

void UAdvancedSlider::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	slider->OnValueChanged.AddDynamic(this, &UAdvancedSlider::OnSliderValueChanged);
	slider->SetMinValue(minValue);
	slider->SetStepSize(stepSize);
	slider->SetMaxValue(maxValue);
	slider->SetValue(slider->MaxValue);
	
	//WHTS87Utils::SetupButtons(this, increaseButton, &UAdvancedSlider::OnIncreaseButtonPressed);

	increaseButton->SetClickMethod(EButtonClickMethod::PreciseClick);
	increaseButton->OnClicked.AddDynamic(this, &UAdvancedSlider::OnIncreaseButtonPressed);

	decreaseButton->SetClickMethod(EButtonClickMethod::PreciseClick);
	decreaseButton->OnClicked.AddDynamic(this, &UAdvancedSlider::OnDecreaseButtonPressed);

	FVector2D sliderTextSize{ FSlateApplication::Get().GetRenderer()->
		GetFontMeasureService().Get().Measure("999", valueText->Font) };

	valueText->SetMinDesiredWidth(sliderTextSize.X);
	sizeBox->SetMinDesiredWidth(sliderTextSize.Y * minXYratio);
	
	OnSliderValueChanged(100.f);
	//ForceLayoutPrepass();
}

#if WITH_EDITOR
EDataValidationResult UAdvancedSlider::IsDataValid(TArray<FText>& ValidationErrors)
{
	Super::IsDataValid(ValidationErrors);

	/*if (WidgetTree) {
		if (WidgetTree->RootWidget != sizeBox) {
			ValidationErrors.Add(FText::FromString("sizeBox is supposed to be root"));
		}
	}*/

	return ValidationErrors.Num() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif

void UAdvancedSlider::OnIncreaseButtonPressed()
{
	float newValue{ slider->GetValue() + slider->StepSize };
	if (newValue > slider->MaxValue) {
		slider->SetValue(slider->MaxValue);
		OnSliderValueChanged(slider->MaxValue);
	}
	else {
		slider->SetValue(newValue);
		OnSliderValueChanged(newValue);
	}
}

void UAdvancedSlider::OnDecreaseButtonPressed()
{
	float newValue{ slider->GetValue() - slider->StepSize };
	if (newValue < slider->MinValue) {
		slider->SetValue(slider->MinValue);
		OnSliderValueChanged(slider->MinValue);
	}
	else {
		slider->SetValue(newValue);
		OnSliderValueChanged(newValue);
	}
}

void UAdvancedSlider::OnSliderValueChanged(float newValue)
{
	valueText->SetText(FText::AsNumber(newValue, &UAdvancedSlider::formattingOptions));
}
