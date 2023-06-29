// by Dmitry Kolontay


#include "UI/Gametime/InventoryUI/InventorySlot.h"
//#include "Components/CanvasPanelSlot.h"
#include "Environment/PickupItemContainer.h"
//#include "UI/Gametime/InventoryMenu.h"
#include "UI/Gametime/InventoryUI/InventoryDragDropOperation.h"
#include "Components/InventoryComponent.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Environment/Pickups/PickupItemInfoBase.h"
#include "Engine/Texture2D.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"


#define LOCTEXT_NAMESPACE "InventorySlotMouseMenuActions"

void UInventorySlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	//outline
}

void UInventorySlot::SetItemContainer(APickupItemContainer& container, float tileSize)
{
	representedContainer = &container;
	check(false)
}

void UInventorySlot::RemoveContainerFromInventory(bool bEject, bool bBroadcast)
{
	if (auto* container{ representedContainer.Get() }) {
		if (auto* inventory{ container->GetOwnerInventory() }) {
			if (inventory->RemoveContainer(*container, true, false)) {
				RemoveFromParent();
			}
		}
	}
	else {
		//?
		check(false);
		RemoveFromParent();
	}
}

void UInventorySlot::UpdateSlot()
{
	if (auto* container{ representedContainer.Get() }) {
		itemCount->SetText(FText::FromString(FString::FromInt(container->GetItemsCount())));
		if (auto* possibleThumbnai{ container->GetItemInfo()->GetThumbnail() }) {
			thumbnail->SetBrushFromTexture(possibleThumbnai);
			
		}
		else {
			check(false);
		}
			
	}
	else {
		//call to update
		RemoveFromParent();
	}
}

UTexture2D* UInventorySlot::GetThumbnail()
{
	//return thumbnail ? Cast<UTexture2D>(thumbnail->Brush.GetResourceObject()) : nullptr;
	return thumbnail ? CastChecked<UTexture2D>(thumbnail->Background.GetResourceObject()) : nullptr;
}

void UInventorySlot::RemoveFromParent()
{
	Super::RemoveFromParent();
}
/*
UInventoryPanel* UInventorySlot::GetParentPanel() const
{
	if (UInventoryPanel* parentPanel{ Cast<UInventoryPanel>(Slot->Parent) })
		return parentPanel;
	return nullptr;
}*/

FReply UInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply reply{ Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent) };
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
		reply.DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}
	return reply;
}

FReply UInventorySlot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton) {
		//check command list, move to item info?
		FMenuBuilder menuBuilder{ true, nullptr };
		
		menuBuilder.BeginSection("Actions");
		{
			/*
			menuBuilder.AddMenuEntry(LOCTEXT("RemoveFromInventory", "Выбросить"), FText(),
				FSlateIcon(/*FEditorStyle::GetStyleSetName(), "ContentBrowser.AssetActions.Delete"*),
				FUIAction(FExecuteAction::CreateUObject(this, &ThisClass::RemoveFromInventory)));
			menuBuilder.AddMenuEntry(LOCTEXT("ShowItemDescription", "Показать описание"), FText(),
				FSlateIcon(/*FEditorStyle::GetStyleSetName(), "ContentBrowser.AssetActions.Delete"*),
				FUIAction(FExecuteAction::CreateUObject(owningPanel, &UInventoryPanel::AddItemInfoPanel, itemContainer->GetItemInfo())));
			menuBuilder.AddMenuEntry(representedContainer.getiteminfo.getactiontext), FText(),
				FSlateIcon(/*FEditorStyle::GetStyleSetName(), "ContentBrowser.AssetActions.Delete"*),
				FUIAction(FExecuteAction::CreateUObject(this, &ThisClass::UseItem)));
			*/
		}
		menuBuilder.EndSection();
		FWidgetPath widgetPath;
		FSlateApplication::Get().GeneratePathToWidgetUnchecked(TakeWidget(), widgetPath);
		//FSlateApplication::Get().getwidge
		//auto thisSWidget = TakeWidget();
		FSlateApplication::Get().PushMenu(
			TakeWidget(),
			widgetPath,
			menuBuilder.MakeWidget(),
			InMouseEvent.GetScreenSpacePosition(),
			FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu));

		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply UInventorySlot::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
		UseItem();
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

void UInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	UInventoryDragDropOperation* operation{NewObject<UInventoryDragDropOperation>()};
	//?
	operation->Payload = this;
	operation->DefaultDragVisual = this;
	operation->Pivot = EDragPivot::MouseDown;
	operation->localPivotPos = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	OutOperation = operation;
	//if?
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UInventorySlot::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	//?
	SetVisibility(Slot->Parent->GetVisibility());
}

void UInventorySlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//owningPanel->ToggleHoverItemInfopanel(itemContainer->GetItemInfo(), true, GetCachedGeometry().GetLocalPositionAtCoordinates(FVector2D(0.5f, 0.5f)).Y);
}

void UInventorySlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	//owningPanel->ToggleHoverItemInfopanel(itemContainer->GetItemInfo(), false);
}

void UInventorySlot::RemoveFromInventory()
{
	//everything via inventory panel?
	//representedContainer.IsValid() ? representedContainer.Get() : nullptr;
	/*check(IsValid(itemContainer));
	UInventory* owningInventory = itemContainer->GetOwnerInventory();
	check(IsValid(owningInventory));
	owningInventory->RemoveContainer(itemContainer, true, true);*/
}

void UInventorySlot::UseItem()
{
	/*check(IsValid(itemContainer));
	UInventory* owningInventory = itemContainer->GetOwnerInventory();
	check(IsValid(owningInventory));
	owningInventory->UseItem(itemContainer, owningInventory->GetOwner());*/
}

#undef LOCTEXT_NAMESPACE