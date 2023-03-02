// by Dmitry Kolontay


#include "UI/HUDManager.h"
#include "UI/Gametime/GametimeUI.h"
#include "UI/Menus/MainMenuUI.h"
#include "Player/PlayerCharacter.h"

AHUDManager::AHUDManager() : gametimeUI{nullptr}, mainMenuUI{nullptr}
{
}

void AHUDManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	check(PlayerOwner);
	check(IsValid(gametimeUIClass) && IsValid(mainMenuUIClass));
	if (IsValid(gametimeUIClass)) {
		gametimeUI = CreateWidget<UGametimeUI>(PlayerOwner, gametimeUIClass); 
	}
	if (IsValid(mainMenuUIClass)) {
		mainMenuUI = CreateWidget<UMainMenuUI>(PlayerOwner, mainMenuUIClass);
	}
}

void AHUDManager::OnPawnPossession()
{
	if (gametimeUI) {
		//gametimeUI->OnPawnPossession();
	}
}

void AHUDManager::OnPawnUnPossess()
{
	if (gametimeUI) {
		//gametimeUI->OnPawnUnPossess();
		//gametimeUI->RemoveFromViewport();
		if(gametimeUI->IsInViewport())
			gametimeUI->RemoveFromParent();
	}
}

void AHUDManager::OnViewTargetChange(AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams)
{
	UClass* viewTargetClass{ NewViewTarget->GetClass() };
	if (viewTargetClass->IsChildOf(APlayerCharacter::StaticClass())) {
		if (gametimeUI && !gametimeUI->IsInViewport()) {
			gametimeUI->AddToPlayerScreen();
		}
		SetPlayerInteractionMode(true);
	}
	else {
		if (gametimeUI && gametimeUI->IsInViewport())
			gametimeUI->RemoveFromViewport();
		SetPlayerInteractionMode(false);
		PlayerOwner->SetInputMode(FInputModeUIOnly::FInputModeUIOnly());
	}
}

void AHUDManager::ToggleInventoryMenu()
{
	if (gametimeUI && gametimeUI->IsInViewport() && PlayerOwner->GetViewTarget()->GetClass()->IsChildOf(APlayerCharacter::StaticClass())) {
		//gametimeUI->ToggleMenu(EGametimeMenu::Inventory);
		//SetPlayerInteractionMode(gametimeUI->GetCurrentMenu() == EGametimeMenu::Walktime);
	}
}

void AHUDManager::BeginPlay()
{
	Super::BeginPlay();
}

void AHUDManager::SetPlayerInteractionMode(bool bInteractionModeGame, bool bShowMouseInUIMode)
{
	if (bInteractionModeGame) {
		PlayerOwner->SetInputMode(FInputModeGameOnly::FInputModeGameOnly());
		PlayerOwner->bShowMouseCursor = false;
		//some checks
		PlayerOwner->SetCinematicMode(false, false, false, true, true);
	}
	else {
		FInputModeGameAndUI inputMode;
		inputMode.SetHideCursorDuringCapture(false);
		PlayerOwner->SetInputMode(inputMode);

		PlayerOwner->bShowMouseCursor = bShowMouseInUIMode;
		PlayerOwner->SetCinematicMode(true, false, false, true, true);
		//TODO
		if (bShowMouseInUIMode) {

		}
		else {

		}
	}
}

#if WITH_EDITOR
EDataValidationResult AHUDManager::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult superResult{ Super::IsDataValid(ValidationErrors) };
	if (superResult == EDataValidationResult::Valid) {
		if (!IsValid(gametimeUIClass))
			ValidationErrors.Add(FText::FromString("Invalid gametimeUIClass"));
		if (!IsValid(mainMenuUIClass))
			ValidationErrors.Add(FText::FromString("Invalid mainMenuUIClass"));
		if (ValidationErrors.Num() > 0) {
			superResult = EDataValidationResult::Invalid;
		}
	}
	return superResult;
}
#endif