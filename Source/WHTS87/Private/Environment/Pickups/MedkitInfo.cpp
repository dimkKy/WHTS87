// by Dmitry Kolontay


#include "Environment/Pickups/MedkitInfo.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

EDataValidationResult UMedkitInfo::IsDataValid(FDataValidationContext& context) const
{
	Super::IsDataValid(context);

	if (healAmount < 1.f)
		context.AddError(FText::FromString("healAmount can not be less than 1%"));
	if (healDuration < 0.f)
		context.AddError(FText::FromString("healDuration can not be less than 0"));

	if (context.GetNumErrors() > 0) {
		return EDataValidationResult::Invalid;
	}
	else {
		return EDataValidationResult::Valid;
	}
}
