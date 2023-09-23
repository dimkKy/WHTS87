// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryMenu.generated.h"

class UInventorySlot;
class UInventoryPanel;
class UInventoryDiscardPanel;
class UCanvasPanel;
class UCanvasPanelSlot;
class UInventoryItemInfoPanel;
class UInventoryItemInfoDraggablePanel;
class UPickupItemInfoBase;
class UWidgetSwitcher;
//class UEquipmentPanel;

/**
 * 
 */
UCLASS(Abstract)
class WHTS87_API UInventoryMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
		void AddItemInfoPanel(UPickupItemInfoBase* itemInfo, FVector2D desiredAbsolutePosition);
	//void ToggleHoverItemInfopanel(UPickupItemInfoBase* itemInfo, bool bEnable, float desiredGLobalPositionY);
	//several panels possible
	void ToggleHoverItemInfopanel(UPickupItemInfoBase* itemInfo, bool bEnable, FVector2D desiredAbsolutePosition);
	//todo
	TSubclassOf<UInventorySlot> GetInventorySlotClass() { return itemInfoSlotClass; };
	float GetSlotTileSize();


#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& context) const override;
#endif
protected:
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* infoPanelsCanvas;
	//DuplicateTransient?
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UInventoryItemInfoPanel> hoveredItemInfoPanelClass;
	UPROPERTY()
		UCanvasPanelSlot* hoveredInfoPanelSlot;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UInventoryItemInfoDraggablePanel> draggableItemInfoPanelClass;
	UPROPERTY()
		TArray<UCanvasPanelSlot*> draggableInfoPanelSlots;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UInventorySlot> itemInfoSlotClass;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UInventoryPanel* playerInventoryPanel;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UInventoryDiscardPanel* discardPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UWidgetSwitcher* secondaryPanelSwitcher;
	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		//UEquipmentPanel* playerEqupmentPanel;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UInventoryPanel* secondaryInventoryPanel;
	
	
};
