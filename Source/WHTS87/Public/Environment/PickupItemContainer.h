// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Environment/InteractableActor.h"
#include "Environment/Pickups/ItemPropertiesBase.h"
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
UCLASS(Blueprintable)
class WHTS87_API APickupItemContainer : public AInteractableActor
{
	GENERATED_BODY()
public:
	APickupItemContainer();
	APickupItemContainer(FVTableHelper& Helper);
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;

	virtual bool IsCurrentlyInteractable(const AActor* caller) const override;

	UStaticMeshComponent* GetBody() const;

	const UPickupItemInfoBase* GetItemInfo() const;

	UInventoryComponent* GetOwnerInventory() const;

	int32 GetItemsCount() const;

	int32 GetLackingItemsCount() const;

	bool InitializeWithItem(const UPickupItemInfoBase& newItemInfo, 
		bool bOverrideDefaultSpawnParameters = false, int32 quantityToSpawn = 1);

	//hide or event delete
	void SetContainerState(EContainerState newState);
	bool SetOwnerInventory(UInventoryComponent& newOwnerInventory);
	// returns itemCount - oldItemsCount
	int32 ChangeItemsCount(int32 term);
	// returns itemCount - oldItemsCount
	int32 SetItemsCount(int32 newQuantity);
	/// Overriding will cause the item logic not to be called, caller should manage it himself. delete
	/// <returns>Number of used items</returns>
	int32 UseItemInContainer(AActor& caller, AActor* target, int32 quantityToUse = 0, bool bOverrideMinQuantity = false);

	//TODO
	UE_NODISCARD bool RemoveFromInventory(bool bEject);

	TUniquePtr<FItemPropertiesBase> properties;
	//TSharedPtr<FItemPropertiesBase> properties;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UE_NODISCARD virtual bool OnInstantInteraction(AActor* caller) override;

	UFUNCTION()
		void OnContainerPutToSleep(UPrimitiveComponent* InComp, FName InBoneName);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* body;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		const UPickupItemInfoBase* itemInfo;
	TWeakObjectPtr<UInventoryComponent> ownerInventory;

	UPROPERTY(EditInstanceOnly, meta = (ClampMin = "1"))
		int32 itemCount;	
	//TODO
	void OnItemsUsedUp();

	bool bAcceptInteraction;
	bool bOverrideTick;

	static constexpr float throwForceMultiplier = 400.f;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual EDataValidationResult IsDataValid(FDataValidationContext& context) const override;
#endif
};
