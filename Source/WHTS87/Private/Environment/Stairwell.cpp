// by Dmitry Kolontay


#include "Environment/Stairwell.h"
#include "Components/StaticMeshComponent.h"

AStairwell::AStairwell() : mainFloor{ CreateDefaultSubobject<UStaticMeshComponent>("mainFloor") }
{
	PrimaryActorTick.bCanEverTick = true;
	mainFloor->SetMobility(EComponentMobility::Stationary);
	mainFloor->SetGenerateOverlapEvents(false);
	mainFloor->SetCollisionObjectType(ECC_WorldStatic);
	SetRootComponent(mainFloor);
}

void AStairwell::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStairwell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStairwell::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

UStaticMeshComponent* AStairwell::AddJoint(UStaticMesh* meshToAssign, const FName& socketName, USceneComponent* componentAttachTo)
{
	if (IsValid(meshToAssign)) {
		UStaticMeshComponent* newJoint{ CastChecked<UStaticMeshComponent>(AddComponentByClass(UStaticMeshComponent::StaticClass(), true, FTransform(), false)) };
		newJoint->SetMobility(EComponentMobility::Stationary);
		newJoint->SetGenerateOverlapEvents(false);
		newJoint->SetCollisionObjectType(ECC_WorldStatic);
		newJoint->SetStaticMesh(meshToAssign);

		//should not return false for any return clause except one on line 2131 (last one) in SceneComponent.cpp
		check(newJoint->AttachToComponent(componentAttachTo, FAttachmentTransformRules::SnapToTargetNotIncludingScale, socketName));
		return newJoint;
	}
	else {
		return nullptr;
	}	
}

