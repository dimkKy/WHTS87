// by Dmitry Kolontay


#include "Environment/Pickups/FlashlightInfo.h"
//#include "Environment/Pickups/PickupItemInfoBase.h"
#include "Environment/PickupItemContainer.h"

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
EDataValidationResult UFlashlightInfo::IsDataValid(TArray<FText>& ValidationErrors)
{
	Super::IsDataValid(ValidationErrors);

	/*if (bodyMesh->FindSocket(spotLightSocket) == NULL) {
		ValidationErrors.Add(FText::FromString("Socket for light has bad name"));
	}*/
		
	if (ValidationErrors.Num() > 0) {
		return EDataValidationResult::Invalid;
	}
	else {
		return EDataValidationResult::Valid;
	}
}
#endif
