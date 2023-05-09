// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "WHTS87UserSettings.generated.h"

/**
 * 
 */
UCLASS()
class WHTS87_API UWHTS87UserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	UWHTS87UserSettings(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(config)
		bool privateMode;
	UPROPERTY(config)
		bool showStartupMenu;
};
