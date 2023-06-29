// by Dmitry Kolontay


#include "UI/Menus/MainMenuUI.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "UI/Menus/NewGameSubmenu.h"
#include "UI/Menus/LoadGameSubmenu.h"
#include "UI/Menus/SettingsSubmenu.h"
#include "WHTS87Utils.h"

void UMainMenuUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	newGameButton->SetClickMethod(EButtonClickMethod::PreciseClick);
	loadGameButton->SetClickMethod(EButtonClickMethod::PreciseClick);
	settingsButton->SetClickMethod(EButtonClickMethod::PreciseClick);

	newGameButton->OnClicked.AddDynamic(this, &UMainMenuUI::ToggleNewGameSubmenu);
	loadGameButton->OnClicked.AddDynamic(this, &UMainMenuUI::ToggleLoadGameSubmenu);
	settingsButton->OnClicked.AddDynamic(this, &UMainMenuUI::ToggleSettingsSubmenu);
	
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
	
	case EMainMenuPage::Settings:
		SetCurrentSubmenu(*settingsSubmenu);
		break;
	case EMainMenuPage::NewGame:
		SetCurrentSubmenu(*newGameSubmenu);
		break;
	case EMainMenuPage::LoadGame:	
		[[fallthrough]];
	default:
		SetCurrentSubmenu(*loadGameSubmenu);
		break;
	}
}

void UMainMenuUI::SetCurrentSubmenu(UUserWidget& newSubmenu)
{
	//REDO
	if (&newSubmenu == submenuSwitcher->GetActiveWidget()) {
		switch (submenuSwitcher->GetVisibility()){
		case ESlateVisibility::Visible:
			submenuSwitcher->SetVisibility(ESlateVisibility::Hidden);
			break;
		case ESlateVisibility::Hidden:
			submenuSwitcher->SetVisibility(ESlateVisibility::Visible);
			break;
		default:
			break;
		}
	}
	else {
		//Parenting to some abstract submenuWidget does not look like an effective way either
		if (WHTS87Utils::IsIn(newSubmenu, 
			loadGameSubmenu, newGameSubmenu, settingsSubmenu)) {
			submenuSwitcher->SetActiveWidget(&newSubmenu);
			submenuSwitcher->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UMainMenuUI::ToggleNewGameSubmenu()
{
	SetCurrentSubmenu(*newGameSubmenu);
}

void UMainMenuUI::ToggleLoadGameSubmenu()
{
	SetCurrentSubmenu(*loadGameSubmenu);
}

void UMainMenuUI::ToggleSettingsSubmenu()
{
	SetCurrentSubmenu(*settingsSubmenu);
}
