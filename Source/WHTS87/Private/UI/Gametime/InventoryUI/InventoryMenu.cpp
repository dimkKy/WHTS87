// by Dmitry Kolontay

#include "UI/Gametime/InventoryUI/InventoryMenu.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/Gametime/InventoryUI/InventorySlot.h"
#include "UI/Gametime/InventoryUI/InventoryItemInfoPanel.h"
#include "UI/Gametime/InventoryUI/InventoryItemInfoDraggablePanel.h"
#include "Environment/Pickups/PickupItemInfoBase.h"
//#include "Blueprint/UserWidget.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

void UInventoryMenu::AddItemInfoPanel(UPickupItemInfoBase* itemInfo, FVector2D desiredAbsPos)
{
	if (IsValid(itemInfo)) {
		for (auto& infoPanelSlot : draggableInfoPanelSlots) {
			UInventoryItemInfoPanel* infopanel{ CastChecked<UInventoryItemInfoPanel>(infoPanelSlot->Content) };
			if (infopanel->GetVisibility() == ESlateVisibility::Collapsed) {
				infopanel->AssignItemInfo(itemInfo);
				hoveredInfoPanelSlot->SetPosition(infoPanelsCanvas->GetCachedGeometry().AbsoluteToLocal(desiredAbsPos));
				return;
			}
			/*if (infoPanelSlot->Content->GetItemInfo() == itemInfo) {
				if (infoPanelSlot->Content->GetVisibility() != ESlateVisibility::Visible)
					infoPanelSlot->Content->SetVisibility(ESlateVisibility::Visible);

				//higlight and move
				return;
			}*/
		}
		UInventoryItemInfoPanel* newInfoPanel{ CreateWidget<UInventoryItemInfoPanel>(this, hoveredItemInfoPanelClass) };
		//newInfoPanel->SetItemInfo(itemInfo, false, GetCachedGeometry().GetLocalSize().X);
		newInfoPanel->SetVisibility(ESlateVisibility::Visible);

		UCanvasPanelSlot* newSlot{ infoPanelsCanvas->AddChildToCanvas(newInfoPanel) };
		newSlot->SetAutoSize(true);
		draggableInfoPanelSlots.Add(newSlot);
	}
}

void UInventoryMenu::ToggleHoverItemInfopanel(UPickupItemInfoBase* itemInfo, bool bEnable, FVector2D desiredAbsolutePosition)
{
	check(IsValid(itemInfo));
	//UPickupItemInfoBase* currentItemInfo{ CastChecked<UInventoryItemInfoPanel>(hoveredInfoPanelSlot->Content)->GetItemInfo()};
	UInventoryItemInfoPanel* hoveredInfopanel{ CastChecked<UInventoryItemInfoPanel>(hoveredInfoPanelSlot->Content) };
	//perepisat govno
	if (bEnable) {
		if (itemInfo) {
			hoveredInfopanel->SetVisibility(ESlateVisibility::Visible);
			hoveredInfopanel->AssignItemInfo(itemInfo);
			
			hoveredInfoPanelSlot->SetPosition(infoPanelsCanvas->GetCachedGeometry().AbsoluteToLocal(desiredAbsolutePosition));
			//hoveredInfoPanelSlot->SetPosition(hoveredInfoPanelSlot->Parent->GetCachedGeometry().AbsoluteToLocal(desiredAbsPos));
			/*FVector2D thisPositionLU{ GetCachedGeometry().GetLocalPositionAtCoordinates({0.f, 0.f}) };
			float spawnPositionY{ desiredPositionY + thisPositionLU.Y - MainScrollBox->GetScrollOffset() };

			hoveredInfoPanelSlot->SetPosition({ thisPositionLU.X,
				spawnPositionY + hoveredInfoPanelSlot->Content->GetDesiredSize().Y > hoveredInfoPanelSlot->Parent->GetCachedGeometry().GetLocalSize().Y ?
				thisPositionLU.Y : spawnPositionY });*/

		}

	}
	else {
		hoveredInfopanel->SetVisibility(ESlateVisibility::Collapsed);
	}
	return;
}

float UInventoryMenu::GetSlotTileSize()
{
	check(false);
	return 0.0f;
}

void UInventoryMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//SetPositionInViewport
	//check(IsValid(hoveredItemInfoPanelClass));
	//hoveredInfoPanelSlot = infoPanelsCanvas->AddChildToCanvas(CreateWidget<UItemInfoPanel>(this, hoveredItemInfoPanelClass));
}

bool UInventoryMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}

#if WITH_EDITOR
EDataValidationResult UInventoryMenu::IsDataValid(FDataValidationContext& context) const
{
	//?
	Super::IsDataValid(context);
	if (IsValid(hoveredItemInfoPanelClass)) {
		if (hoveredItemInfoPanelClass.Get()->IsChildOf(UInventoryItemInfoDraggablePanel::StaticClass())) {
			context.AddError(FText::FromString(
				"Invalid hoveredItemInfoPanelClass: should not be derived from UInventoryItemInfoDraggablePanel"));
		}
	}
	else {
		context.AddError(FText::FromString("Invalid hoveredItemInfoPanelClass"));
	}
	if (!IsValid(draggableItemInfoPanelClass))
		context.AddError(FText::FromString("Invalid draggableItemInfoPanelClass"));
	if (!IsValid(itemInfoSlotClass))
		context.AddError(FText::FromString("Invalid itemInfoSlotClass"));

	return context.GetNumErrors() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif
