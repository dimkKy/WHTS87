// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryMenu.generated.h"

class UInventoryPanel;
class UCanvasPanel;
class UCanvasPanelSlot;
class UInventoryItemInfoPanel;
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
		void AddItemInfoPanel(UPickupItemInfoBase* itemInfo);
	//void ToggleHoverItemInfopanel(UPickupItemInfoBase* itemInfo, bool bEnable, float desiredGLobalPositionY);
	//several panels possible
	void ToggleHoverItemInfopanel(UPickupItemInfoBase* itemInfo, bool bEnable, FVector2D desiredGlobalPosition);
protected:
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* infoPanelsCanvas;
	//DuplicateTransient?
	UPROPERTY()
		UCanvasPanelSlot* hoveredInfoPanelSlot;
	UPROPERTY()
		TArray<UCanvasPanelSlot*> freeInfoPanelSlots;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UInventoryPanel* playerInventoryPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UWidgetSwitcher* secondaryPanelSwitcher;
	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		//UEquipmentPanel* playerEqupmentPanel;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UInventoryPanel* secondaryInventoryPanel;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UInventoryItemInfoPanel> itemInfoPanelClass;
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
};
