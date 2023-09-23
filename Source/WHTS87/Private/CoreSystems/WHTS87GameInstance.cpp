// by Dmitry Kolontay


#include "CoreSystems/WHTS87GameInstance.h"
#include "CoreSystems/StairwellManager.h"
#include "MoviePlayer.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

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
EDataValidationResult UWHTS87GameInstance::IsDataValid(FDataValidationContext& context) const
{
	Super::IsDataValid(context);
	//
	if (!IsValid(StaiwellManagerClass)) {
		context.AddError(FText::FromString("Invalid StaiwellManagerClass"));
	}
	//
	if (context.GetNumErrors() > 0) {
		return EDataValidationResult::Invalid;
	}
	else {
		return EDataValidationResult::Valid;
	}
}
#endif
