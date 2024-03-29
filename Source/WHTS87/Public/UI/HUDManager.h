// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDManager.generated.h"


class UGametimeUI;
class UMainMenuUI;
class UInteractionHelper;

enum class EUIType : uint8
{
	/*Playing in a world, view target is player character */
	Player,
	/*We are inside main menu*/
	MainMenu,
	/*Hide UI with possibility to skip*/
	Cinematic,
	/*Hide all UI*/
	NoUI,
};

/**
 * 
 */
UCLASS(Abstract)
class WHTS87_API AHUDManager : public AHUD
{
	GENERATED_BODY()
public:
	AHUDManager();
	virtual void PostInitializeComponents() override;

	void OnPawnPossession();
	void OnPawnUnPossess();
	void SetUIType(EUIType newUIType, FViewTargetTransitionParams TransitionParams);
	UInteractionHelper* GetInteractionHelper();

	//void ToggleInventoryMenu();
	
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& context) const override;
#endif
protected:

	virtual void BeginPlay() override;

	void ShowStartupMenu();

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UGametimeUI> gametimeUIClass;
	UPROPERTY()
		UGametimeUI* gametimeUI;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UMainMenuUI> mainMenuUIClass;
	UPROPERTY()
		UMainMenuUI* mainMenuUI;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UGametimeUI> startupMenuUIClass;
	UPROPERTY()
		UMainMenuUI* startupMenuUI;

	//UUserWidget* menus[static_cast<int32>(EUIType::NoUI)];
};
