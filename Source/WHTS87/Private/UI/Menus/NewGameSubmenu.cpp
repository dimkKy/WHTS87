// by Dmitry Kolontay


#include "UI/Menus/NewGameSubmenu.h"
#include "Components/Button.h"

void UNewGameSubmenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	startButton->SetClickMethod(EButtonClickMethod::PreciseClick);
}

void UNewGameSubmenu::OnNewGameStartButtonPressed()
{
}
