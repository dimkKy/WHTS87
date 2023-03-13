// by Dmitry Kolontay


#include "UI/Gametime/InventoryUI/InventoryMenu.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "UI/Gametime/InventoryUI/InventoryItemInfoPanel.h"
#include "UI/Gametime/InventoryUI/InventoryItemInfoDraggablePanel.h"
#include "Environment/Pickups/PickupItemInfoBase.h"
//#include "Blueprint/UserWidget.h"

void UInventoryMenu::AddItemInfoPanel(UPickupItemInfoBase* itemInfo)
{
	for (auto const& infoPanelSlot : draggableInfoPanelSlots)
	{
		/*if (infoPanelSlot->Content->GetItemInfo() == itemInfo) {
			if (infoPanelSlot->Content->GetVisibility() != ESlateVisibility::Visible)
				infoPanelSlot->Content->SetVisibility(ESlateVisibility::Visible);

			//higlight and move
			return;
		}*/
	}
	UInventoryItemInfoPanel* newInfoPanel{ CreateWidget<UInventoryItemInfoPanel>(this, hoveredItemInfoPanelClass) };
	//newInfoPanel->SetItemInfo(itemInfo, false, GetCachedGeometry().GetLocalSize().X);
	newInfoPanel->SetVisibility(ESlateVisibility::Visible);

	UCanvasPanelSlot* newSlot{ infoPanelsCanvas->AddChildToCanvas(newInfoPanel) };
	newSlot->SetAutoSize(true);
	draggableInfoPanelSlots.Add(newSlot);
}

void UInventoryMenu::ToggleHoverItemInfopanel(UPickupItemInfoBase* itemInfo, bool bEnable, FVector2D desiredGlobalPosition)
{
	check(IsValid(itemInfo));
	//UPickupItemInfoBase* currentItemInfo{ CastChecked<UPickupItemInfoBase>(hoveredInfoPanelSlot->Content)->GetItemInfo()};
	//perepisat govno
	/*if (bEnable) {
		hoveredInfopanel->SetVisibility(ESlateVisibility::Visible);
		if (itemInfo != currentItemInfo) {
			hoveredInfopanel->SetItemInfo(itemInfo, true, GetCachedGeometry().GetLocalSize().X);
		}


		FVector2D thisPositionLU{ GetCachedGeometry().GetLocalPositionAtCoordinates({0.f, 0.f}) };
		float spawnPositionY{ desiredPositionY + thisPositionLU.Y - MainScrollBox->GetScrollOffset() };

		hoveredInfopanelSlot->SetPosition({ thisPositionLU.X,
			spawnPositionY + hoveredInfopanelSlot->Content->GetDesiredSize().Y > hoveredInfopanelSlot->Parent->GetCachedGeometry().GetLocalSize().Y ?
			thisPositionLU.Y : spawnPositionY });

		//hoveredInfopanel->InvalidateLayoutAndVolatility();
		//hoveredInfopanel->ForceLayoutPrepass();
		//hoveredInfopanel->SetPositionInViewport()
		//hoveredInfopanelSlot->SetPosition

	}
	else {
		if (itemInfo != currentItemInfo)
			return false;
		hoveredInfopanel->SetVisibility(ESlateVisibility::Collapsed);
	}*/
	return;
}

float UInventoryMenu::GetSlotTileSize()
{
	return 0.0f;
}

void UInventoryMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//SetPositionInViewport
	//check(IsValid(hoveredItemInfoPanelClass));
	//hoveredInfoPanelSlot = infoPanelsCanvas->AddChildToCanvas(CreateWidget<UItemInfoPanel>(this, hoveredItemInfoPanelClass));
}

bool UInventoryMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}

#if WITH_EDITOR
EDataValidationResult UInventoryMenu::IsDataValid(TArray<FText>& ValidationErrors)
{
	//?
	EDataValidationResult superResult{ Super::IsDataValid(ValidationErrors) };
	if (superResult == EDataValidationResult::Valid) {
		if (IsValid(hoveredItemInfoPanelClass)) {
			if(hoveredItemInfoPanelClass.Get()->IsChildOf(UInventoryItemInfoDraggablePanel::StaticClass()))
				ValidationErrors.Add(FText::FromString("Invalid hoveredItemInfoPanelClass: should not be derived from UInventoryItemInfoDraggablePanel"));
		}
		else 
			ValidationErrors.Add(FText::FromString("Invalid hoveredItemInfoPanelClass"));
		if (!IsValid(draggableItemInfoPanelClass)) 
			ValidationErrors.Add(FText::FromString("Invalid draggableItemInfoPanelClass"));
			
		if (ValidationErrors.Num() > 0) {
			superResult = EDataValidationResult::Invalid;
		}
	}
	return superResult;
}
#endif
