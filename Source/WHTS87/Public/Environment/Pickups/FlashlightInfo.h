// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Environment/Pickups/PickupItemInfoBase.h"
#include "FlashlightInfo.generated.h"

//class USpotLightComponent;

struct FFlashlightProperties : public FItemPropertiesBase
{
	FFlashlightProperties() :
		bIsTurnedOn{ false } {};
	bool bIsTurnedOn;


	//TWeakObjectPtr<USpotLightComponent> lightSource;
	static const FName spotLightSocketName;

};

/**
 * 
 */
UCLASS(Blueprintable)
class WHTS87_API UFlashlightInfo : public UPickupItemInfoBase
{
	GENERATED_BODY()

public:
	//UFlashlightInfo();

	virtual bool ConstructContainer(APickupItemContainer& container) const override;

	virtual TUniquePtr<FItemPropertiesBase> MakeNonStaticProperties() const override;

	virtual int32 OnUse(AActor& caller, AActor* target, int32 timesToUse) const override;
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.01"))
		float dischargeRate;

	/** Degrees. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Light, meta = (UIMin = "1.0", UIMax = "80.0"))
		float innerConeAngle;
	/** Degrees. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Light, meta = (UIMin = "1.0", UIMax = "80.0"))
		float outerConeAngle;
	//TODO: add other light parameters
};
