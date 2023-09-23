// by Dmitry Kolontay


#include "Environment/InteractableActor.h"

AInteractableActor::AInteractableActor() /*: longInteractionTime{0.f}*/
{
	PrimaryActorTick.bCanEverTick = true;
	SetCanBeDamaged(false);
}


UE_NODISCARD bool AInteractableActor::OnInteract(AActor* caller, bool bIsInstantAction)
{
	if (IsValid(caller) && IsCurrentlyInteractable(caller)) {
		if (bIsInstantAction) {
			return OnInstantInteraction(caller);
		}
		else {
			//if (longInteractionTime > 0.49f)
			return OnLongInteraction(caller);
		}
	}
	return false;
}

/*#if WITH_EDITOR
EDataValidationResult AInteractableActor::IsDataValid(FDataValidationContext& context) const
{
	EDataValidationResult superResult{ Super::IsDataValid(ValidationErrors) };
	if (superResult == EDataValidationResult::Valid) {
		if (longInteractionTime != 0.f && longInteractionTime < 0.49f)
			context.AddError(FText::FromString("Interaction time can not be less than 0.5 seconds. Put zero if you want to disable long interaction"));

		if (context.GetNumErrors() > 0) {
			superResult = EDataValidationResult::Invalid;
		}
	}
	return superResult;
}
#endif*/


