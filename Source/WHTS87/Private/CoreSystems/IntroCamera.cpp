// by Dmitry Kolontay


#include "CoreSystems/IntroCamera.h"
//#include "Camera/CameraComponent.h"
#include "Environment/Monitor.h"
#include "Kismet/KismetMathLibrary.h"
#include "WHTS87Utils.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

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
EDataValidationResult AIntroCamera::IsDataValid(FDataValidationContext& context) const
{
	Super::IsDataValid(context);

	if (!targetMonitor) {
		context.AddError(FText::FromString("Invalid targetMonitor"));
	}

	return context.GetNumErrors() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif // WITH_EDITOR
