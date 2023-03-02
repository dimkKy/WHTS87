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

	UE_NODISCARD virtual bool OnInteract(AActor* Caller, bool bIsInstantAction = true) final;
	virtual bool IsCurrentlyInteractable() const { return true; };
	//virtual float GetLongInteractionTime() const { return longInteractionTime; };
	virtual float GetLongInteractionTime() const { return 0.f; };

	/// <summary>
	/// getter for UI
	/// </summary>
	/// <param name="outInstantActionText">untouched if 0 or 2 returned</param>
	/// <param name="outLongActionText">untouched if 0 or 1 returned</param>
	/// <returns>0 - no description (not expected); 1 - instant only; 2 - long only; 3 - both</returns>
	virtual int32 GetInteractionActionDescription(FText& outInstantActionText, FText& outLongActionText) PURE_VIRTUAL(AInteractableActor::GetInteractionActionDescription, return 0;);
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
