// by Dmitry Kolontay


#include "UI/Gametime/InventoryDragDropOperation.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

/*bool UInventoryDragDropOperation::SaveWidgetPosition(UCanvasPanelSlot* canvasPanelSlot)
{
	if (IsValid(canvasPanelSlot)) {
		initialPosition = canvasPanelSlot->GetPosition();
		initialCanvasPanel = CastChecked<UCanvasPanel>(canvasPanelSlot->Parent);
		return true;
	}
	else
		return false;
}

bool UInventoryDragDropOperation::SaveWidgetPosition(FVector2D& slotPosition, UCanvasPanel* canvasPanel)
{
	if (IsValid(canvasPanel)) {
		initialPosition = slotPosition;
		initialCanvasPanel = canvasPanel;
		return true;
	}
	else
		return false;
}

UCanvasPanel* UInventoryDragDropOperation::GetWidgetInitialPosition(FVector2D& outLocalPosition)
{
	outLocalPosition = initialPosition;
	return initialCanvasPanel;
}*/
