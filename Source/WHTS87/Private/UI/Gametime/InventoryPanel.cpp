// by Dmitry Kolontay


#include "UI/Gametime/InventoryPanel.h"
#include "UI/Gametime/InventorySlot.h"
#include "Components/ScrollBox.h"
#include "Components/CanvasPanel.h"



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
}

float UInventoryPanel::GetTileSize()
{
    return 0.0f;
}

bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    return false;
}
