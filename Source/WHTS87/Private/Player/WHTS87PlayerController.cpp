// by Dmitry Kolontay


#include "Player/WHTS87PlayerController.h"
#include "UI/HUDManager.h"
#include "Player/PlayerCharacter.h"
#include "UI/Gametime/InteractionHelper.h"
#include "UI/HUDManager.h"
//#include <CoreSystems/WHTS87UserSettings.h>

void AWHTS87PlayerController::OnToggleInventoryMenu()
{
	if (GEngine->GameViewport == nullptr) {
		return;
	}
	if (auto veiwTarget{ GetViewTarget() }) {
		if (veiwTarget->GetClass()->IsChildOf(APlayerCharacter::StaticClass())) {
			check(false);
			//GetHUDManager()->ToggleInventoryMenu();
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

AHUDManager* AWHTS87PlayerController::GetHUDManager() const
{
	return CastChecked<AHUDManager>(MyHUD);
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
	/*if (CastChecked<UWHTS87UserSettings>(GEngine->GetGameUserSettings())->showStartupMenu) {
		GetHUDManager().
	}*/

}

void AWHTS87PlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);
	check(P->GetClass()->IsChildOf(APlayerCharacter::StaticClass()));
	GetHUDManager()->OnPawnPossession();
}

void AWHTS87PlayerController::OnUnPossess()
{
	GetHUDManager()->OnPawnUnPossess();
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
