// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"

class APickupItemContainer;
class UTextBlock;
class UImage;
class UTexture2D;
class USizeBox;
class UInventoryPanel;
class UInventoryComponent;

/**
 *
 */
UCLASS()
class WHTS87_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual void RemoveFromParent() override;

	void SetItemContainer(APickupItemContainer& container, float tileSize);
	void RemoveContainerFromInventory(bool bEject = false, bool bBroadcast = true);
	void UpdateSlot();

	APickupItemContainer* GetItemContainer() 
		{ return representedContainer.Get(); };

	UTexture2D* GetThumbnail();
	//UInventoryPanel* GetParentPanel () const;
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
};
