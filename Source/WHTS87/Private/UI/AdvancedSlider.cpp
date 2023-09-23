// by Dmitry Kolontay


#include "UI/AdvancedSlider.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "Fonts/FontMeasure.h"
#include "WHTS87Utils.h"
#if WITH_EDITOR
#include "Blueprint/WidgetTree.h"
#include "Misc/DataValidation.h"
#endif

const FNumberFormattingOptions UAdvancedSlider::formattingOptions{ 
	FNumberFormattingOptions().SetMaximumFractionalDigits(0) };

void UAdvancedSlider::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	slider->OnValueChanged.AddDynamic(this, &UAdvancedSlider::OnSliderValueChanged);
	slider->SetMinValue(minValue);
	slider->SetStepSize(stepSize);
	slider->SetMaxValue(maxValue);
	
	//WHTS87Utils::SetupButtons(this, increaseButton, &UAdvancedSlider::OnIncreaseButtonPressed);

	increaseButton->SetClickMethod(EButtonClickMethod::PreciseClick);
	increaseButton->OnClicked.AddDynamic(this, &UAdvancedSlider::OnIncreaseButtonPressed);

	decreaseButton->SetClickMethod(EButtonClickMethod::PreciseClick);
	decreaseButton->OnClicked.AddDynamic(this, &UAdvancedSlider::OnDecreaseButtonPressed);

	FVector2D sliderTextSize{ FSlateApplication::Get().GetRenderer()->
		GetFontMeasureService().Get().Measure(FString::FromInt(FMath::RoundToInt32(maxValue)), valueText->GetFont()) };

	valueText->SetMinDesiredWidth(sliderTextSize.X);
	sizeBox->SetMinDesiredWidth(sliderTextSize.Y * minXYratio);
	
	SetSliderValue(slider->GetMaxValue());
	//ForceLayoutPrepass();
}

void UAdvancedSlider::SetSliderValue(float inValue)
{
	slider->SetValue(inValue);
	OnSliderValueChanged(inValue);
}

#if WITH_EDITOR
EDataValidationResult UAdvancedSlider::IsDataValid(FDataValidationContext& context) const
{
	Super::IsDataValid(context);

	/*if (WidgetTree) {
		if (WidgetTree->RootWidget != sizeBox) {
			context.AddError(FText::FromString("sizeBox is supposed to be root"));
		}
	}*/

	return context.GetNumErrors() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif

void UAdvancedSlider::OnIncreaseButtonPressed()
{
	float newValue{ slider->GetValue() + slider->GetStepSize() };
	if (newValue > slider->GetMaxValue()) {
		SetSliderValue(slider->GetMaxValue());
	}
	else {
		SetSliderValue(newValue);
	}
}

void UAdvancedSlider::OnDecreaseButtonPressed()
{
	float newValue{ slider->GetValue() - slider->GetStepSize() };
	if (newValue < slider->GetMinValue()) {
		SetSliderValue(slider->GetMinValue());
	}
	else {
		SetSliderValue(newValue);
	}
}

void UAdvancedSlider::OnSliderValueChanged(float newValue)
{
	valueText->SetText(FText::AsNumber(newValue, &UAdvancedSlider::formattingOptions));
}
