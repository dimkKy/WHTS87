// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Environment/InteractableActor.h"
#include "PhysicsDoor.generated.h"

UENUM(BlueprintType)
enum class EDoorlockState : uint8
{
	/**Always can be opened by everyone*/
	NoLock,
	/**Always can be opened by everyone, can be repaired*/
	BrokenLock,
	/**Can be closed with key preventing AI from opening*/
	OpenLock,
	/**Can be opened or picked resulting in BrokenLock*/
	ClosedLock,
};
class UPhysicsConstraintComponent;
/**
 * 
 */
UCLASS(Blueprintable)
class WHTS87_API APhysicsDoor : public AInteractableActor
{
	GENERATED_BODY()
public:
	APhysicsDoor();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;
	bool SetMeshes(UStaticMesh* newDoorFrameMesh, UStaticMesh* newDoorMesh);

protected:
	virtual void BeginPlay() override;
	UE_NODISCARD virtual bool OnInstantInteraction(AActor* Caller) override;
	UE_NODISCARD virtual bool OnLongInteraction(AActor* Caller) override;
	void InstantClose();
	UFUNCTION()
		void OnDoorPutToSleep(UPrimitiveComponent* InComp, FName InBoneName);
	UFUNCTION()
		void OnDoorWakeUp(UPrimitiveComponent* InComp, FName InBoneName);

	void CloseDoor();
	void OpenDoor();
	void PushDoor();
	//void Open(bool bInstant = false);
	//uproperty?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* doorFrame;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* door;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UPhysicsConstraintComponent* physicsConstraint;

#if WITH_EDITOR
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMesh* doorFrameMesh = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMesh* doorMesh = nullptr;
	UFUNCTION(CallInEditor, BlueprintCallable)
		void TestInitialize();
#endif

	EDoorlockState lockState;
	bool bUnknownState;
	bool bIsClosed;
	bool bIsPendingClose;
	float maxOpenHalfAngle;
	float physicsConstraintStrength;
	float physicsConstraintDamping;
	float doorPushingImpulse;
	FRotator openConstraitPosition;
	FRotator closeConstraitPosition;
};
