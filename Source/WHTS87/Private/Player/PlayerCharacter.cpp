// by Dmitry Kolontay


#include "Player/PlayerCharacter.h"
#include "Player/PlayerInventoryComponent.h"
#include "Player/PlayerVitalsComponent.h"
#include "Player/WHTS87PlayerController.h"
#include "Environment/InteractableActor.h"
#include "UI/HUDManager.h"
#include "UI/Gametime/InteractionHelper.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerCharacter::APlayerCharacter() : firstPersonCamera{ CreateDefaultSubobject<UCameraComponent>("firstPersonCamera") },
	cameraSpringArm{ CreateDefaultSubobject<USpringArmComponent>("cameraSpringArm") },
	inventory{ CreateDefaultSubobject<UPlayerInventoryComponent>("inventory") },
	vitals{ CreateDefaultSubobject<UPlayerVitalsComponent>("vitals") },
	baseTurnRate{ 45.f }, baseLookUpRate{ 45.f }, maxInteractionDistance{ 450.f },
	actorInFocus{ nullptr }, actorCurrentlyInteractingWith{ nullptr }
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickInterval(0.25f);
	GetCapsuleComponent()->InitCapsuleSize(34.f, 87.0f);

	firstPersonCamera->SetupAttachment(GetCapsuleComponent());
	firstPersonCamera->SetRelativeLocation(FVector(-15.f, 0.f, 70.f));
	firstPersonCamera->bUsePawnControlRotation = true;

	cameraSpringArm->SetupAttachment(firstPersonCamera);
	cameraSpringArm->TargetArmLength = 0.f;
	cameraSpringArm->bDoCollisionTest = false;
	cameraSpringArm->bUsePawnControlRotation = false;
	cameraSpringArm->bEnableCameraRotationLag = true;
	cameraSpringArm->CameraRotationLagSpeed = 50.f;
}

FVector APlayerCharacter::DeprojectCameraScreenPlane(float normX, float normY, ERelativeTransformSpace transformSpace)
{
	normX = FMath::Clamp(normX, -1.f, 1.f);
	normY = FMath::Clamp(normY, -1.f, 1.f);
	float nearClipPlaneDist{ firstPersonCamera->OrthoNearClipPlane };
	if (FMath::IsNearlyZero(nearClipPlaneDist) || nearClipPlaneDist < 0.f) {
		if (IsValid(GEngine))
			nearClipPlaneDist = GEngine->NearClipPlane;
		else
			nearClipPlaneDist = 10.f;
	}
	FVector2D halvedPlaneSize{ 0.f, nearClipPlaneDist * FMath::Tan(firstPersonCamera->FieldOfView * 0.5f) };
	halvedPlaneSize.X = halvedPlaneSize.Y * firstPersonCamera->AspectRatio;
	/*FVector2D halvedPlaneSize;
	if (auto playerController = Cast<APlayerController>(Controller)) {
		if (auto localPlayer = Cast<ULocalPlayer>(playerController->Player)) {
			switch (localPlayer->AspectRatioAxisConstraint.GetValue())
			{
			case AspectRatio_MaintainXFOV:
				halvedPlaneSize.X = nearClipPlaneDist * FMath::Tan(firstPersonCameraComp->FieldOfView * 0.5f);
				halvedPlaneSize.Y = halvedPlaneSize.X / firstPersonCameraComp->AspectRatio;
				break;
			case AspectRatio_MaintainYFOV:
				[[fallthrough]];
			case AspectRatio_MajorAxisFOV:
				[[fallthrough]];
			default:
				halvedPlaneSize.Y = nearClipPlaneDist * FMath::Tan(firstPersonCameraComp->FieldOfView * 0.5f);
				halvedPlaneSize.X = halvedPlaneSize.Y * firstPersonCameraComp->AspectRatio;
				break;
			}
		}
		else {
			halvedPlaneSize.Y = nearClipPlaneDist * FMath::Tan(firstPersonCameraComp->FieldOfView * 0.5f);
			halvedPlaneSize.X = halvedPlaneSize.Y * firstPersonCameraComp->AspectRatio;
		}

	}
	else {
		halvedPlaneSize.Y = nearClipPlaneDist * FMath::Tan(firstPersonCameraComp->FieldOfView * 0.5f);
		halvedPlaneSize.X = halvedPlaneSize.Y * firstPersonCameraComp->AspectRatio;
	}*/

	switch (transformSpace)
	{
	case RTS_World:
	{
		FTransform cameraWorldTransform{ firstPersonCamera->GetComponentTransform() };
		return cameraWorldTransform.GetLocation() +
			cameraWorldTransform.GetRotation().RotateVector({ nearClipPlaneDist, halvedPlaneSize.X * normX, halvedPlaneSize.Y * normY });
	}
	break;
	case RTS_Actor:
		return firstPersonCamera->GetRelativeLocation() +
			firstPersonCamera->GetRelativeRotation().RotateVector({ nearClipPlaneDist, halvedPlaneSize.X * normX, halvedPlaneSize.Y * normY });
		break;
	case RTS_ParentBoneSpace:
	{
		FTransform armToCameraRelativeTransform{ cameraSpringArm->GetRelativeTransform() };
		return armToCameraRelativeTransform.GetLocation() +
			armToCameraRelativeTransform.GetRotation().RotateVector({ nearClipPlaneDist, halvedPlaneSize.X * normX, halvedPlaneSize.Y * normY });
	}
	break;
	case RTS_Component:
		[[fallthrough]];
	default:
		return { nearClipPlaneDist, halvedPlaneSize.X * normX, halvedPlaneSize.Y * normY };
		break;
	}
}

