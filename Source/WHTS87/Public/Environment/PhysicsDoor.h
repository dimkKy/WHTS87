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
	bool SetMeshes(UStaticMesh& newDoorFrameMesh, UStaticMesh& newDoorMesh);

protected:
	virtual void BeginPlay() override;

	UE_NODISCARD virtual bool OnInstantInteraction(AActor* caller) override;
	UE_NODISCARD virtual bool OnLongInteraction(AActor* caller) override;

	void InstantClose();

	UFUNCTION()
		void OnDoorPutToSleep(UPrimitiveComponent* InComp, FName InBoneName);
	UFUNCTION()
		void OnDoorWakeUp(UPrimitiveComponent* InComp, FName InBoneName);

	void CloseDoor();
	void OpenDoor();
	void PushDoor();
	//void Open(bool bInstant = false);
	// 
	//uproperty?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* doorFrame;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* door;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UPhysicsConstraintComponent* constraint;

#if WITH_EDITORONLY_DATA
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

	constexpr static float maxOpenHalfAngle{ 46.f };
	constexpr static float physicsConstraintStrength{ 5.f };
	constexpr static float physicsConstraintDamping{ 10.f };
	constexpr static float doorPushingImpulse{ 15.f };
	constexpr static float closePosTolerance{ 1.f };
	const static FRotator openConstraitPosition;
	const static FRotator closeConstraitPosition;
};
