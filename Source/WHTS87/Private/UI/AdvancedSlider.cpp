// by Dmitry Kolontay


#include "UI/AdvancedSlider.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "Fonts/FontMeasure.h"

const FNumberFormattingOptions UAdvancedSlider::valueFormattingOptions{ FNumberFormattingOptions().SetMaximumFractionalDigits(0) };

UAdvancedSlider::UAdvancedSlider(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UAdvancedSlider::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	slider->OnValueChanged.AddDynamic(this, &UAdvancedSlider::OnSliderValueChanged);
	slider->SetMinValue(0.f);
	slider->SetStepSize(1.f);
	slider->SetMaxValue(100.f);
	slider->SetValue(slider->MaxValue);
	
	increaseValueButton->SetClickMethod(EButtonClickMethod::PreciseClick);
	increaseValueButton->OnClicked.AddDynamic(this, &UAdvancedSlider::OnIncreaseValueButtonPressed);
	decreaseValueButton->SetClickMethod(EButtonClickMethod::PreciseClick);
	decreaseValueButton->OnClicked.AddDynamic(this, &UAdvancedSlider::OnDecreaseValueButtonPressed);
	//auto scale = UWidgetLayoutLibrary::GetViewportScale(GetWorld()->GetGameViewport());
	FVector2D measuredSliderValueTextSize{ FSlateApplication::Get().GetRenderer()->GetFontMeasureService().Get().Measure("999", sliderValueText->Font) };
	sliderValueText->SetMinDesiredWidth(measuredSliderValueTextSize.X);
	sizeBox->SetMinDesiredWidth(measuredSliderValueTextSize.Y * 10.f);
	
	OnSliderValueChanged(100.f);
	//ForceLayoutPrepass();
}

EDataValidationResult UAdvancedSlider::IsDataValid(TArray<FText>& ValidationErrors)
{
	return EDataValidationResult();
}

void UAdvancedSlider::OnIncreaseValueButtonPressed()
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

void UAdvancedSlider::OnDecreaseValueButtonPressed()
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
	sliderValueText->SetText(FText::AsNumber(newValue, &UAdvancedSlider::valueFormattingOptions));
}
