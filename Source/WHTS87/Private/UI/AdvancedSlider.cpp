// by Dmitry Kolontay


#include "UI/AdvancedSlider.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "Fonts/FontMeasure.h"

const FNumberFormattingOptions UAdvancedSlider::valueFormattingOptions{ FNumberFormattingOptions().SetMaximumFractionalDigits(0) };

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
	FVector2D sliderTextSize{ 
		FSlateApplication::Get().GetRenderer()->GetFontMeasureService().Get().Measure("999", sliderValueText->Font) };
	sliderValueText->SetMinDesiredWidth(sliderTextSize.X);
	sizeBox->SetMinDesiredWidth(sliderTextSize.Y * 10.f);
	
	OnSliderValueChanged(100.f);
	//ForceLayoutPrepass();
}
#if WITH_EDITOR
#include "Blueprint/WidgetTree.h"
EDataValidationResult UAdvancedSlider::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult superResult{ Super::IsDataValid(ValidationErrors) };
	if (superResult != EDataValidationResult::Invalid) {
		if (WidgetTree->RootWidget != sizeBox)
			ValidationErrors.Add(FText::FromString("sizeBox is supposed to be root"));

		if (ValidationErrors.Num() > 0) {
			superResult = EDataValidationResult::Invalid;
		}
	}
	return superResult;
}
#endif

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
