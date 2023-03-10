// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WHTS87PlayerController.generated.h"

//class AHUDManager;

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
	//AHUDManager* GetHUDManager() const;
	virtual void SetViewTarget(class AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams = FViewTargetTransitionParams()) override;

protected:
	virtual void BeginPlay() override;
	virtual void AcknowledgePossession(APawn* P) override;
	virtual void OnUnPossess() override;
};