USceneComponent* APlayerCharacter::GetEquipmentAttachParent()
{
	return static_cast<USceneComponent*>(cameraSpringArm);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	AddTickPrerequisiteComponent(vitals);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//handle interaction
	if (actorCurrentlyInteractingWith) {
		//negative means button was just pressed
		if (timeSinceLongInteractionStarted > 0.f) {
			//long interaction is currently in proccess, checks are inside OnInteracting()
			timeSinceLongInteractionStarted += DeltaTime;
			if (actorCurrentlyInteractingWith->GetLongInteractionTime() < timeSinceLongInteractionStarted) {
				//finish long interaction
				if (actorCurrentlyInteractingWith->OnInteract(this, false)) {
					//long interaction successfull
				}
				else {
					//long interaction failed
				}
				StopInteracting();
			}
		}
	}
	else {
		if (GetWorld()->LineTraceSingleByChannel(interactionShotResult, firstPersonCamera->GetComponentLocation(),
			(firstPersonCamera->GetForwardVector() * maxInteractionDistance) + firstPersonCamera->GetComponentLocation(),
			ECC_Visibility, FCollisionQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam)) {
			if (AInteractableActor * interactableInFocus{ Cast<AInteractableActor>(interactionShotResult.GetActor()) }) {
				if (interactableInFocus == actorInFocus) {
					//start showing prompt
				}
				else {
					actorInFocus = interactableInFocus;
				}
			}
		}
		else {
			actorInFocus = nullptr;
		}
	}
	

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::StartInteraction);
	PlayerInputComponent->BindAction("Interact", IE_Repeat, this, &APlayerCharacter::OnInteracting);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &APlayerCharacter::AbortInteraction);
	//PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APlayerCharacter::OnReload);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::ApplySelfDamage);
}

void APlayerCharacter::MoveForward(float value)
{
	if (value != 0.0f) {
		AddMovementInput(GetActorForwardVector(), value);
	}
}

void APlayerCharacter::MoveRight(float value)
{
	if (value != 0.0f) {
		AddMovementInput(GetActorRightVector(), value);
	}
}

void APlayerCharacter::TurnAtRate(float rate)
{
	AddControllerYawInput(rate * baseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float rate)
{
	AddControllerPitchInput(rate * baseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::StartInteraction()
{
	if (GetWorld()->LineTraceSingleByChannel(interactionShotResult, firstPersonCamera->GetComponentLocation(), 
			(firstPersonCamera->GetForwardVector() * maxInteractionDistance) + firstPersonCamera->GetComponentLocation(), 
			ECC_Visibility, FCollisionQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam)) {
		if (AInteractableActor* interactableInFocus{ Cast<AInteractableActor>(interactionShotResult.GetActor()) }) {
			actorCurrentlyInteractingWith = interactableInFocus;
			timeSinceLongInteractionStarted = -1.f;
			SetActorTickInterval(0.f);
			/*/ugly
			APlayerController* playerController{ CastChecked<APlayerController>(Controller) };
			AHUDManager* HUDManager{ Cast<AHUDManager>(playerController->MyHUD) };
			AActor* viewTarger{ playerController->GetViewTarget() };
			//should also work without ui
			if (HUDManager != nullptr && IsValid(viewTarger) && viewTarger->GetClass()->IsChildOf(StaticClass())) {
				
				UInteractionHelper* helper{ HUDManager->GetInteractionHelper() };


			}

			UInteractionHelper* helper{ CastChecked<AWHTS87PlayerController>(Controller)->GetInteractionHelper() };
			if (helper != nullptr && interactableInFocus->IsCurrentlyInteractable(this)) {
				actorCurrentlyInteractingWith = interactableInFocus;
				//helper
				//CastChecked<AWHTS87PlayerController>(Controller)->MyHUD;
				//CastChecked<AWHTS87PlayerController>(Controller)->SetInputMode

			}*/
		}
	}
}

void APlayerCharacter::OnInteracting()
{
	//track location
	check(actorCurrentlyInteractingWith != nullptr);
	if (actorCurrentlyInteractingWith->GetLongInteractionTime() == 0.f)
		return;
	if (actorCurrentlyInteractingWith->IsCurrentlyInteractable(this) && 
		FVector::DistSquared(firstPersonCamera->GetComponentLocation(), actorCurrentlyInteractingWith->GetActorLocation())
		< (maxInteractionDistance + 10.f) * maxInteractionDistance) {
		//proceed
		if (timeSinceLongInteractionStarted < 0.f) {
			//actually start long interaction
			timeSinceLongInteractionStarted = 0.25f;

		}
	}
	else {
		//break interaction
		StopInteracting();
	}
}

void APlayerCharacter::AbortInteraction()
{
	//verify we can still interact
	if (actorCurrentlyInteractingWith->IsCurrentlyInteractable(this) && 
		FVector::DistSquared(firstPersonCamera->GetComponentLocation(), actorCurrentlyInteractingWith->GetActorLocation())
		< (maxInteractionDistance + 10.f) * maxInteractionDistance) {
		if (timeSinceLongInteractionStarted < 0.f) {
			//instant interaction
			timeSinceLongInteractionStarted = 0.25f;
			if (actorCurrentlyInteractingWith->OnInteract(this)) {
				//instant interaction successfull
			}
			else {
				//instant interaction failed
			}
		}
	}
	StopInteracting();
}

void APlayerCharacter::StopInteracting()
{
	SetActorTickInterval(0.25f);
	actorCurrentlyInteractingWith = nullptr;
}
