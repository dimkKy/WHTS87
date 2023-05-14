// by Dmitry Kolontay


#include "CoreSystems/IntroCamera.h"
#include "Camera/CameraComponent.h"
#include "Environment/Monitor.h"
#include "WHTS87Utils.h"

AIntroCamera::AIntroCamera() :
	camera{CreateDefaultSubobject<UCameraComponent>("camera")},
	transitionDuration{ 3.f }
{
	PrimaryActorTick.bCanEverTick = true;
}

void AIntroCamera::BeginPlay()
{
	Super::BeginPlay();

	targetPos = targetMonitor->GetCloseViewingPos(camera->FieldOfView, 
		camera->AspectRatio, WHTS87Utils::GetCameraNearPlane(*camera));
}

void AIntroCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#if WITH_EDITOR
EDataValidationResult AIntroCamera::IsDataValid(TArray<FText>& ValidationErrors)
{
	Super::IsDataValid(ValidationErrors);

	if (!targetMonitor) {
		ValidationErrors.Add(FText::FromString("Invalid targetMonitor"));
	}

	return ValidationErrors.Num() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif // WITH_EDITOR
