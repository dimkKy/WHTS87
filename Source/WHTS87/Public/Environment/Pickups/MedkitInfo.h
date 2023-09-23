// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Environment/Pickups/PickupItemInfoBase.h"
#include "Environment/Pickups/ItemPropertiesBase.h"
#include "MedkitInfo.generated.h"

struct FMedkitProperties : public FItemPropertiesBase
{
	/*FMedkitProperties() :
		bIsTurnedOn{ false } {};
	float healAmount;
	float healDuration;



	//TWeakObjectPtr<USpotLightComponent> lightSource;
	static const FName spotLightSocketName;*/

};

/**
 * 
 */
UCLASS()
class WHTS87_API UMedkitInfo : public UPickupItemInfoBase
{
	GENERATED_BODY()
	
public:
	//UFlashlightInfo();

	//virtual bool ConstructContainer(APickupItemContainer& container) const override;

	//virtual TUniquePtr<FItemPropertiesBase> MakeNonStaticProperties() const override;

	//virtual int32 OnUse(AActor& caller, AActor* target, int32 timesToUse) const override;
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& context) const override;
#endif

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1"))
		float healAmount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0"))
		float healDuration;
	/** Degrees. */
};
