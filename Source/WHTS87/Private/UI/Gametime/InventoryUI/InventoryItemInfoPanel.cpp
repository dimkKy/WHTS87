// by Dmitry Kolontay


#include "UI/Gametime/InventoryUI/InventoryItemInfoPanel.h"
#include "Environment/Pickups/PickupItemInfoBase.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

void UInventoryItemInfoPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	itemDescription->SetAutoWrapText(true);
	SetVisibility(ESlateVisibility::Collapsed);
}

void UInventoryItemInfoPanel::SetWidthOverride(float deziredWidth)
{
	sizeBox->SetWidthOverride(deziredWidth);
}

#if WITH_EDITOR
EDataValidationResult UInventoryItemInfoPanel::IsDataValid(FDataValidationContext& context) const
{
	Super::IsDataValid(context);
	/*if (superResult != EDataValidationResult::Invalid) {
		if (sizeBox is root?)
			context.AddError(FText::FromString("sizeBox is supposed to be root"));

		if (context.GetNumErrors() > 0) {
			superResult = EDataValidationResult::Invalid;
		}
	}*/
	return context.GetNumErrors() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif

void UInventoryItemInfoPanel::AssignItemInfo(UPickupItemInfoBase* newItemInfo)
{
	if (IsValid(newItemInfo)) {
		itemName->SetText(newItemInfo->GetDisplayName());
		itemDescription->SetText(newItemInfo->GetDescription());
	}
}
