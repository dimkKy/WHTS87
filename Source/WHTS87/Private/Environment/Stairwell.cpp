// by Dmitry Kolontay


#include "Environment/Stairwell.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "WHTS87Utils.h"

AStairwell::AStairwell() : 
	mainFloor{ CreateDefaultSubobject<UStaticMeshComponent>("mainFloor") },
	exitComponent{ mainFloor }
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

bool AStairwell::GenerateEntranceLadder(UStaticMesh& mesh)
{
	verify(RootComponent == mainFloor);
	auto* newJoint{ CastChecked<UStaticMeshComponent>(AddComponentByClass(
		UStaticMeshComponent::StaticClass(), true, RootComponent->GetRelativeTransform(), false)) };
	newJoint->SetMobility(EComponentMobility::Stationary);
	newJoint->SetGenerateOverlapEvents(false);
	newJoint->SetCollisionObjectType(ECC_WorldStatic);
	newJoint->SetStaticMesh(&mesh);
	SetRootComponent(newJoint);
	if (mesh.Sockets.Num() >= 1 && mainFloor->AttachToComponent(
		newJoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale, mesh.Sockets[0]->SocketName)) {
		//TODO?
		//jointComponents.Add(newJoint);
		return true;
	}
	else {
		return false;
	}
	return false;
}

FTransform AStairwell::GetNextSWSpawnPoint() const
{
	verify(exitComponent && exitComponent->DoesSocketExist(WHTS87Utils::NamingRules::toNextSWSocket));
	return exitComponent->GetSocketTransform(WHTS87Utils::NamingRules::toNextSWSocket);
}

void AStairwell::SetExitComponent(UStaticMeshComponent& comp)
{
	check(comp.DoesSocketExist(WHTS87Utils::NamingRules::toNextSWSocket));
	exitComponent = &comp;
}

void AStairwell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStairwell::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

UStaticMeshComponent* AStairwell::AddJoint(UStaticMesh& mesh, const FName& socketName, USceneComponent& parent)
{
	auto* newJoint{ CastChecked<UStaticMeshComponent>(
		AddComponentByClass(UStaticMeshComponent::StaticClass(), true, {}, false)) };

	newJoint->SetMobility(EComponentMobility::Stationary);
	newJoint->SetGenerateOverlapEvents(false);
	newJoint->SetCollisionObjectType(ECC_WorldStatic);
	newJoint->SetStaticMesh(&mesh);
	newJoint->AttachToComponent(&parent, 
		FAttachmentTransformRules::SnapToTargetNotIncludingScale, socketName);

	return newJoint;
}

