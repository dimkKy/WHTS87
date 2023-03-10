// by Dmitry Kolontay


#include "Environment/Pickups/PickupItemInfoBase.h"
#include "Environment/PickupItemContainer.h"

UPickupItemInfoBase::UPickupItemInfoBase() :
	name{ "None" }, XInventorySize{ 1 }, YInventorySize{ 1 }, maxStackSize{ 1 }, defaultSpawnStackSize{ 1 },
	minUsableQuantity{ 1 }, itemWeightInKG{ 1.f }, thumbnail{ nullptr }, bodyMesh{ nullptr }
{
}

bool UPickupItemInfoBase::ConstructContainerMesh(APickupItemContainer* container)
{
	if (IsValid(container) && container->GetItemInfo() == this) {
		UStaticMeshComponent* containerBody{ container->GetBody() };
		containerBody->SetStaticMesh(bodyMesh);
		containerBody->SetMassOverrideInKg(NAME_None, itemWeightInKG);
		containerBody->SetAngularDamping(itemWeightInKG * 0.5f);
		containerBody->SetLinearDamping(itemWeightInKG);
		return true;
	}
	else {
		return false;
	}
}

FPrimaryAssetId UPickupItemInfoBase::GetPrimaryAssetId() const
{
	FPrimaryAssetId thisAssetId{ Super::GetPrimaryAssetId() };
	thisAssetId.PrimaryAssetName = name;
	return thisAssetId;
}

#if WITH_EDITOR
EDataValidationResult UPickupItemInfoBase::IsDataValid(TArray<FText>& ValidationErrors)
{
	if (name.IsNone())
		ValidationErrors.Add(FText::FromString("Name was NAME_NONE"));
	if (useActionText.IsEmpty())
		ValidationErrors.Add(FText::FromString("UseActionText was empty"));
	if (description.IsEmpty())
		ValidationErrors.Add(FText::FromString("Description was empty"));
	if (displayName.IsEmpty())
		ValidationErrors.Add(FText::FromString("DisplayName was empty"));
	if (XInventorySize < 1 || YInventorySize < 1)
		ValidationErrors.Add(FText::FromString("Iventory sizes can not be less than 1"));
	if (maxStackSize < 1 || defaultSpawnStackSize < 1 || maxStackSize < defaultSpawnStackSize)
		ValidationErrors.Add(FText::FromString("Invalid stack sizes: can not be less than 1, max size can not be less than default one"));
	if (itemWeightInKG < 0.1f)
		ValidationErrors.Add(FText::FromString("Weight can not be less than 100g"));
	if (!thumbnail->IsValidLowLevel())
		ValidationErrors.Add(FText::FromString("Thumbnail is not valid"));
	if (minUsableQuantity > maxStackSize)
		ValidationErrors.Add(FText::FromString("Min usable quantity is greater than max stack size"));
	if (!bodyMesh->HasValidRenderData())
		ValidationErrors.Add(FText::FromString("Body mesh is not valid"));
	if (ValidationErrors.Num() > 0) {
		return EDataValidationResult::Invalid;
	}
	else {
		return EDataValidationResult::Valid;
	}
}
#endif
