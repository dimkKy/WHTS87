// by Dmitry Kolontay


#include "UI/HUDManager.h"
#include "UI/Gametime/GametimeUI.h"
#include "UI/Gametime/InteractionHelper.h"
#include "UI/Menus/MainMenuUI.h"
#include "Player/PlayerCharacter.h"

AHUDManager::AHUDManager() : 
	gametimeUI{nullptr}, mainMenuUI{nullptr}
{
}

void AHUDManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	check(PlayerOwner);
	check(IsValid(gametimeUIClass) && IsValid(mainMenuUIClass));
	if (gametimeUIClass) {
		gametimeUI = CreateWidget<UGametimeUI>(PlayerOwner, gametimeUIClass); 
	}
	if (mainMenuUIClass) {
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

void AHUDManager::SetUIType(EUIType newUIType, FViewTargetTransitionParams TransitionParams)
{
	switch (newUIType) {
	case EUIType::Player:
		if (mainMenuUI && mainMenuUI->IsInViewport()) {
			mainMenuUI->RemoveFromParent();
		}
		if (TransitionParams.BlendTime == 0.f) {
			if (gametimeUI && !gametimeUI->IsInViewport()) {
				gametimeUI->AddToPlayerScreen();
			}
		}
		else {
			//TODO
			check(false);
		}	
		break;
	case EUIType::MainMenu:
		if (gametimeUI && gametimeUI->IsInViewport())
			gametimeUI->RemoveFromParent();
		if (TransitionParams.BlendTime == 0.f) {
			if (mainMenuUI && !mainMenuUI->IsInViewport()) {
				mainMenuUI->AddToPlayerScreen();
			}
		}
		else {
			//TODO
			check(false);
		}
		break;
	case EUIType::Cinematic:
		//TODO
		check(false);
		break;
	case EUIType::NoUI:
		[[fallthrough]];
	default:
		
		break;
	}
}

UInteractionHelper* AHUDManager::GetInteractionHelper()
{
	return gametimeUI->GetInteractionHelper();
}

void AHUDManager::ToggleInventoryMenu()
{
	if (gametimeUI && gametimeUI->IsInViewport()) {
		//gametimeUI->ToggleMenu(EGametimeMenu::Inventory);
		//SetPlayerInteractionMode(gametimeUI->GetCurrentMenu() == EGametimeMenu::Walktime);
	}
}

void AHUDManager::BeginPlay()
{
	Super::BeginPlay();
}

#if WITH_EDITOR
EDataValidationResult AHUDManager::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult superResult{ Super::IsDataValid(ValidationErrors) };
	if (superResult != EDataValidationResult::Invalid) {
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