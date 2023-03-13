// by Dmitry Kolontay


#include "CoreSystems/StairwellManager.h"

UStairwellManager::UStairwellManager()
{
}

bool UStairwellManager::ShouldCreateSubsystem(UObject* Outer) const
{
	if (this->GetClass()->IsInBlueprint())
		return Super::ShouldCreateSubsystem(Outer);
	else
		return false;
}
