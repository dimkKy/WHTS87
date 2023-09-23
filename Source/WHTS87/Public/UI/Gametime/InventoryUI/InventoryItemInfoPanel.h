// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemInfoPanel.generated.h"

class UPickupItemInfoBase;
class USizeBox;
class UTextBlock;
/**
 * one panel per inventory panel
 */
UCLASS(Abstract)
class WHTS87_API UInventoryItemInfoPanel : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual void AssignItemInfo(UPickupItemInfoBase* newItemInfo);
	virtual void SetWidthOverride(float deziredWidth);

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& context) const override;
#endif
protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		USizeBox* sizeBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* itemName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* itemDescription;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* itemWeight;
};
