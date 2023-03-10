// by Dmitry Kolontay


#include "UI/Gametime/InventoryDiscardPanel.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"

void UInventoryDiscardPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//переписать
	disvardingItemThubnail->SetOpacity(0.f);
}

bool UInventoryDiscardPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	/*
	UInventoryDragDropOperator* DragDropOperator = CastChecked<UInventoryDragDropOperator>(InOperation);
	if (DragDropOperator->Payload->GetClass()->IsChildOf(UInventorySlot::StaticClass())) {
		UInventorySlot* DraggedInventorySlot = CastChecked<UInventorySlot>(DragDropOperator->Payload);
		APickupItemContainer* itemContainerToRemove = DraggedInventorySlot->GetItemContainer();
		check(IsValid(itemContainerToRemove));
		MainImage->SetOpacity(0.f);
		itemContainerToRemove->GetOwnerInventory()->RemoveContainer(itemContainerToRemove, true, true);
		return true;
	}*/
	disvardingItemThubnail->SetOpacity(0.f);
	return false;
}

void UInventoryDiscardPanel::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	/*
	UInventoryDragDropOperator* DragDropOperator = CastChecked<UInventoryDragDropOperator>(InOperation);
	if (DragDropOperator->Payload->GetClass()->IsChildOf(UInventorySlot::StaticClass())) {
		UInventorySlot* DraggedInventorySlot = CastChecked<UInventorySlot>(DragDropOperator->Payload);
		UTexture2D* PossibleItemThumbnail = DraggedInventorySlot->GetItemContainer()->GetItemInfo()->GetThumbnail();
		if (IsValid(PossibleItemThumbnail)) {
			disvardingItemThubnail->SetBrushFromTexture(PossibleItemThumbnail);
			FVector2D imageSize = disvardingItemThubnail->Brush.GetImageSize();
			//
			//FVector2D desiredSize = mainSizeBox->GetDesiredSize();
			FVector2D localSize = disvardingItemThubnail->GetCachedGeometry().GetLocalSize();
			//
			if (localSize.X < localSize.Y) {
				//MainBorder->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
				//MainBorder->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
				imageSize.Y = localSize.X * imageSize.Y / imageSize.X;
				imageSize.X = localSize.X;

			}
			else {
				//MainBorder->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
				//MainBorder->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
				imageSize.X = localSize.Y * imageSize.X / imageSize.Y;
				imageSize.Y = localSize.Y;
			}
			disvardingItemThubnail->SetBrushSize(imageSize);
			disvardingItemThubnail->SetOpacity(1.f);
		}
		else {
			//defaultthumbnail?
		}
	}*/
}

void UInventoryDiscardPanel::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	disvardingItemThubnail->SetOpacity(0.f);
}
