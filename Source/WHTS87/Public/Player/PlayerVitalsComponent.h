// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Components/VitalsComponent.h"
#include "PlayerVitalsComponent.generated.h"

/**
 * 
 */
UCLASS()
class WHTS87_API UPlayerVitalsComponent : public UVitalsComponent
{
	GENERATED_BODY()
public:
	UPlayerVitalsComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ApplyMentalDamage(float amount, float duration);
	float GetSanityPercent() const { return currentSanityPercent; };
protected:
	virtual void BeginPlay() override;
	//replace wiyh delegate?
	void OnSanityEnded();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float sanityPercentRegenPerSecond;
	float currentSanityPercent;
	TDoubleLinkedList<FPointsModifier> sanityModifiersQueue;
};
