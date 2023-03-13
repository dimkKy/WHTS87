// by Dmitry Kolontay


#include "Player/WHTS87PlayerController.h"
#include "UI/HUDManager.h"
#include "Player/PlayerCharacter.h"
#include "UI/Gametime/InteractionHelper.h"

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
	UClass* viewTargetClass{ NewViewTarget->GetClass() };
	if (AHUDManager * HUD{ Cast<AHUDManager>(MyHUD) }) {
		if (viewTargetClass->IsChildOf(APlayerCharacter::StaticClass())) {
			HUD->SetUIType(EUIType::Player, TransitionParams);
			return;
		}
		
	}
	else
		return;
}

UInteractionHelper* AWHTS87PlayerController::GetInteractionHelper()
{
	AActor* viewTarget{ GetViewTarget() };
	if (GEngine->GameViewport != nullptr && viewTarget != nullptr && viewTarget->GetClass()->IsChildOf(APlayerCharacter::StaticClass())) {
		if (AHUDManager * HUD{ Cast<AHUDManager>(MyHUD) }) {
			return HUD->GetInteractionHelper();
		}
	}
	return nullptr;
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

void AWHTS87PlayerController::SetPlayerInteractionMode(bool bInteractionModeGame, bool bShowMouseInUIMode)
{
	if (bInteractionModeGame) {
		SetInputMode(FInputModeGameOnly::FInputModeGameOnly());
		bShowMouseCursor = false;
		//some checks
		SetCinematicMode(false, false, false, true, true);
	}
	else {
		FInputModeGameAndUI inputMode;
		inputMode.SetHideCursorDuringCapture(false);
		SetInputMode(inputMode);

		bShowMouseCursor = bShowMouseInUIMode;
		SetCinematicMode(true, false, false, true, true);
		//TODO
		if (bShowMouseInUIMode) {

		}
		else {

		}
	}
}
