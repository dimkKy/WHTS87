// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryDiscardPanel.generated.h"

class UImage;
//class USizeBox;
class UBorder;
/**
 * 
 */
UCLASS(Abstract)
class WHTS87_API UInventoryDiscardPanel : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
protected:
	/*UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		USizeBox* sizeBox;*/
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UBorder* border;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* itemThumbnail;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
