// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WHTS87GameModeBase.generated.h"

class AWHTS87PlayerController;
/**
 * 
 */
UCLASS()
class WHTS87_API AWHTS87GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AWHTS87GameModeBase();
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	//change name
	UFUNCTION()
		void OnGameplayStart();

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& context) const override;
#endif
protected:
	//UFUNCTION()
		//void OnGameSpecificSettingsLoad(const FString& slotName, const int32 userIndex, USaveGame* loadedData);
	//UFUNCTION()
		//void OnStartupMenuClose();

	//UFUNCTION()
		//void OnSaveGameSpecificSettings(const FString& SlotName, const int32 UserIndex, bool bSuccess);
	virtual void StartPlay() override;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		//TSubclassOf<UStartupMenu> startupMenuClass;

	TWeakObjectPtr<AWHTS87PlayerController> localController;
	//UStartupMenu* startupMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UWorld> mainMenuLevel;
};
