// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Stairwell.generated.h"

UCLASS()
class WHTS87_API AStairwell : public AActor
{
	GENERATED_BODY()
	
public:
	AStairwell();
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	//virtual void FinishSpawning(const FTransform& Transform, bool bIsDefaultTransform = false, const FComponentInstanceDataCache* InstanceDataCache = nullptr);
	UStaticMeshComponent* AddJoint(UStaticMesh* meshToAssign, const FName& socketName, USceneComponent* componentAttachTo);

protected:
	virtual void BeginPlay() override;
	//bool GenerateEntranceLadder(UStaticMesh* meshToAssign);
	//UStaticMeshComponent* TryGetNextStairwellSpawnPoint(bool bTryFindNewAnyway, FName* socketNameToSearch = nullptr);
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		UStaticMeshComponent* mainFloor;

	//friend class UStairwellManager;
};
