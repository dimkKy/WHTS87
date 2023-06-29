// by Dmitry Kolontay


#include "WHTS87Utils.h"
#include "Camera/CameraComponent.h"

float WHTS87Utils::GetCameraNearPlane(const UCameraComponent& camera)
{
	float nearClipPlane{ camera.OrthoNearClipPlane };
	if (nearClipPlane <= 0.f || FMath::IsNearlyZero(nearClipPlane)) {
		if (IsValid(GEngine)) {
			nearClipPlane = GEngine->NearClipPlane;
		}
		else {
			nearClipPlane = defaultNearClipPlane;
		}	
	}
    return nearClipPlane;
}

FName WHTS87Utils::GetNumberedName(const FString& source, int32 number)
{
    return FName{ source + FString::FromInt(number) };
}
