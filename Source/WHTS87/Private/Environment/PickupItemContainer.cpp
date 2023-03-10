// by Dmitry Kolontay


#include "Environment/PickupItemContainer.h"
#include "Environment/Pickups/PickupItemInfoBase.h"
#include "Components/InventoryComponent.h"

APickupItemContainer::APickupItemContainer() : body{ CreateDefaultSubobject<UStaticMeshComponent>("body") }, itemInfo{ nullptr }, 
	ownerInventory{ nullptr }, itemCount { 0 }, bAcceptInteraction{ true }, bOverrideTick{ false }
{
	body->SetCollisionObjectType(ECC_PhysicsBody);
	body->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	body->BodyInstance.bGenerateWakeEvents = true;
	body->SetGenerateOverlapEvents(false);
	body->SetSimulatePhysics(true);
	SetRootComponent(body);
}

void APickupItemContainer::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	//?
	if (IsValid(itemInfo)) {
		InitializeWithItem(itemInfo);
	}
}

void APickupItemContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (itemInfo)
		itemInfo->OnContainerTick(this, DeltaTime);
}

UPickupItemInfoBase* APickupItemContainer::GetItemInfo()
{
	return itemInfo;
}

int32 APickupItemContainer::GetLackingItemsCount() const
{
	return itemInfo->GetMaxStackSize() - itemCount;
}

bool APickupItemContainer::InitializeWithItem(UPickupItemInfoBase* newItemInfo, bool bOverrideDefaultSpawnParameters, int32 quantityToSpawn)
{
	//INITIALIZATION WITH THE SAME ELEMENT IS POSSIBLE - FIX REQUIRED
	//https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/ProgrammingWithCPP/Assertions/
	//validation?
	//cannot reuse
	if (IsValid(newItemInfo) && itemInfo != nullptr) {
		if (newItemInfo->GetMaxStackSize() > 1 || quantityToSpawn > 1) {
			if (bOverrideDefaultSpawnParameters) {
				if (quantityToSpawn > newItemInfo->GetMaxStackSize())
					itemCount = newItemInfo->GetMaxStackSize();
				else
					itemCount = quantityToSpawn;
			}
			else {
				itemCount = newItemInfo->GetDefaultSpawnStackSize();
			}
		}
		else {
			itemCount = 1;
		}
		if (newItemInfo->ConstructContainerMesh(this)) {
			itemInfo = newItemInfo;
			return true;
		}
		else
			return false;

	}
	else {
		return false;
	}
}

void APickupItemContainer::SetContainerState(EContainerState newState)
{
	switch (newState)
	{
	case EContainerState::Hidden:
		SetActorTickEnabled(false);
		body->SetSimulatePhysics(false);
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		bAcceptInteraction = false;
		break;
	case EContainerState::VisibleLocked:
		if (!bOverrideTick) {
			SetActorTickEnabled(true);
		}
		body->SetSimulatePhysics(false);
		SetActorHiddenInGame(false);
		body->SetCastShadow(true);
		SetActorEnableCollision(false);
		bAcceptInteraction = false;
		break;
	case EContainerState::HeldVisible:
		if (!bOverrideTick) {
			SetActorTickEnabled(true);
		}
		body->SetSimulatePhysics(false);
		SetActorHiddenInGame(false);
		SetActorEnableCollision(false);
		bAcceptInteraction = false;
		body->SetCastShadow(false);
		break;
	case EContainerState::HeldHidden:
		//todo
		check(false);
		break;
	case EContainerState::VisibleAccessible:
		[[fallthrough]];
	default:
		if (!bOverrideTick) {
			SetActorTickEnabled(true);
		}
		body->SetSimulatePhysics(true);
		SetActorHiddenInGame(false);
		body->SetCastShadow(true);
		SetActorEnableCollision(true);
		bAcceptInteraction = true;
		break;
	}
}

bool APickupItemContainer::SetOwnerInventory(UInventoryComponent* newOwnerInventory)
{
	ownerInventory = TWeakObjectPtr<UInventoryComponent>{ newOwnerInventory };
	return true;
}

