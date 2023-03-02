// by Dmitry Kolontay


#include "UI/Menus/MainMenuUI.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "UI/Menus/NewGameSubmenu.h"
#include "UI/Menus/LoadGameSubmenu.h"
#include "UI/Menus/SettingsSubmenu.h"

void UMainMenuUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	newGameMenuButton->SetClickMethod(EButtonClickMethod::PreciseClick);
	loadGameMenuButton->SetClickMethod(EButtonClickMethod::PreciseClick);
	settingsMenuButton->SetClickMethod(EButtonClickMethod::PreciseClick);

	newGameMenuButton->OnClicked.AddDynamic(this, &UMainMenuUI::ToggleNewGameSubmenu);
	loadGameMenuButton->OnClicked.AddDynamic(this, &UMainMenuUI::ToggleLoadGameSubmenu);
	settingsMenuButton->OnClicked.AddDynamic(this, &UMainMenuUI::ToggleSettingsSubmenu);
	
	submenuSwitcher->SetActiveWidget(newGameSubmenu);
	submenuSwitcher->SetVisibility(ESlateVisibility::Hidden);
	//currentSubmenu = EMainMenuPage::NewGame;
}

void UMainMenuUI::SetCurrentSubmenu(EMainMenuPage newSubmenu)
{
	switch (newSubmenu) {
		/*case EMainMenuPage::None:
			submenuSwitcher->SetVisibility(ESlateVisibility::Hidden);
			currentSubmenu = newSubmenu;
			return;*/
	case EMainMenuPage::NewGame:
		SetCurrentSubmenu(newGameSubmenu);
		break;
	case EMainMenuPage::LoadGame:
		SetCurrentSubmenu(loadGameSubmenu);
		break;
	case EMainMenuPage::Settings:
		SetCurrentSubmenu(settingsSubmenu);
		break;
	default:
		break;
	}
}

void UMainMenuUI::SetCurrentSubmenu(UUserWidget* newSubmenu)
{
	if (newSubmenu) {
		if (newSubmenu == submenuSwitcher->GetActiveWidget()) {
			if (submenuSwitcher->GetVisibility() == ESlateVisibility::Visible)
				submenuSwitcher->SetVisibility(ESlateVisibility::Hidden);
			else
				submenuSwitcher->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			//Parenting to some abstract submenuWidget does not look like an effective way either
			if (newSubmenu == loadGameSubmenu || newSubmenu == newGameSubmenu || newSubmenu == settingsSubmenu) {
				submenuSwitcher->SetActiveWidget(newSubmenu);
				submenuSwitcher->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void UMainMenuUI::ToggleNewGameSubmenu()
{
	SetCurrentSubmenu(newGameSubmenu);
}

void UMainMenuUI::ToggleLoadGameSubmenu()
{
	SetCurrentSubmenu(loadGameSubmenu);
}

void UMainMenuUI::ToggleSettingsSubmenu()
{
	SetCurrentSubmenu(settingsSubmenu);
}
