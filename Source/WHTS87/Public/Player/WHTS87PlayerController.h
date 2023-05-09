// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WHTS87PlayerController.generated.h"

class AHUDManager;
class UInteractionHelper;

/**
 * 
 */
UCLASS()
class WHTS87_API AWHTS87PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void OnToggleInventoryMenu();
	//void OnToggleJournalMenu();
	//void OnTogglePauseMenu();
	virtual void SetupInputComponent() override;
	FORCEINLINE UE_NODISCARD AHUDManager* GetHUDManager() const;
	//AHUDManager* GetHUDManager() const;
	virtual void SetViewTarget(class AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams = FViewTargetTransitionParams()) override;
	UInteractionHelper* GetInteractionHelper();
protected:
	virtual void BeginPlay() override;
	virtual void AcknowledgePossession(APawn* P) override;

	virtual void OnUnPossess() override;

	/// <summary>
	/// SetInputMode replacement. Limited to UI- or Game- only
	/// </summary>
	/// <param name="bNewInteractionModeGame"> set false to change to UI mode</param>
	/// <param name="bShowMouseInUIMode">whether or not to let player interact via point-n-clicking. UI mode only</param>
	void SetPlayerInteractionMode(bool bInteractionModeGame, bool bShowMouseInUIMode = true);
};
