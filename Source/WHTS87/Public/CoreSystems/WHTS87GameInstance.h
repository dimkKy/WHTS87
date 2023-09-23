// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WHTS87GameInstance.generated.h"

class UStairwellManager;
/**
 * 
 */
UCLASS()
class WHTS87_API UWHTS87GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void OnStart() override;
	virtual void OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld) override;
	/*UFUNCTION()
		virtual void BeginLoadingScreen(const FString& mapName);
	UFUNCTION()
		virtual void EndLoadingScreen(UWorld* inLoadedWorld);*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<UStairwellManager> StaiwellManagerClass;
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& context) const override;
#endif
protected:
};
