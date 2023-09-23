// by Dmitry Kolontay


#include "UI/Menus/StartupMenu.h"
#include "Components/Button.h"
#include <CoreSystems/WHTS87UserSettings.h>

void UStartupMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	backButton->SetClickMethod(EButtonClickMethod::PreciseClick);
	backButton->OnClicked.AddDynamic(this, &UStartupMenu::RemoveFromParent);

	saveExitButton->SetClickMethod(EButtonClickMethod::PreciseClick);
	saveExitButton->OnClicked.AddDynamic(this, &UStartupMenu::OnSaveExitButtonPressed);
}

/*void UStartupMenu::OnBackButtonPressed()
{
	RemoveFromParent();
}*/

void UStartupMenu::OnSaveExitButtonPressed()
{
	auto settings{ CastChecked<UWHTS87UserSettings>(GEngine->GetGameUserSettings()) };
	//TODO
	check(false);
	settings->showStartupMenu = false;
	settings->privateMode = true;
	settings->ApplyNonResolutionSettings();
	settings->SaveSettings();
	RemoveFromParent();
}
