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

	FIntPoint GetInventorySize() const;
	int32 GetXInventorySize() const;
	int32 GetYInventorySize() const;

	int32 GetDefaultStackSize() const;
	int32 GetMaxStackSize() const;
	int32 GetMinUsableQuantity() const;

	float GetItemWeight() const;

	UTexture2D* GetThumbnail() const;
	const UStaticMesh* GetBodyMesh() const;

	//references?
	FName GetName() const;

	FText GetUseActionText() const;
	FText GetDisplayName() const;
	FText GetDescription() const;

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
