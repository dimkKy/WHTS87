// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VitalsComponent.generated.h"

//DECLARE_MULTICAST_DELEGATE(FOnHealthEnded);

struct FPointsModifier
{
	//GENERATED_BODY()
public:
	//FPointsModifier() : amountLeft{ 0.f }, durationLeft{ 0.f } {};
	float amountLeft;
	float durationLeft;
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WHTS87_API UVitalsComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	UVitalsComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ApplyPhysicalDamage(float amount, float duration);
	float GetMaxHP() const { return maxHPBase; };
	float GetCurrentHP() const { return currentHP; };

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
protected:
	virtual void BeginPlay() override;
	float ModifyQueue(TDoubleLinkedList<FPointsModifier>& queue, float deltaTime);
	//replace wiyh delegate?
	void OnHPEnded();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1"))
		float maxHPBase;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1"))
		float HPRegenPerSecond;
	float currentHP;
	TDoubleLinkedList<FPointsModifier> healthModifiersQueue;

};
