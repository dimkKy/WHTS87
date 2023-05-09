// by Dmitry Kolontay


#include "CoreSystems/WHTS87GameModeBase.h"
#include "CoreSystems/StairwellManager.h"
#include "Player/WHTS87PlayerController.h"
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

void AWHTS87GameModeBase::StartPlay()
{
	Super::StartPlay();
	GetWorld()->GetSubsystem<UStairwellManager>()->Init();
}
