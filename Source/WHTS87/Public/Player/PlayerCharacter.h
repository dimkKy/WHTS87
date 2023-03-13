// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UPlayerVitalsComponent;
class UPlayerInventoryComponent;
class AInteractableActor;
class UInteractionHelper;

UCLASS()
class WHTS87_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(BlueprintAssignable)
	/// <summary>
	/// camera screen to world. [-1, -1] - left lower corner
	/// </summary>
	/// <param name="normX">will be clamped between -1 and 1</param>
	/// <param name="normY">same</param>
	/// <param name="TransformSpace">relative to actor/camera/springarm/world as follows</param>
	/// <returns></returns>
	FVector DeprojectCameraScreenPlane(float normX, float normY, ERelativeTransformSpace transformSpace = RTS_Component);
	//FVector DeprojectCameraScreenPlane(float angleX, float normY, ERelativeTransformSpace transformSpace = RTS_Component);
	USceneComponent* GetEquipmentAttachParent();
	//todo
	void StopInteracting();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UCameraComponent* firstPersonCamera;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USpringArmComponent* cameraSpringArm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UPlayerInventoryComponent* inventory;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UPlayerVitalsComponent* vitals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float baseTurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float baseLookUpRate;

	void MoveForward(float value);
	void MoveRight(float value);
	void TurnAtRate(float rate);
	void LookUpAtRate(float Rate);

	//get interction target
	void StartInteraction();
	//initiate long interaction
	void OnInteracting();
	//finish instant interaction (if any)
	void AbortInteraction();
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float maxInteractionDistance;
	AActor* actorInFocus;

	UPROPERTY(DuplicateTransient)
		AInteractableActor* actorCurrentlyInteractingWith;
	//negative value means interaction is instant
	float timeSinceLongInteractionStarted;

	//TWeakObjectPtr<UInteractionHelper> interactionHelper;
	//UInteractionHelper* interactionHelper;

	FHitResult interactionShotResult;
};
