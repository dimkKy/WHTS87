// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Environment/InteractableActor.h"
#include "Components/TimelineComponent.h"
#include "InteractableSwitcherBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, NotBlueprintable)
class WHTS87_API AInteractableSwitcherBase : public AInteractableActor
{
	GENERATED_BODY()
public:
	AInteractableSwitcherBase();
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual bool IsCurrentlyInteractable(const AActor* caller) const override;

	virtual TMulticastDelegate<void(bool)>* GetOnInteractedDelegate() 
		PURE_VIRTUAL(AInteractableSwitcherBase::GetOnInteractedDelegate, return nullptr; );
protected:
	virtual void BeginPlay() override;
	UE_NODISCARD virtual bool OnInstantInteraction(AActor* caller) override;
	UE_NODISCARD virtual bool OnLongInteraction(AActor* caller) override;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* switcherFrame;
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* switcherBody;
	UPROPERTY(EditDefaultsOnly)
		UTimelineComponent* animationTimeline;
	
	float timeSincePressed;

	//should be normalized with 0.0
	UPROPERTY(EditDefaultsOnly)
		UCurveFloat* animationCurve;
	FOnTimelineEventStatic onTimelineFinished;

	UFUNCTION()
		virtual void OnTimelineFinished() 
			PURE_VIRTUAL(AInteractableSwitcherBase::OnTimelineFinished, return; );

	FOnTimelineFloat onTimeLineUpdated;

	UFUNCTION()
		virtual void UpdateBodyPosition(float normalizedPosition) 
			PURE_VIRTUAL(AInteractableSwitcherBase::UpdateBodyPosition, return; );
};
