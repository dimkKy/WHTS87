// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuUI.generated.h"

UENUM(BlueprintType)
enum class EMainMenuPage : uint8
{
	//Hide all other menus
	//None,
	//
	//Welcome,
	//
	NewGame,
	//
	LoadGame,
	//
	Settings,
};

class UWidgetSwitcher;
class UNewGameSubmenu;
class USettingsSubmenu;
class ULoadGameSubmenu;
class UButton;
/**
 * 
 */
UCLASS()
class WHTS87_API UMainMenuUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	void SetCurrentSubmenu(EMainMenuPage newPage);
protected:
	//EMainMenuPage currentSubmenu;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UWidgetSwitcher* submenuSwitcher;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UNewGameSubmenu* newGameSubmenu;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		ULoadGameSubmenu* loadGameSubmenu;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		USettingsSubmenu* settingsSubmenu;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* newGameMenuButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* loadGameMenuButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* settingsMenuButton;

	void SetCurrentSubmenu(UUserWidget* newSubmenu);
	UFUNCTION()
		void ToggleNewGameSubmenu();
	UFUNCTION()
		void ToggleLoadGameSubmenu();
	UFUNCTION()
		void ToggleSettingsSubmenu();
};
