// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"

class APickupItemContainer;
class UTextBlock;
class UImage;
class USizeBox;

/**
 *
 */
UCLASS()
class WHTS87_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	//void SetItemContainer(APickupItemContainer* NewItemContainer, float tileSize, UInventoryPanel* newOwningPanel);
	void UpdateSlot();
	virtual void RemoveFromParent() override;
	APickupItemContainer* GetItemContainer() const { return representedContainer.IsValid() ? representedContainer.Get() : nullptr; };
	//UInventoryPanel* GetInventoryPanel() const { return owningPanel; };
	//UPROPERTY(EditAnywhere, Category = "Config")
		//TSubclassOf<UItemInfoPanel> infopanelClass;
	//void ToggleInfopanelVisibility(bool bVisible);
protected:
	TWeakObjectPtr<APickupItemContainer> representedContainer;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* thumbnail;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* itemCount;
	//UInventoryPanel* owningPanel;
	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		//USizeBox* mainSizeBox;
	
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	void RemoveFromInventory();
	void UseItem();
	//void ShowInfoPanel();
	//bool UpdateItemInfopanel();

	//virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	//virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;	
};
