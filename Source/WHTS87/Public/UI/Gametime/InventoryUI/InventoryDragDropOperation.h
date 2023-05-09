// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "InventoryDragDropOperation.generated.h"

//class UCanvasPanelSlot;
//class UCanvasPanel;
class UPanelWidget;
/**
 * 
 */
UCLASS()
class WHTS87_API UInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
public:
	UInventoryDragDropOperation();
	//UE_NODISCARD bool SaveWidgetPosition(UCanvasPanelSlot* canvasPanelSlot);
	//UE_NODISCARD bool SaveWidgetPosition(FVector2D& slotPosition, UCanvasPanel* canvasPanel);
	//UCanvasPanel* GetWidgetInitialPosition(FVector2D& outLocalPosition);
	FVector2D localPivotPos;
	FVector2D initialPosition;
	UPanelWidget* initialPanel;
	//UCanvasPanel* initialPanel;
protected:
	
};
