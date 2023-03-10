﻿// by Dmitry Kolontay


#include "UI/Gametime/InventoryPanel.h"
#include "UI/Gametime/InventorySlot.h"
#include "UI/Gametime/InventoryMenu.h"
#include "Components/InventoryComponent.h"
#include "Components/ScrollBox.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Environment/PickupItemContainer.h"
#include "Environment/Pickups/PickupItemInfoBase.h"



void UInventoryPanel::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    panelScrollBox->ConsumeMouseWheel = EConsumeMouseWheel::WhenScrollingPossible;
    panelScrollBox->SetOrientation(EOrientation::Orient_Vertical);
    auto test = Slot;
    //what is Slot?
}

void UInventoryPanel::UpdateAllSlots()
{
	check(IsValid(slotClass));
	UInventoryMenu* parentMenu{ GetParentMenu() };
	if (parentMenu != nullptr && representedInventory.IsValid()) {
		float tileSize{ parentMenu->GetSlotTileSize()};
		TArray<UWidget*> currentSlots{ slotCanvas->GetAllChildren() };
		slotCanvas->ClearChildren();
		for (const auto& slot : currentSlots) {
			slot->RemoveFromParent();
		}

		for (auto const& itemContainerTuple : representedInventory.Get()->containerMap) {

			UInventorySlot* inventorySlot{ CreateWidget<UInventorySlot>(this, slotClass) };
			inventorySlot->SetItemContainer(itemContainerTuple.Key, tileSize);
			UCanvasPanelSlot* canvasSlot{ slotCanvas->AddChildToCanvas(inventorySlot) };
			canvasSlot->SetPosition(FVector2D(tileSize * itemContainerTuple.Value.Key, tileSize * itemContainerTuple.Value.Value));
			canvasSlot->SetSize(FVector2D(tileSize * itemContainerTuple.Key->GetItemInfo()->GetXInventorySize(), tileSize * itemContainerTuple.Key->GetItemInfo()->GetYInventorySize()));
		}
	}
}

UInventoryMenu* UInventoryPanel::GetParentMenu() const
{
    if (UInventoryMenu* parentMenu{ Cast<UInventoryMenu>(Slot->Parent) })
        return parentMenu;
    return nullptr;
}

void UInventoryPanel::SetNewInventory(UInventoryComponent* newInventory)
{
	representedInventory = TWeakObjectPtr<UInventoryComponent>{ newInventory };
}

bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    return false;
}
