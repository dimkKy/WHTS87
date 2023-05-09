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
	itemThumbnail->SetOpacity(0.f);
	//itemThumbnail->Slot <- cast to set alignment
}

bool UInventoryDiscardPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (UInventorySlot * slot{ Cast<UInventorySlot>(CastChecked<UInventoryDragDropOperation>(InOperation)->Payload) }) {
		slot->RemoveContainerFromInventory(true, false);
		itemThumbnail->SetOpacity(0.f);
		return true;
	}
	else {
		//?
		FSlateApplication::Get().CancelDragDrop();
	}
	itemThumbnail->SetOpacity(0.f);
	return false;
}

void UInventoryDiscardPanel::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (UInventorySlot* slot{ Cast<UInventorySlot>(
		CastChecked<UInventoryDragDropOperation>(InOperation)->Payload) }) {
		UTexture2D* thumbnail{ slot->GetThumbnail() };
		if (!IsValid(thumbnail)) {
			return;
		}
		itemThumbnail->SetBrushFromTexture(thumbnail);
		FVector2D availableSize{ GetCachedGeometry().GetLocalSize() };
		FVector2D desiredBrushSize{ slot->GetCachedGeometry().GetLocalSize() };
		float desiredRatio{ desiredBrushSize.X / desiredBrushSize.Y };
		if (desiredRatio > availableSize.X / availableSize.Y) {
			//other widget is more stretched than this
			itemThumbnail->SetBrushSize({ availableSize.X, availableSize.X / desiredRatio });
		}
		else {
			//other widget is less stretched than this
			itemThumbnail->SetBrushSize({ availableSize.Y * desiredRatio, availableSize.Y });
		}
		itemThumbnail->SetOpacity(1.f);
	}
}

void UInventoryDiscardPanel::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	itemThumbnail->SetOpacity(0.f);
}
