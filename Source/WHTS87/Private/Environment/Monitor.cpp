// by Dmitry Kolontay


#include "Environment/Monitor.h"
#include "Components/WidgetComponent.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

AMonitor::AMonitor() :
	display{ CreateDefaultSubobject<UWidgetComponent>("display") }
{
	SetRootComponent(display);
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
EDataValidationResult AMonitor::IsDataValid(FDataValidationContext& context) const
{
	Super::IsDataValid(context);

	if (!widgetClass) {
		context.AddError(FText::FromString("Invalid widgetClass"));
	}

	return context.GetNumErrors() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif // WITH_EDITOR