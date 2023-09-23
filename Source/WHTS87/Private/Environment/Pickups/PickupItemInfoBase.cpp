// by Dmitry Kolontay


#include "Environment/Pickups/PickupItemInfoBase.h"
#include "Environment/PickupItemContainer.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

UPickupItemInfoBase::UPickupItemInfoBase() :
	name{ "None" }, xSize{ 1 }, ySize{ 1 }, maxStackSize{ 1 },
	defaultSpawnStackSize{ 1 }, minUsableQuantity{ 1 }, 
	itemWeightInKG{ 1.f }, thumbnail{ nullptr }, bodyMesh{ nullptr }
{

}


bool UPickupItemInfoBase::ConstructContainer(APickupItemContainer& container) const
{
	if (!IsValidChecked(&container) || container.GetItemInfo() != nullptr) {
		return false;
	}
		
	UStaticMeshComponent* containerBody{ container.GetBody() };
	containerBody->SetStaticMesh(bodyMesh);
	containerBody->SetMassOverrideInKg(NAME_None, itemWeightInKG);
	containerBody->SetAngularDamping(itemWeightInKG * 0.5f);
	containerBody->SetLinearDamping(itemWeightInKG);
	//container.properties = MakeNonStaticProperties();
	return true;
}

FPrimaryAssetId UPickupItemInfoBase::GetPrimaryAssetId() const
{
	FPrimaryAssetId thisAssetId{ Super::GetPrimaryAssetId() };
	thisAssetId.PrimaryAssetName = name;
	return thisAssetId;
}

#if WITH_EDITOR
EDataValidationResult UPickupItemInfoBase::IsDataValid(FDataValidationContext& context) const
{
	Super::IsDataValid(context);

	if (name.IsNone())
		context.AddError(FText::FromString("Name was NAME_NONE"));
	if (useActionText.IsEmpty())
		context.AddError(FText::FromString("UseActionText was empty"));
	if (description.IsEmpty())
		context.AddError(FText::FromString("Description was empty"));
	if (displayName.IsEmpty())
		context.AddError(FText::FromString("DisplayName was empty"));
	if (xSize < 1 || ySize < 1)
		context.AddError(FText::FromString("Iventory sizes can not be less than 1"));
	if (maxStackSize < 1 || defaultSpawnStackSize < 1 || maxStackSize < defaultSpawnStackSize)
		context.AddError(FText::FromString("Invalid stack sizes: can not be less than 1, max size can not be less than default one"));
	if (itemWeightInKG < 0.1f)
		context.AddError(FText::FromString("Weight can not be less than 100g"));
	if (!thumbnail->IsValidLowLevel())
		context.AddError(FText::FromString("Thumbnail is not valid"));
	if (minUsableQuantity > maxStackSize)
		context.AddError(FText::FromString("Min usable quantity is greater than max stack size"));
	if (!bodyMesh->HasValidRenderData())
		context.AddError(FText::FromString("Body mesh is not valid"));

	if (context.GetNumErrors() > 0) {
		return EDataValidationResult::Invalid;
	}
	else {
		return EDataValidationResult::Valid;
	}
}
#endif

FIntPoint UPickupItemInfoBase::GetInventorySize() const
{
	return { xSize, ySize };
}

int32 UPickupItemInfoBase::GetXInventorySize() const
{
	return xSize;
}

int32 UPickupItemInfoBase::GetYInventorySize() const
{
	return ySize;
}

int32 UPickupItemInfoBase::GetDefaultStackSize() const
{
	return defaultSpawnStackSize;
}

int32 UPickupItemInfoBase::GetMaxStackSize() const
{
	return maxStackSize;
}
//delete?
int32 UPickupItemInfoBase::GetMinUsableQuantity() const
{
	return minUsableQuantity;
}

float UPickupItemInfoBase::GetItemWeight() const
{
	return itemWeightInKG;
}

UTexture2D* UPickupItemInfoBase::GetThumbnail() const
{
	return thumbnail;
}

const UStaticMesh* UPickupItemInfoBase::GetBodyMesh() const
{
	return bodyMesh;
}

FName UPickupItemInfoBase::GetName() const
{ 
	return name; 
}

FText UPickupItemInfoBase::GetUseActionText() const
{
	return useActionText;
}

FText UPickupItemInfoBase::GetDisplayName() const
{
	return displayName;
}

FText UPickupItemInfoBase::GetDescription() const
{
	return description;
}