// by Dmitry Kolontay


#include "CoreSystems/IntroCamera.h"
//#include "Camera/CameraComponent.h"
#include "Environment/Monitor.h"
#include "Kismet/KismetMathLibrary.h"
#include "WHTS87Utils.h"

AIntroCamera::AIntroCamera(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	//camera{CreateDefaultSubobject<UCameraComponent>("camera")},
	transitionDuration{ 3.f }
{
}

void AIntroCamera::BeginPlay()
{
	Super::BeginPlay();

	/*targetPos = targetMonitor->GetCloseViewingPos(camera->FieldOfView, 
		camera->AspectRatio, WHTS87Utils::GetCameraNearPlane(*camera));*/
	targetRot = -1.f * targetMonitor->GetActorForwardVector();
}

void AIntroCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UKismetMathLibrary::TInterpTo
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
