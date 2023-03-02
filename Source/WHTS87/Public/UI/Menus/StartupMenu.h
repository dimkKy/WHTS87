// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartupMenu.generated.h"

class UButton;
/**
 * 
 */
UCLASS(Abstract)
class WHTS87_API UStartupMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* continueButton;
//protected:
};
