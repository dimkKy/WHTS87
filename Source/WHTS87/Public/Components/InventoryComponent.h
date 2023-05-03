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

	UE_NODISCARD int32 CheckInsertContainer(APickupItemContainer& container, const FIntPoint& pos) const;

	UE_NODISCARD int32 TryAddContainer(APickupItemContainer& container, bool bShouldTopUp, bool bBroadcast);
	UE_NODISCARD int32 TryAddContainer(APickupItemContainer& container, const FIntPoint& pos, bool bShouldTopUp, bool bBroadcast);

	UE_NODISCARD APickupItemContainer* RemoveContainer(const FIntPoint& pos, bool bEject, bool bBroadcast);
	UE_NODISCARD bool RemoveContainer(APickupItemContainer& container, bool bEject, bool bBroadcast);

	TMap<APickupItemContainer*, FIntPoint> GetContainersInfo() const;

	//int32 UseItem(int32 XPosition, int32 YPosition, AActor* target, int32 quantityToUse = 0, bool bBroadcast = true);
	//int32 UseItem(APickupItemContainer* usedContainer, AActor* target, int32 quantityToUse = 0, bool bBroadcast = true, bool bOverrideMinQuantity = false);

	UE_NODISCARD int32 CheckItems(UPickupItemInfoBase& item, int32 quantity) const;
	// to do
	UE_NODISCARD bool TryUseItems(UPickupItemInfoBase& itemTo, int32 quantity, bool bBroadcast);
	//UPROPERTY(BlueprintAssignable)
	UE_NODISCARD bool GetContainerCoordinates(APickupItemContainer& container, FIntPoint& outpos) const;

	int32 GetXSize() const noexcept 
		{ return xSize; };
	int32 GetYSize() const noexcept 
		{ return ySize; };

	FOnInventoryUpdated onInventoryUpdated;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
protected:
	virtual void BeginPlay() override;

	UE_NODISCARD int32 InsertNewContainer(APickupItemContainer& container, const FIntPoint& pos);
	UE_NODISCARD int32 MoveContainer(APickupItemContainer& container, const FIntPoint& pos);
	UE_NODISCARD int32 TryTopUpContainer(APickupItemContainer& target, APickupItemContainer& source, int32 quantity, bool bBroadcast);

	bool CheckFreeCells(const FIntPoint& pos, const FIntPoint& size) const;

	// X size is not supposed to change
	bool ChangeInventorySize(int32 newYsize);
	//int32 AddItems(APickupItemContainer* PickedUpContainer, int32 quantity, bool bShouldTopUp = true, bool bBroadcast = true);

	TMap<APickupItemContainer*, int32> containerMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1"))
		int32 xSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "2"))
		int32 ySize;
	
	UPROPERTY(VisibleAnywhere, DuplicateTransient)
	TArray<APickupItemContainer*> cells;
	//TMultiMap<UClass*, TSharedPtr<FItemContainer>> Items;

	static constexpr int32 minYsize = 4;
};
