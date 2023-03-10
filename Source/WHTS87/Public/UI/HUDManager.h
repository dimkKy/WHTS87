// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDManager.generated.h"


class UGametimeUI;
class UMainMenuUI;
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
	void OnViewTargetChange(AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams);
	void ToggleInventoryMenu();

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
	
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UGametimeUI> gametimeUIClass;
	UPROPERTY()
		UGametimeUI* gametimeUI;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UGametimeUI> mainMenuUIClass;
	UPROPERTY()
		UMainMenuUI* mainMenuUI;

	/// <summary>
	/// SetInputMode replacement. Limited to UI- or Game- only
	/// </summary>
	/// <param name="bNewInteractionModeGame"> set false to change to UI mode</param>
	/// <param name="bShowMouseInUIMode">whether or not to let player interact via point-n-clicking. UI mode only</param>
	void SetPlayerInteractionMode(bool bInteractionModeGame, bool bShowMouseInUIMode = true);
};
