// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Templates/SharedPointer.h"
#include "PickupItemInfoBase.generated.h"

class APickupItemContainer;

struct FItemNonStaticProperties : public TSharedFromThis<FItemNonStaticProperties, ESPMode::NotThreadSafe>
{
public:
	FItemNonStaticProperties() : someValue{ 0.f } {};
	float someValue;
protected:

};
/**
 * 
 */
UCLASS()
class WHTS87_API UPickupItemInfoBase : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPickupItemInfoBase();
	int32 GetXInventorySize() const { return XInventorySize; }
	int32 GetYInventorySize() const { return YInventorySize; }
	int32 GetDefaultSpawnStackSize() const { return defaultSpawnStackSize; }
	int32 GetMaxStackSize() const { return maxStackSize; }
	int32 GetMinUsableQuantity() const { return minUsableQuantity; }
	float GetItemWeight() const { return itemWeightInKG; }
	UTexture2D* GetThumbnail() const { return thumbnail; }
	UStaticMesh* GetBodyMesh() const { return bodyMesh; }
	//references?
	FName GetName() const { return name; }

	FText GetUseActionText() const { return useActionText; }
	FText GetDisplayName() const { return displayName; }
	FText GetDescription() const { return description; }

	virtual bool ConstructContainerMesh(APickupItemContainer* container);

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
	//container is not needed?
	//virtual int32 OnUse(APickupItemContainer* container, AActor* caller, AActor* target, int32 timesToUse) PURE_VIRTUAL(UPickupItemGenericInfo::OnUse, return timesToUse;);
	virtual int32 OnUse(AActor* caller, AActor* target, int32 timesToUse) PURE_VIRTUAL(UPickupItemInfoBase::OnUse, return timesToUse;);
	virtual void OnContainerTick(APickupItemContainer* container, float deltatime) PURE_VIRTUAL(UPickupItemInfoBase::OnContainerTick, return;);;

protected:
	//copies?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1"))
		int32 XInventorySize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1"))
		int32 YInventorySize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1"))
		int32 maxStackSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1"))
		int32 defaultSpawnStackSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1"))
		int32 minUsableQuantity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.1"))
		float itemWeightInKG;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FText useActionText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FText displayName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine = true))
		FText description;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UTexture2D* thumbnail;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMesh* bodyMesh;
};
