// by Dmitry Kolontay


#include "UI/Gametime/GametimeUI.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UI/Gametime/InventoryMenu.h"
#include "UI/Gametime/JournalMenu.h"

void UGametimeUI::UpdatePlayerHealth(int32 NewHealth)
{
	textHealth->SetText(FText::FromString(FString::FromInt(NewHealth) + "%"));
}

void UGametimeUI::OnPawnPossession()
{
	SetCurrentMenu(walktimeCanvas);
	//OnPlayerUIChangedEventHandle = Cast<APlayerCharacter>(GetOwningPlayerPawn())->OnPlayerUIChanged.AddUObject(this, &UGametimeUI::UpdatePlayerHealth);
	//Cast<APlayerCharacter>(GetOwningPlayerPawn())->OnPlayerUIChanged.add
	

	//mainInventoryPanel->SetOwningGametimeUI(this);
	//mainInventoryPanel->SetInventory(Cast<APlayerCharacter>(GetOwningPlayerPawn())->GetInventory());
	//OnPlayerInventoryChangedEventHandle = Cast<APlayerCharacter>(GetOwningPlayerPawn())->GetInventory()->OnInventoryUpdated.AddUObject(mainInventoryPanel, )
}

void UGametimeUI::OnPawnUnPossess()
{
	//CastChecked<APlayerCharacter>(GetOwningPlayerPawn())->OnPlayerUIChanged.Remove(OnPlayerUIChangedEventHandle);
	//mainInventoryPanel->SetInventory(nullptr);
}

void UGametimeUI::SetCurrentMenu(EGametimeMenu newMenu)
{
	switch (newMenu)
	{
	case EGametimeMenu::Walktime:
		SetCurrentMenu(walktimeCanvas);
		break;
	case EGametimeMenu::Inventory:
		SetCurrentMenu(inventoryMenu);
		break;
	case EGametimeMenu::Journal:
		SetCurrentMenu(journalMenu);
		break;
	default:
		break;
	}
}

void UGametimeUI::SetCurrentMenu(UWidget* newSubmenu)
{
	if (newSubmenu) {
		/*if (newSubmenu == menuSwitcher->GetActiveWidget()) {
			if (menuSwitcher->GetVisibility() == ESlateVisibility::Visible)
				menuSwitcher->SetVisibility(ESlateVisibility::Hidden);
			else
				menuSwitcher->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			//Parenting to some abstract submenuWidget does not look like an effective way either
			if (newSubmenu == inventoryMenu || newSubmenu == journalMenu || newSubmenu == walktimeCanvas) {
				menuSwitcher->SetActiveWidget(newSubmenu);
				menuSwitcher->SetVisibility(ESlateVisibility::Visible);
			}
		}*/
		if (newSubmenu == menuSwitcher->GetActiveWidget()) {
			menuSwitcher->SetActiveWidget(walktimeCanvas);
		}
		else {
			//Parenting to some abstract submenuWidget does not look like an effective way either
			if (newSubmenu == journalMenu || newSubmenu == walktimeCanvas) {
				menuSwitcher->SetActiveWidget(newSubmenu);
			}
		}
	}
}
