// by Dmitry Kolontay


#include "UI/Gametime/InventoryUI/InventoryPanel.h"
#include "UI/Gametime/InventoryUI/InventorySlot.h"
#include "UI/Gametime/InventoryUI/InventoryMenu.h"
#include "UI/Gametime/InventoryUI/InventoryDragDropOperation.h"
#include "Components/InventoryComponent.h"
#include "Components/ScrollBox.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Environment/PickupItemContainer.h"
#include "Environment/Pickups/PickupItemInfoBase.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif


void UInventoryPanel::NativeOnInitialized()
{
    Super::NativeOnInitialized();
	scrollBox->SetConsumeMouseWheel(EConsumeMouseWheel::WhenScrollingPossible);
    scrollBox->SetOrientation(EOrientation::Orient_Vertical);
}

void UInventoryPanel::UpdateAllSlots()
{
	if (!ensureAlways(representedInventory.IsValid()))
		return;

	UInventoryMenu* parent{ CastChecked<UInventoryMenu>(Slot->Parent->WidgetGeneratedBy) };
	
	slotCanvas->ClearChildren();
	for (auto& slot : slotCanvas->GetAllChildren()) {
		slot->RemoveFromParent();
	}

	float tileSize{ parent->GetSlotTileSize() };
	for (auto& containerInfo : representedInventory->GetContainersInfo()) {
		UInventorySlot* slot{ CreateWidget<UInventorySlot>(this, parent->GetInventorySlotClass()) };
		slot->SetItemContainer(*containerInfo.Key, tileSize);

		UCanvasPanelSlot* canvasSlot{ slotCanvas->AddChildToCanvas(slot) };
		canvasSlot->SetPosition(containerInfo.Value * tileSize);
		canvasSlot->SetSize(containerInfo.Key->GetItemInfo()->GetInventorySize() * tileSize);
	}
	//do I need if
	/*if (Slot->Parent->WidgetGeneratedBy.IsValid() && representedInventory.IsValid()) {
		
	}*/
}

/*
UInventoryMenu* UInventoryPanel::GetParentMenu() const
{
	return CastChecked<UInventoryMenu>(Slot->Parent->WidgetGeneratedBy.Get());
}*/

void UInventoryPanel::SetNewInventory(UInventoryComponent* newInventory)
{
	representedInventory = TWeakObjectPtr<UInventoryComponent>{ newInventory };
}

#if WITH_EDITOR
EDataValidationResult UInventoryPanel::IsDataValid(FDataValidationContext& context) const
{
	Super::IsDataValid(context);

	if (slotCanvas->Slot->Parent != scrollBox) {
		context.AddError(FText::FromString("slotCanvas is supposed to be inside scrollBox"));
	}
		

	return context.GetNumErrors() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif

bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (!ensureAlways(representedInventory.IsValid()))
		return false;

	UInventoryDragDropOperation* operation{ CastChecked<UInventoryDragDropOperation>(InOperation) };

	if (UInventorySlot* draggedSlot{ Cast<UInventorySlot>(operation->Payload) }) {
		
		UInventoryMenu* parentMenu{ CastChecked<UInventoryMenu>(Slot->Parent->WidgetGeneratedBy.Get()) };
		float tileSize{ parentMenu->GetSlotTileSize() };

		FVector2D slotLocalMinPos{ InGeometry.AbsoluteToLocal(
			InDragDropEvent.GetScreenSpacePosition()) - operation->localPivotPos };

		FIntPoint newPos{ 
			FMath::Clamp(FGenericPlatformMath::RoundToInt32(
				slotLocalMinPos.X / tileSize), 0, representedInventory->GetXSize()),
			FMath::Clamp(FGenericPlatformMath::RoundToInt32(
				(slotLocalMinPos.Y + scrollBox->GetScrollOffset()) / tileSize), 0, representedInventory->GetYSize())
		};

		APickupItemContainer* containter{ draggedSlot->GetItemContainer() };
		if (representedInventory->TryAddContainer(*containter, newPos, true, false)) {
			UpdateAllSlots();
			if (containter->GetOwnerInventory() != representedInventory && containter->GetItemsCount() > 1) {
				if (UCanvasPanelSlot * canvasSlot{ Cast<UCanvasPanelSlot>(
					operation->initialPanel->AddChild(draggedSlot)) }) {
					canvasSlot->SetPosition(operation->initialPosition);
				}
			}
		}
		return true;
	}
	return false;
}
