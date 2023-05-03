// by Dmitry Kolontay


#include "UI/Gametime/InventoryUI/InventoryDiscardPanel.h"
#include "UI/Gametime/InventoryUI/InventoryDragDropOperation.h"
#include "UI/Gametime/InventoryUI/InventorySlot.h"
#include "UI/Gametime/InventoryUI/InventoryMenu.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"

void UInventoryDiscardPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//redo
	discardingItemThubnail->SetOpacity(0.f);
	//discardingItemThubnail->Slot <- cast to set alignment
}

bool UInventoryDiscardPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	//UInventoryDragDropOperation* operation{ CastChecked<UInventoryDragDropOperation>(InOperation) };
	if (UInventorySlot * draggedInventorySlot{ Cast<UInventorySlot>(CastChecked<UInventoryDragDropOperation>(InOperation)->Payload) }) {
		draggedInventorySlot->RemoveContainerFromInventory(true, false);
		APickupItemContainer* itemContainerToRemove{ draggedInventorySlot->GetItemContainer() };
		discardingItemThubnail->SetOpacity(0.f);
		return true;
	}
	else {
		//?
		FSlateApplication::Get().CancelDragDrop();
	}
	discardingItemThubnail->SetOpacity(0.f);
	return false;
}

void UInventoryDiscardPanel::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (UInventorySlot * draggedInventorySlot{ Cast<UInventorySlot>(CastChecked<UInventoryDragDropOperation>(InOperation)->Payload) }) {
		UTexture2D* possibleItemThumbnail{ draggedInventorySlot->GetThumbnail() };
		if (IsValid(possibleItemThumbnail)) {
			discardingItemThubnail->SetBrushFromTexture(possibleItemThumbnail);
			FVector2D availableSize{ GetCachedGeometry().GetLocalSize() };
			FVector2D desiredBrushSize{ draggedInventorySlot->GetCachedGeometry().GetLocalSize() };
			float desiredRatio{ desiredBrushSize.X / desiredBrushSize.Y };
			if (desiredRatio > availableSize.X / availableSize.Y) {
				//other widget is more stretched than this
				discardingItemThubnail->SetBrushSize({ availableSize.X, availableSize.X / desiredRatio });
			}
			else {
				//other widget is less stretched than this
				discardingItemThubnail->SetBrushSize({ availableSize.Y * desiredRatio, availableSize.Y });
			}
			discardingItemThubnail->SetOpacity(1.f);
		}
	}
}

void UInventoryDiscardPanel::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	discardingItemThubnail->SetOpacity(0.f);
}
