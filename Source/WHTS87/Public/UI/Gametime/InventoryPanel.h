// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryPanel.generated.h"

//class UInventoryComponent;
class UScrollBox;
class UCanvasPanel;
class UInventorySlot;
/**
 * 
 */
UCLASS()
class WHTS87_API UInventoryPanel : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	//virtual void SetInventory(UInventoryComponent* NewInventory);
	void UpdateAllSlots();
	float GetTileSize();
protected:
	//TWeakObjectPtr<UInventoryComponent> inventory;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UInventorySlot> slotClass;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UScrollBox* panelScrollBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* slotCanvas;

};
