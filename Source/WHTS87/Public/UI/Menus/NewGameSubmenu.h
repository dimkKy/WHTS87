// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewGameSubmenu.generated.h"

class UButton;
/**
 * 
 */
UCLASS(Abstract)
class WHTS87_API UNewGameSubmenu : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* startButton;
protected:
	UFUNCTION()
		void OnNewGameStartButtonPressed();
};