int32 APickupItemContainer::ChangeItemsCount(int32 term)
{
	return SetItemsCount(itemCount + term);
}

int32 APickupItemContainer::SetItemsCount(int32 newQuantity)
{
	if (itemInfo) {
		int32 oldItemsCount{ itemCount };
		if (itemInfo->GetMaxStackSize() > newQuantity)
			itemCount = itemInfo->GetMaxStackSize();
		else {
			if (newQuantity > 0) {
				itemCount = newQuantity;
			}
			else {
				check(false);
				OnItemsUsedUp();
				//disposed
			}
		}
		return itemCount - oldItemsCount;
	}
	else {
		return 0;
	}
}

int32 APickupItemContainer::UseItemInContainer(AActor* caller, AActor* target, int32 quantityToUse, bool bOverrideMinQuantity)
{
	if (itemInfo) {
		int32 minUsableQuantity{ itemInfo->GetMinUsableQuantity() };
		if (quantityToUse == 0) {
			if (bOverrideMinQuantity)
				return 0;
			else
				quantityToUse = minUsableQuantity;
		}
		int32 timesUsed{ 0 };
		if (quantityToUse <= itemCount && (bOverrideMinQuantity || quantityToUse >= minUsableQuantity)) {
			if (bOverrideMinQuantity) {
				itemCount -= quantityToUse;
			}
			else {
				//timesUsed += itemInfo->OnUse(this, caller, target, quantityToUse / minUsableQuantity);
				timesUsed += itemInfo->OnUse(caller, target, quantityToUse / minUsableQuantity);
				itemCount -= timesUsed * minUsableQuantity;
			}
		}
		//destroy?
		return timesUsed * minUsableQuantity;
	}
	else
		return 0;
}

void APickupItemContainer::OnRemoveFromInventory(bool bEject)
{

	if (ownerInventory.IsValid() && bEject) {
		if (!bOverrideTick) {
			SetActorTickEnabled(true);
		}
		body->SetSimulatePhysics(true);
		SetActorHiddenInGame(false);
		body->SetCastShadow(true);
		SetActorEnableCollision(true);
		bAcceptInteraction = true;
		AActor* owningActor{ ownerInventory.Get()->GetOwner() };
		SetActorLocationAndRotation(owningActor->GetActorLocation(), owningActor->GetActorRotation(), false, nullptr, ETeleportType::ResetPhysics);
		body->AddImpulse(owningActor->GetVelocity() + owningActor->GetActorForwardVector() * body->GetMass() * 250);
	}
	ownerInventory = TWeakObjectPtr<UInventoryComponent>{ nullptr };
}

void APickupItemContainer::BeginPlay()
{
	Super::BeginPlay();
	body->OnComponentSleep.AddDynamic(this, &APickupItemContainer::OnContainerPutToSleep);
}

UE_NODISCARD bool APickupItemContainer::OnInstantInteraction(AActor* caller)
{
	if (UInventoryComponent* callerInventory{ caller->FindComponentByClass<UInventoryComponent>() }) {
		int32 oldItemCount{ itemCount };
		if (callerInventory->AddContainer(this) == oldItemCount) {
			if (itemCount > 0) {
				SetContainerState(EContainerState::Hidden);
				//in inventory
			}
			else {
				//disposed 
				OnItemsUsedUp();
			}
		}
		SetOwnerInventory(callerInventory);
		return true;
	}
	return false;
}

void APickupItemContainer::OnContainerPutToSleep(UPrimitiveComponent* InComp, FName InBoneName)
{
	check(InComp == (UPrimitiveComponent*)body);
	TSet<AActor*> overlappingActors;
	GetOverlappingActors(overlappingActors);
	if (overlappingActors.Num() == 0 && InComp->IsSimulatingPhysics()) {
		InComp->SetSimulatePhysics(false);
	}
}

void APickupItemContainer::OnItemsUsedUp()
{
}

#if WITH_EDITOR
void APickupItemContainer::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.Property != NULL && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(APickupItemContainer, itemInfo))
		InitializeWithItem(itemInfo);
}
#endif
