// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Templates/SharedPointer.h"
#include "WHTS87Utils.h"
#include "PickupItemInfoBase.generated.h"

class APickupItemContainer;

struct FItemPropertiesBase/* : public TSharedFromThis<FItemPropertiesBase, ESPMode::NotThreadSafe>*/
{
	template <WHTS87Utils::ChildOf<FItemPropertiesBase> TProperties, class...Args>
	static TUniquePtr<FItemPropertiesBase> MakeUniquePtr(const Args&...args) {

		return TUniquePtr<FItemPropertiesBase>(
			static_cast<FItemPropertiesBase*>(new TProperties(args...)));
	}

protected:
	FItemPropertiesBase() {};
};

/**
 * 
 */
UCLASS(Abstract, Const, NotPlaceable)
class WHTS87_API UPickupItemInfoBase : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPickupItemInfoBase();

	virtual TUniquePtr<FItemPropertiesBase> MakeNonStaticProperties() const
		PURE_VIRTUAL(UPickupItemInfoBase::MakeNonStaticProperties(), return {nullptr};);

	FIntPoint GetInventorySize() const
		{ return { xSize, ySize }; }
	int32 GetXInventorySize() const 
		{ return xSize; }
	int32 GetYInventorySize() const 
		{ return ySize; }

	int32 GetDefaultStackSize() const 
		{ return defaultSpawnStackSize; }
	int32 GetMaxStackSize() const 
		{ return maxStackSize; }

	int32 GetMinUsableQuantity() const 
		{ return minUsableQuantity; }

	float GetItemWeight() const 
		{ return itemWeightInKG; }
	const UTexture2D* GetThumbnail() const 
		{ return thumbnail; }
	const UStaticMesh* GetBodyMesh() const
		{ return bodyMesh; }
	//references?
	FName GetName() const 
		{ return name; }

	FText GetUseActionText() const 
		{ return useActionText; }
	virtual FText GetDisplayName() const 
		{ return displayName; }
	virtual FText GetDescription() const 
		{ return description; }

	virtual bool ConstructContainer(APickupItemContainer& container) const;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
	//container is not needed?
	//virtual int32 OnUse(APickupItemContainer* container, AActor* caller, AActor* target, int32 timesToUse) PURE_VIRTUAL(UPickupItemGenericInfo::OnUse, return timesToUse;);
	virtual int32 OnUse(AActor& caller, AActor* target, int32 timesToUse) const 
		PURE_VIRTUAL(UPickupItemInfoBase::OnUse, return timesToUse;);

	virtual void OnContainerTick(APickupItemContainer& container, float deltatime) const 
		PURE_VIRTUAL(UPickupItemInfoBase::OnContainerTick, return;);

protected:
	//copies?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName name;
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "4"))
		int32 xSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "4"))
		int32 ySize;

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
