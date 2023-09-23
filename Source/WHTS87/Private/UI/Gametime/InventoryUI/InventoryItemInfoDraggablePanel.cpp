// by Dmitry Kolontay


#include "UI/Gametime/InventoryUI/InventoryItemInfoDraggablePanel.h"
#include "UI/Gametime/InventoryUI/InventoryDragDropOperation.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"
#include "Components/ScaleBox.h"
#include "Fonts/FontMeasure.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

void UInventoryItemInfoDraggablePanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	closeButton->SetClickMethod(EButtonClickMethod::PreciseClick);
	closeButton->OnClicked.AddDynamic(this, &UInventoryItemInfoDraggablePanel::OnCloseButtonClicked);
	closeButtonScaleBox->SetStretch(EStretch::Type::ScaleToFitY);
	//OnWidgetRebuilt
}

FReply UInventoryItemInfoDraggablePanel::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply{ Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent) };
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
		Reply.DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
		return Reply;
	}
	return Reply;
}

FReply UInventoryItemInfoDraggablePanel::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//highlignt items
	return FReply::Unhandled();
}

void UInventoryItemInfoDraggablePanel::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	//new object?
	UInventoryDragDropOperation* dragDropOperation = NewObject<UInventoryDragDropOperation>();
	//?
	dragDropOperation->Payload = this;
	dragDropOperation->DefaultDragVisual = this;
	dragDropOperation->Pivot = EDragPivot::MouseDown;
	dragDropOperation->localPivotPos = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	if (UCanvasPanelSlot* canvasPanelSlot{ Cast<UCanvasPanelSlot>(Slot) }) {
		dragDropOperation->initialPosition = canvasPanelSlot->GetPosition();
		dragDropOperation->initialPanel = canvasPanelSlot->Parent;
	}
	//FVector2D LocalMousePosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	OutOperation = dragDropOperation;
	//if?
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UInventoryItemInfoDraggablePanel::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	if (UInventoryDragDropOperation* dragDropOperation{ Cast<UInventoryDragDropOperation>(InOperation) }) {
		if (UCanvasPanel* canvasPanel{ Cast<UCanvasPanel>(dragDropOperation->initialPanel) }) {
			UCanvasPanelSlot* canvasSlot{ canvasPanel->AddChildToCanvas(this) };
			canvasSlot->SetPosition(dragDropOperation->initialPosition);
			canvasSlot->SetAutoSize(true);
			SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UInventoryItemInfoDraggablePanel::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	//?
	RemoveFromParent();
}

void UInventoryItemInfoDraggablePanel::OnCloseButtonClicked()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

#if WITH_EDITOR
EDataValidationResult UInventoryItemInfoDraggablePanel::IsDataValid(FDataValidationContext& context) const
{
	Super::IsDataValid(context);
	if (closeButton->Slot->Parent != closeButtonScaleBox)
		context.AddError(FText::FromString("closeButton is supposed to be placed inside closeButtonScaleBox"));

	return context.GetNumErrors() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif
