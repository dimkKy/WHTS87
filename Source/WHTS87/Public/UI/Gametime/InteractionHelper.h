// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionHelper.generated.h"

class AInteractableActor;
/**
 * 
 */
UCLASS()
class WHTS87_API UInteractionHelper : public UUserWidget
{
	GENERATED_BODY()
public:
	UInteractionHelper(const FObjectInitializer& ObjectInitializer);
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
	//void ShowInteractionTip(const FText*);
	void ProceedWithLongInteraction(AInteractableActor* interactable);
protected:
	virtual void NativeOnInitialized() override;
	//void OnLongInteractionStarted();
	//void OnLongInteractionInterrupted();

	float timeSinceButtonPressed;
	UPROPERTY()
		AInteractableActor* actorCurrentlyInteractingWith;
};
