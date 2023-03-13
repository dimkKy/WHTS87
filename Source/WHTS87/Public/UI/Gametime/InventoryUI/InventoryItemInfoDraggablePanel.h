// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "UI/Gametime/InventoryUI/InventoryItemInfoPanel.h"
#include "InventoryItemInfoDraggablePanel.generated.h"

class UButton;
class UScaleBox;
/**
 * 
 */
UCLASS()
class WHTS87_API UInventoryItemInfoDraggablePanel : public UInventoryItemInfoPanel
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UScaleBox* closeButtonScaleBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* closeButton;
	UFUNCTION()
		void OnCloseButtonClicked();

};
