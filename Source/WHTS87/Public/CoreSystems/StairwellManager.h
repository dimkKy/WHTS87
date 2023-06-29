// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "StairwellManager.generated.h"

class AStairwell;

enum class EStairwellType : uint8
{
	/***/
	RectangularWithTravel,
	/**With "save" room*/
	RectangularSafe,
	/**NOT default default*/
	RectangularDefault,
	/**Default default, should be last as it also used as counter*/
	CanonicalRounded,

	EStairwellType_MAX,
};

/**
 * 
 */
UCLASS(Blueprintable)
class WHTS87_API UStairwellManager : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UStairwellManager();
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	void SetSeed();
	void SetSeed(int32 seed);
	int32 GetSeed() const 
		{ return worldSeed; };

	void Init();
	void OnGameplayStart();
	void AddStairwellsChunk();
	void AddDefaultStairwell(int32 quantity = 1);
	EStairwellType AddStairwell();
	
protected:
	int32 worldSeed;
	TArray<AStairwell*> stairwells;
	TArray<int32> chunkBorders;

	int32 lastSWTypeNumber[static_cast<int32>(EStairwellType::EStairwellType_MAX)];
	void GetAllStairwells(TArray<AStairwell*>& out);
	//currently not applicable to stairwell's mainfloor
	//void ParseMeshComponents(TDoubleLinkedList<UStaticMeshComponent*>& componentsToLook, EStairwellType stairwellType, int32 stairwellSeed, bool bSpawnItems);
	int32 GetStairwellSeed(int32 stairwellNumber);
	EStairwellType GetStairwellType(int32 seed);
	void ConstructStarweillBody(AStairwell& stairwell, EStairwellType type);

	// unfinished mechanism
	FTransform GetNewSWTransform();

	/// <summary>
	/// will NOT add stairwell to stairwells array
	/// </summary>
	/// <param name="newSWTransform"></param>
	/// <param name="stairwellType"></param>
	/// <param name="stairwellSeed"></param>
	/// <returns></returns>
	UE_NODISCARD AStairwell* ConstructStairwell(FTransform transform, EStairwellType type, int32 seed);
	//UE_NODISCARD UStaticMeshComponent* AddMeshComponent(UStaticMesh* meshToAssign, const FName& socketName, USceneComponent* componentAttachTo);
	//bool AddDefaultConnectingFloor(AStairwell* stairwell);

	constexpr static std::pair<int32, int32> 
		SWTypeThresholds[static_cast<int32>(EStairwellType::EStairwellType_MAX)] = {
			{15, 25},
			{15, 25},
			{0, 7},
			{0, 7},
	};
	constexpr static bool checkSWTypeThresholds(SIZE_T i = 0);

	
	constexpr static int32 SWTypeChances
		[static_cast<int32>(EStairwellType::EStairwellType_MAX)] = {
			00700, //7.00%
			01300,
			06000,
			10000,
	};
	constexpr static bool checkSWTypeChances(SIZE_T i = 0);
};
