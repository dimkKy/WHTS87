// by Dmitry Kolontay


#include "Environment/Monitor.h"
#include "Components/WidgetComponent.h"

AMonitor::AMonitor() :
	display{ CreateDefaultSubobject<UWidgetComponent>("display") }
{
	//display->SetDrawAtDesiredSize(true);?
}

void AMonitor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AMonitor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMonitor::BeginPlay()
{
}

FVector AMonitor::GetCloseViewingPos(float cameraFOV, float aspectRatio, float nearClipPlane)
{
	return FVector();
}

#if WITH_EDITOR
EDataValidationResult AMonitor::IsDataValid(TArray<FText>& ValidationErrors)
{
	Super::IsDataValid(ValidationErrors);

	if (!widgetClass) {
		ValidationErrors.Add(FText::FromString("Invalid widgetClass"));
	}

	return ValidationErrors.Num() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif // WITH_EDITOR