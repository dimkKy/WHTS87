// by Dmitry Kolontay


#include "CoreSystems/WHTS87GameModeBase.h"
#include "CoreSystems/StairwellManager.h"
#include "Player/WHTS87PlayerController.h"
#include "Player/PlayerCharacter.h"
#include "UI/HUDManager.h"
#include "EngineUtils.h"
#include "CoreSystems/IntroCamera.h"
#include "Kismet/GameplayStatics.h"

AWHTS87GameModeBase::AWHTS87GameModeBase() :
	localController{ nullptr }, mainMenuLevel{ nullptr }
{

}

void AWHTS87GameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	//показать
	if (AWHTS87PlayerController* controller = Cast<AWHTS87PlayerController>(NewPlayer)) {
		localController = controller;
		//load settings

		/*if (USpecificSettingsSave* loadedSettings = Cast<USpecificSettingsSave>(UGameplayStatics::LoadGameFromSlot(FPlatformProcess::UserName(), 0)))
		{
			
		}
		else {
			check(IsValid(startupMenuClass));
			startupMenu = CreateWidget<UStartupMenu>(GetWorld(), startupMenuClass);

			startupMenu->continueButton->OnClicked.AddDynamic(this, &APersistentGameMode::OnStartupMenuClose);

			startupMenu->AddToPlayerScreen(999);
		}
		localPlayerController = playerController;*/
	}

	/*
	FInputModeUIOnly inputModeData;
	//inputModeData.SetHideCursorDuringCapture(false);
	inputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
	NewPlayer->SetInputMode(inputModeData);
	//NewPlayer->SetInputMode(FInputModeGameOnly());
	//NewPlayer->SetCinematicMode(true, true, true);
	NewPlayer->bShowMouseCursor = true;*/
	/*
	FAsyncLoadGameFromSlotDelegate loadedDelegate;
	loadedDelegate.BindUObject(this, &APersistentGameMode::OnGameSpecificSettingsLoad);
	UGameplayStatics::AsyncLoadGameFromSlot(FString(FPlatformProcess::UserName()), 0, loadedDelegate);*/
}

void AWHTS87GameModeBase::OnGameplayStart()
{
	UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(GetWorld(), mainMenuLevel, {}, true);

	localController->SetViewTarget(localController->GetPawn());
#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "OnNewGameStart");
#endif
	//GetWorld()->GetSubsystem<UStairwellManager>()->OnGameplayStart();
}

#if WITH_EDITOR
EDataValidationResult AWHTS87GameModeBase::IsDataValid(TArray<FText>& ValidationErrors)
{
	Super::IsDataValid(ValidationErrors);

	if (mainMenuLevel) {
		int32 introCamerasCount{ 0 };
		for (TActorIterator<AActor> it{ mainMenuLevel->GetWorld(), AIntroCamera::StaticClass() }; it; ++it)
		{
			introCamerasCount++;
		}
		if (introCamerasCount != 1) {
			ValidationErrors.Add(FText::FromString("There should be exactly one intro camera in main level"));
		}
	}
	else {
		ValidationErrors.Add(FText::FromString("Invalid mainMenuLevel"));
	}

	if (HUDClass && HUDClass->IsChildOf<AHUDManager>()) {
		if (!HUDClass->IsInBlueprint()) {
			ValidationErrors.Add(FText::FromString("Use blueprinted AHUDManager"));
		}
	}
	else {
		ValidationErrors.Add(FText::FromString("Invalid HUDClass"));
	}

	if (PlayerControllerClass && PlayerControllerClass->IsChildOf<AWHTS87PlayerController>()) {

	}
	else {
		ValidationErrors.Add(FText::FromString("Invalid PlayerControllerClass"));
	}

	if (DefaultPawnClass && DefaultPawnClass->IsChildOf<APlayerCharacter>()) {

	}
	else {
		ValidationErrors.Add(FText::FromString("Invalid DefaultPawnClass"));
	}

	return ValidationErrors.Num() > 0 ?
		EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif

void AWHTS87GameModeBase::StartPlay()
{
	Super::StartPlay();
	//GetWorld()->GetSubsystem<UStairwellManager>()->Init();
}
