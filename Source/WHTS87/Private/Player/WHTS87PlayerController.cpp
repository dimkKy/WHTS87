// by Dmitry Kolontay


#include "Player/WHTS87PlayerController.h"
#include "UI/HUDManager.h"
#include "Player/PlayerCharacter.h"

void AWHTS87PlayerController::OnToggleInventoryMenu()
{
	if (GEngine->GameViewport != nullptr && IsValid(GetViewTarget()) && GetViewTarget()->GetClass()->IsChildOf(APlayerCharacter::StaticClass())) {
		if (AHUDManager * HUD{ Cast<AHUDManager>(MyHUD) }) {
			HUD->ToggleInventoryMenu();
		}
	}
}

/*void AWHTS87PlayerController::OnTogglePauseMenu()
{
}*/

void AWHTS87PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("ToggleInventoryMenu", IE_Pressed, this, &AWHTS87PlayerController::OnToggleInventoryMenu);
}

void AWHTS87PlayerController::SetViewTarget(AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams)
{
	Super::SetViewTarget(NewViewTarget, TransitionParams);
	//if (GetViewTarget()->GetClass()->IsChildOf(APlayerCharacter::StaticClass()))
	if (AHUDManager* HUD{ Cast<AHUDManager>(MyHUD) }) {
		HUD->OnViewTargetChange(NewViewTarget, TransitionParams);
	}
}

void AWHTS87PlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AWHTS87PlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);
	check(P->GetClass()->IsChildOf(APlayerCharacter::StaticClass()));
	if (AHUDManager* HUD{ Cast<AHUDManager>(MyHUD) }) {
		HUD->OnPawnPossession();
	}
}

void AWHTS87PlayerController::OnUnPossess()
{
	if (AHUDManager* HUD{ Cast<AHUDManager>(MyHUD) }) {
		HUD->OnPawnUnPossess();
	}
	Super::OnUnPossess();
}
