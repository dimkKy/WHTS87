// by Dmitry Kolontay


#include "CoreSystems/WHTS87GameInstance.h"
#include "CoreSystems/StairwellManager.h"
#include "MoviePlayer.h"

void UWHTS87GameInstance::Init()
{
	Super::Init();
	//FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UMyGameInstance::BeginLoadingScreen);
	//FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UMyGameInstance::EndLoadingScreen);
}

void UWHTS87GameInstance::OnStart()
{
	Super::OnStart();
}

void UWHTS87GameInstance::OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld)
{
	Super::OnWorldChanged(OldWorld, NewWorld);
	if (!IsRunningDedicatedServer())
	{
		FLoadingScreenAttributes loadingScreen;
		loadingScreen.bAutoCompleteWhenLoadingCompletes = false;
		loadingScreen.bWaitForManualStop = true;
		loadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();

		GetMoviePlayer()->SetupLoadingScreen(loadingScreen);
	}
}

#if WITH_EDITOR
EDataValidationResult UWHTS87GameInstance::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult superResult{ Super::IsDataValid(ValidationErrors) };
	if (superResult != EDataValidationResult::Invalid) {
		if (!IsValid(StaiwellManagerClass))
			ValidationErrors.Add(FText::FromString("Invalid StaiwellManagerClass"));
		if (ValidationErrors.Num() > 0) {
			superResult = EDataValidationResult::Invalid;
		}
	}
	return superResult;
}
#endif
