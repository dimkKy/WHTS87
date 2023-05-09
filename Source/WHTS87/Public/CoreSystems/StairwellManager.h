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

	void SetSeed();
	void SetSeed(int32 newSeed);
	int32 GetSeed() const 
		{ return seed; };

	void Init();
	void OnGameplayStart();
	/*void AddStairwellsChunk();
	void AddStairwell(EStairwellType& outAddedStairwellType);
	void AddDefaultStairwell(int32 quantity = 1);*/
protected:
	int32 seed;
	//TArray<AStairwell*> stairwells;
	TArray<int32> lastSWTypeNumber;
	//void GetAllStairwells(TArray<AStairwell*>& outActors);
	//currently not applicable to stairwell's mainfloor
	//void ParseMeshComponents(TDoubleLinkedList<UStaticMeshComponent*>& componentsToLook, EStairwellType stairwellType, int32 stairwellSeed, bool bSpawnItems);
	int32 GetGenerateStairwellSeed(int32 stairwellNumber);


	//bool ShouldChooseStairwellType(EStairwellType stairwellType, int32 stairwellSeed);

	// unfinished mechanism
	//FTransform GetGenerateNewSWTransform();

	/// <summary>
	/// will NOT add stairwell to stairwells array
	/// </summary>
	/// <param name="newSWTransform"></param>
	/// <param name="stairwellType"></param>
	/// <param name="stairwellSeed"></param>
	/// <returns></returns>
	//UE_NODISCARD AStairwell* ConstructStairwell(FTransform newSWTransform, EStairwellType stairwellType, int32 stairwellSeed);
	//UE_NODISCARD UStaticMeshComponent* AddMeshComponent(UStaticMesh* meshToAssign, const FName& socketName, USceneComponent* componentAttachTo);
	//bool AddDefaultConnectingFloor(AStairwell* stairwell);

};
