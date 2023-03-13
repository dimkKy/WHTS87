// by Dmitry Kolontay


#include "UI/Gametime/InventoryUI/InventoryItemInfoPanel.h"
#include "Environment/Pickups/PickupItemInfoBase.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"

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
EDataValidationResult UInventoryItemInfoPanel::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult superResult{ Super::IsDataValid(ValidationErrors) };
	/*if (superResult != EDataValidationResult::Invalid) {
		if (sizeBox is root?)
			ValidationErrors.Add(FText::FromString("sizeBox is supposed to be root"));

		if (ValidationErrors.Num() > 0) {
			superResult = EDataValidationResult::Invalid;
		}
	}*/
	return superResult;
}
#endif

void UInventoryItemInfoPanel::AssignItemInfo(UPickupItemInfoBase* newItemInfo)
{
	if (IsValid(newItemInfo)) {
		itemName->SetText(newItemInfo->GetDisplayName());
		itemDescription->SetText(newItemInfo->GetDescription());
	}
}
