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
	UStaticMeshComponent* AddJoint(UStaticMesh& mesh, const FName& socketName, USceneComponent& parent);

protected:
	virtual void BeginPlay() override;

	bool GenerateEntranceLadder(UStaticMesh& mesh);

	FTransform GetNextSWSpawnPoint() const;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		UStaticMeshComponent* mainFloor;
	void SetExitComponent(UStaticMeshComponent& comp);

	UStaticMeshComponent* exitComponent;

	friend class UStairwellManager;
};
