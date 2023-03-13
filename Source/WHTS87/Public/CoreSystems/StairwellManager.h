// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "StairwellManager.generated.h"

/**
 * 
 */
UCLASS()
class WHTS87_API UStairwellManager : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UStairwellManager();
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
};
