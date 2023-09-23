// by Dmitry Kolontay


#include "Environment/Pickups/FlashlightInfo.h"
//#include "Environment/Pickups/PickupItemInfoBase.h"
#include "Environment/PickupItemContainer.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

bool UFlashlightInfo::ConstructContainer(APickupItemContainer& container) const
{
	if (Super::ConstructContainer(container)) {
		container.properties = FItemPropertiesBase::MakeUniquePtr<FFlashlightProperties>();
		//light source?
		return true;
	}
	return false;
}

TUniquePtr<FItemPropertiesBase> UFlashlightInfo::MakeNonStaticProperties() const
{
	return FItemPropertiesBase::MakeUniquePtr<FFlashlightProperties>();
}

int32 UFlashlightInfo::OnUse(AActor& caller, AActor* target, int32 timesToUse) const
{
	/*if (APlayerCharacter* actorToEquipOn = Cast<APlayerCharacter>(target)) {
		if (caller == target) {
			actorToEquipOn->EquipItemContainer(container);
		}
	}*/
	return 0;
}

#if WITH_EDITOR
EDataValidationResult UFlashlightInfo::IsDataValid(FDataValidationContext& context) const
{
	Super::IsDataValid(context);

	/*if (bodyMesh->FindSocket(spotLightSocket) == NULL) {
		context.AddError(FText::FromString("Socket for light has bad name"));
	}*/
		
	if (context.GetNumErrors() > 0) {
		return EDataValidationResult::Invalid;
	}
	else {
		return EDataValidationResult::Valid;
	}
}
#endif
