// by Dmitry Kolontay


#include "Environment/Pickups/MedkitInfo.h"

EDataValidationResult UMedkitInfo::IsDataValid(TArray<FText>& ValidationErrors)
{
	Super::IsDataValid(ValidationErrors);

	if (healAmount < 1.f)
		ValidationErrors.Add(FText::FromString("healAmount can not be less than 1%"));
	if (healDuration < 0.f)
		ValidationErrors.Add(FText::FromString("healDuration can not be less than 0"));

	if (ValidationErrors.Num() > 0) {
		return EDataValidationResult::Invalid;
	}
	else {
		return EDataValidationResult::Valid;
	}
}
