// by Dmitry Kolontay


#include "Environment/PhysicsDoor.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "WHTS87Utils.h"

const FRotator APhysicsDoor::openConstraitPosition = { 0.f, maxOpenHalfAngle * 2.f, 0.f };
const FRotator APhysicsDoor::closeConstraitPosition = { 0.f, maxOpenHalfAngle * -2.f, 0.f };

APhysicsDoor::APhysicsDoor() : doorFrame{ CreateDefaultSubobject<UStaticMeshComponent>("doorFrame") },
	door{ CreateDefaultSubobject<UStaticMeshComponent>("door") },
	constraint{ CreateDefaultSubobject<UPhysicsConstraintComponent>("constraint") },
	lockState { EDoorlockState::ClosedLock }, bUnknownState{ true }, bIsClosed{ true }, 
	bIsPendingClose{ false }
{
	SetActorTickInterval(1.f);

	doorFrame->SetMobility(EComponentMobility::Stationary);
	doorFrame->SetCollisionObjectType(ECC_WorldStatic);
	doorFrame->SetGenerateOverlapEvents(false);
	SetRootComponent(doorFrame);

	door->SetCollisionObjectType(ECC_WorldDynamic);
	door->BodyInstance.bEnableGravity = false;
	door->BodyInstance.bGenerateWakeEvents = true;
	door->BodyInstance.StabilizationThresholdMultiplier = 0.2f;
	
	door->SetAngularDamping(physicsConstraintDamping);
	door->SetGenerateOverlapEvents(false);

	constraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, maxOpenHalfAngle);
	constraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	constraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.f);
	constraint->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
	//component 2 is parent one
	constraint->ComponentName1.ComponentName = doorFrame->GetFName();
	constraint->ComponentName2.ComponentName = door->GetFName();
	constraint->SetDisableCollision(true);
	constraint->ConstraintInstance.SetAngularDriveParams(
		door->BodyInstance.StabilizationThresholdMultiplier + 0.1f, physicsConstraintDamping, 0.f);
}

void APhysicsDoor::OnConstruction(const FTransform& Transform)
{
#if WITH_EDITOR
	if (!IsValid(door->GetStaticMesh()))
		door->SetStaticMesh(doorMesh);
	if (!IsValid(doorFrame->GetStaticMesh()))
		doorFrame->SetStaticMesh(doorFrameMesh);
#endif

	constraint->AttachToComponent(doorFrame, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "DoorSocket");
	door->SetRelativeRotation({ 0.f, maxOpenHalfAngle, 0.f });
	door->AttachToComponent(constraint, FAttachmentTransformRules::KeepRelativeTransform);
	Super::OnConstruction(Transform);
}

void APhysicsDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//TODO
}

bool APhysicsDoor::SetMeshes(UStaticMesh& newDoorFrameMesh, UStaticMesh& newDoorMesh)
{
	check(newDoorFrameMesh.FindSocket(WHTS87Utils::GetNumberedName(WHTS87Utils::NamingRules::doorSocket)) != NULL);
	
	if (!doorFrame->GetStaticMesh() || !door->GetStaticMesh()) {
		return doorFrame->SetStaticMesh(&newDoorFrameMesh) && 
			door->SetStaticMesh(&newDoorMesh);
	}
	else {
		return false;
	}
}

void APhysicsDoor::BeginPlay()
{
	door->OnComponentSleep.AddDynamic(this, &APhysicsDoor::OnDoorPutToSleep);
	constraint->SetAngularOrientationTarget(closeConstraitPosition);
	door->SetSimulatePhysics(false);
	door->SetWorldRotation(doorFrame->GetComponentRotation());
	bIsClosed = true;
}

UE_NODISCARD bool APhysicsDoor::OnInstantInteraction(AActor* Caller)
{
	//TODO
	if (bIsClosed) {
		OpenDoor();
	}	
	else {
		bIsPendingClose = !bIsPendingClose;
		PushDoor();
	}
	return true;
}

UE_NODISCARD bool APhysicsDoor::OnLongInteraction(AActor* Caller)
{
	return false;
}

void APhysicsDoor::InstantClose()
{
	constraint->SetAngularOrientationTarget(closeConstraitPosition);
	door->SetSimulatePhysics(false);
	door->SetWorldRotation(doorFrame->GetComponentRotation());
	bIsClosed = true;
}

void APhysicsDoor::OnDoorPutToSleep(UPrimitiveComponent* InComp, FName InBoneName)
{
	check(InComp == (UPrimitiveComponent*)door);

	if (!bIsPendingClose) {
		return;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("OnDoorPutToSleep fired")));
	if (door->GetComponentRotation().Equals(doorFrame->GetComponentRotation(), closePosTolerance)) {
		bIsClosed = true;
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Closed")));
		InComp->SetSimulatePhysics(false);
		door->SetWorldRotation(doorFrame->GetComponentRotation());
	}
}

void APhysicsDoor::OnDoorWakeUp(UPrimitiveComponent* InComp, FName InBoneName)
{
	check(InComp == (UPrimitiveComponent*)door);
}

void APhysicsDoor::CloseDoor()
{
	if (!bIsClosed) {
		bIsPendingClose = true;
		PushDoor();
	}
}

void APhysicsDoor::OpenDoor()
{
	if (bIsClosed) {
		bIsClosed = false;
		bIsPendingClose = false;
		PushDoor();
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Opened")));
	}
	/*else {
		//?
	}*/
}

void APhysicsDoor::PushDoor()
{
	if (!bIsClosed) {
		if (!door->IsSimulatingPhysics())
			door->SetSimulatePhysics(true);
		if (bIsPendingClose) {
			door->AddAngularImpulseInRadians({ 0.f, 0.f, doorPushingImpulse * -1.f }, NAME_None, true);
			constraint->SetAngularOrientationTarget(closeConstraitPosition);
		}
		else {
			door->AddAngularImpulseInRadians({ 0.f, 0.f, doorPushingImpulse }, NAME_None, true);
			constraint->SetAngularOrientationTarget(openConstraitPosition);
		}
	}
}

#if WITH_EDITOR
void APhysicsDoor::TestInitialize()
{
	RegisterAllComponents();
	door->SetStaticMesh(doorMesh);
	doorFrame->SetStaticMesh(doorFrameMesh);
}
#endif