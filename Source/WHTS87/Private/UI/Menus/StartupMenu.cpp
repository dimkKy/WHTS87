// by Dmitry Kolontay


#include "UI/Menus/StartupMenu.h"
#include "Components/Button.h"

void UStartupMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	continueButton->SetClickMethod(EButtonClickMethod::PreciseClick);
}
