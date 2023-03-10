// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Environment/InteractableActor.h"
#include "PickupItemContainer.generated.h"

class UInventoryComponent;
class UPickupItemInfoBase;

enum class EContainerState : uint8
{
	/***/
	Hidden,
	/***/
	VisibleAccessible,
	/***/
	VisibleLocked,
	/***/
	HeldVisible,
	/***/
	HeldHidden,
};
/**
 * 
 */
UCLASS()
class WHTS87_API APickupItemContainer : public AInteractableActor
{
	GENERATED_BODY()
public:
	APickupItemContainer();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;

	virtual bool IsCurrentlyInteractable(AActor* caller) const override { return bAcceptInteraction; };

	UStaticMeshComponent* GetBody() { return body; };
	UPickupItemInfoBase* GetItemInfo();
	UInventoryComponent* GetOwnerInventory() { return ownerInventory.IsValid() ? ownerInventory.Get() : nullptr; };
	int32 GetItemsCount() const { return itemCount; };
	int32 GetLackingItemsCount() const;
	bool InitializeWithItem(UPickupItemInfoBase* newItemInfo, bool bOverrideDefaultSpawnParameters = false, int32 quantityToSpawn = 1);
	//hide or event delete
	void SetContainerState(EContainerState newState);
	bool SetOwnerInventory(UInventoryComponent* newOwnerInventory);
	int32 ChangeItemsCount(int32 term);
	int32 SetItemsCount(int32 newQuantity);
	/// <summary>
	/// Overriding will cause the item logic not to be called, caller should manage it himself.
	/// </summary>
	/// <returns>Number of used items</returns>
	int32 UseItemInContainer(AActor* caller, AActor* target, int32 quantityToUse = 0, bool bOverrideMinQuantity = false);

	//TODO
	void OnRemoveFromInventory(bool bEject);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UE_NODISCARD virtual bool OnInstantInteraction(AActor* caller) override;

	UFUNCTION()
		void OnContainerPutToSleep(UPrimitiveComponent* InComp, FName InBoneName);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* body;
	UPROPERTY(VisibleAnywhere)
		UPickupItemInfoBase* itemInfo;
	TWeakObjectPtr<UInventoryComponent> ownerInventory;

	UPROPERTY(EditInstanceOnly, meta = (ClampMin = "1"))
		int32 itemCount;	
	//TODO
	void OnItemsUsedUp();

	bool bAcceptInteraction;
	bool bOverrideTick;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
