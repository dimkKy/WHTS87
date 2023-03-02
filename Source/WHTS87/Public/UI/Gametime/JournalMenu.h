// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JournalMenu.generated.h"

/**
 * TODO
 */
UCLASS(Abstract)
class WHTS87_API UJournalMenu : public UUserWidget
{
	GENERATED_BODY()
public:

protected:
	virtual void NativeOnInitialized() override;
};
