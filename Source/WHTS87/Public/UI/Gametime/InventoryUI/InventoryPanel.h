// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryPanel.generated.h"

class UInventoryComponent;
class UScrollBox;
class UCanvasPanel;
class UInventoryMenu;
/**
 * 
 */
UCLASS(Abstract)
class WHTS87_API UInventoryPanel : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	//virtual void SetInventory(UInventoryComponent* NewInventory);
	void UpdateAllSlots();
	//UInventoryMenu* GetParentMenu() const;
	void SetNewInventory(UInventoryComponent* newInventory);
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& context) const override;
#endif
protected:
	//?
	TWeakObjectPtr<UInventoryComponent> representedInventory;
	//move to inventory menu?
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UScrollBox* scrollBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* slotCanvas;

};
