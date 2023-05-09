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

void UStairwellManager::SetSeed()
{
	SetSeed(FMath::RandHelper(TNumericLimits<int32>::Max()));
}

void UStairwellManager::SetSeed(int32 newSeed)
{
	seed = newSeed;
}

void UStairwellManager::Init()
{
	UWorld* world = GetWorld();
	check(IsValid(world));
	/*GetAllStairwells(stairwells);
	if (stairwells.Num() != 0) {
		AddDefaultStairwell(2);
	}
	else {
		//do smthng idk
		check(false);
	}*/
}

void UStairwellManager::OnGameplayStart()
{
	//AddStairwellsChunk();
}
