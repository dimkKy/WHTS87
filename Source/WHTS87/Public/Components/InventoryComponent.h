// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);
class APickupItemContainer;
class UPickupItemInfoBase;

/// <summary>
/// NOT thread-safe
/// </summary>
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WHTS87_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UInventoryComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	int32 AddContainer(APickupItemContainer* containerToAdd, bool bShouldTopUp = true, bool bBroadcast = true);
	bool CheckInsertContainer(APickupItemContainer* containerToCheck, int32 XPosition, int32 YPosition);
	bool TryInsertContainer(APickupItemContainer* containerToTryInserting, int32 XPosition, int32 YPosition, bool bBroadcast = true);

	APickupItemContainer* RemoveContainer(int32 XPosition, int32 YPosition, bool bBroadcast = true, bool bEject = false);
	bool RemoveContainer(APickupItemContainer* containerToRemove, bool bBroadcast = true, bool bEject = false);

	//int32 UseItem(int32 XPosition, int32 YPosition, AActor* target, int32 quantityToUse = 0, bool bBroadcast = true);
	//int32 UseItem(APickupItemContainer* usedContainer, AActor* target, int32 quantityToUse = 0, bool bBroadcast = true, bool bOverrideMinQuantity = false);

	int32 CheckItems(UPickupItemInfoBase* itemToFind, int32 quantityToCheck) const;
	/// <summary>
	/// to do
	/// </summary>
	bool TryUseItems(UPickupItemInfoBase* itemToUse, int32 quantityToUse = 0, bool bBroadcast = true);
	//UPROPERTY(BlueprintAssignable)
	

	
	bool GetContainerCoordinates(APickupItemContainer* pickedUpContainer, int32& outXPosition, int32& outYPosition) const;
	int32 GetInventoryXSize() const noexcept { return XInventorySize; };
	int32 GetInventoryYSize() const noexcept { return YInventorySize; };
	//two arrays, hide?
	TMap<APickupItemContainer*, TPair<int32, int32>> containerMap;
	//UPROPERTY()
	//	TArray<APickupItemContainer*> containers;
	//TArray<APickupItemContainer*, TPair<int32, int32>> containerMap;

	FOnInventoryUpdated onInventoryUpdated;
protected:
	virtual void BeginPlay() override;
	/// <summary>
	/// X size is not supposed to change
	/// </summary>
	bool ChangeInventorySize(int32 newYInventorySize);
	//int32 AddItems(APickupItemContainer* PickedUpContainer, int32 quantity, bool bShouldTopUp = true, bool bBroadcast = true);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1"))
		int32 XInventorySize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1"))
		int32 YInventorySize;
	TArray<TArray<APickupItemContainer*>> cells;
	//TMultiMap<UClass*, TSharedPtr<FItemContainer>> Items;
};
