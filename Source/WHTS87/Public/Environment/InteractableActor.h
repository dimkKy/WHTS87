// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

UCLASS(Abstract, NotBlueprintable)
class WHTS87_API AInteractableActor : public AActor
{
	GENERATED_BODY()
public:	
	AInteractableActor();

	UE_NODISCARD virtual bool OnInteract(AActor* caller, bool bIsInstantAction = true) final;
	//split to long and instant interactions?
	UE_NODISCARD virtual bool IsCurrentlyInteractable(AActor* caller) const { return true; };
	//virtual float GetLongInteractionTime() const { return longInteractionTime; };
	virtual float GetLongInteractionTime() const { return 0.f; };

	// getters for UI
	UE_NODISCARD bool GetInstantActionDescription(FText& outInstantActionText) PURE_VIRTUAL(AInteractableActor::GetInstantActionDescription, return false;);
	UE_NODISCARD bool GetLongActionDescription(FText& outLongActionText) PURE_VIRTUAL(AInteractableActor::GetLongActionDescription, return false;);
protected:
	//UE_NODISCARD virtual bool OnInteract_Internal(AActor* Caller, bool bIsInstantAction) PURE_VIRTUAL(AIInteractable::OnInteract, return false;);
	UE_NODISCARD virtual bool OnInstantInteraction(AActor* caller) PURE_VIRTUAL(AIInteractable::OnInstantInteraction, return false;);
	UE_NODISCARD virtual bool OnLongInteraction(AActor * caller) { return false; };

/*	UPROPERTY(EditDefaultsOnly)
		float longInteractionTime;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>&ValidationErrors) override;
#endif*/
};
