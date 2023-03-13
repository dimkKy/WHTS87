// by Dmitry Kolontay


#include "UI/Gametime/InteractionHelper.h"
#include "Environment/InteractableActor.h"

UInteractionHelper::UInteractionHelper(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), 
	timeSinceButtonPressed{ 0.f }, actorCurrentlyInteractingWith{ nullptr }
{
}

void UInteractionHelper::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);
}

void UInteractionHelper::ProceedWithLongInteraction(AInteractableActor* interactable)
{
	if (IsValid(interactable)) {

	}
	else {
		//interrupt
	}
}

void UInteractionHelper::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}
