// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GametimeUI.generated.h"

UENUM(BlueprintType)
enum class EGametimeMenu : uint8
{
	/**Basic gametime UI should be always visible*/
	//Persistent,
	/**Basic gametime UI should not be visible while other menus are active*/
	Walktime,
	/***/
	Inventory,
	/***/
	Journal,
	/***/
	//Pause,
};

class UTextBlock;
class UImage;
class UInventoryMenu;
class UJournalMenu;
class UCanvasPanel;
class UWidgetSwitcher;
class UInteractionHelper;

/**
 * 
 */
UCLASS(Abstract)
class WHTS87_API UGametimeUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
		void UpdatePlayerHealth(int32 NewHealth);
	void OnPawnPossession();
	void OnPawnUnPossess();
	void SetCurrentMenu(EGametimeMenu newMenu);
	UInteractionHelper* GetInteractionHelper();
protected:
	void SetCurrentMenu(UWidget* newMenu);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* persistentCanvas;
	//TODO
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* textHealth;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* walktimeCanvas;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UInteractionHelper* interactionHelper;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* crosshairImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UWidgetSwitcher* menuSwitcher;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UInventoryMenu* inventoryMenu;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UJournalMenu* journalMenu;

	//FDelegateHandle OnPlayerUIChangedEventHandle;
};
