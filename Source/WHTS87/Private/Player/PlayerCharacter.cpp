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
#include "WHTS87Utils.h"

const FVector APlayerCharacter::cameraRelativeLocation{ -15.f, 0.f, 70.f };

APlayerCharacter::APlayerCharacter() : 
	firstPersonCamera{ CreateDefaultSubobject<UCameraComponent>("firstPersonCamera") },
	cameraSpringArm{ CreateDefaultSubobject<USpringArmComponent>("cameraSpringArm") },
	inventory{ CreateDefaultSubobject<UPlayerInventoryComponent>("inventory") },
	vitals{ CreateDefaultSubobject<UPlayerVitalsComponent>("vitals") },
	baseTurnRate{ 45.f }, baseLookUpRate{ 45.f }, maxInteractionDistance{ 450.f },
	actorInFocus{ nullptr }, actorInteractingWith{ nullptr }
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickInterval(0.25f);
	GetCapsuleComponent()->InitCapsuleSize(capsuleRadius, capsuleHalfHeight);

	firstPersonCamera->SetupAttachment(GetCapsuleComponent());
	firstPersonCamera->SetRelativeLocation(cameraRelativeLocation);
	firstPersonCamera->bUsePawnControlRotation = true;

	cameraSpringArm->SetupAttachment(firstPersonCamera);
	cameraSpringArm->TargetArmLength = 0.f;
	cameraSpringArm->bDoCollisionTest = false;
	cameraSpringArm->bUsePawnControlRotation = false;
	cameraSpringArm->bEnableCameraRotationLag = true;
	cameraSpringArm->CameraRotationLagSpeed = cameraRotationLagSpeed;
}

FVector APlayerCharacter::DeprojectCameraScreenPlane(const FVector2D& _normCord, ERelativeTransformSpace transformSpace) const
{
	FVector2D normCoord{ _normCord.ClampAxes(-1.f, 1.f) };
	float nearClipDist{ WHTS87Utils::GetCameraNearPlane(*firstPersonCamera) };
	FVector2D halvedPlaneSize{ nearClipDist * FMath::Tan(firstPersonCamera->FieldOfView * 0.5f) };
	halvedPlaneSize.X = halvedPlaneSize.Y * firstPersonCamera->AspectRatio;
	/*FVector2D halvedPlaneSize;
	if (auto playerController = Cast<APlayerController>(Controller)) {
		if (auto localPlayer = Cast<ULocalPlayer>(playerController->Player)) {
			switch (localPlayer->AspectRatioAxisConstraint.GetValue())
			{
			case AspectRatio_MaintainXFOV:
				halvedPlaneSize.X = nearClipDist * FMath::Tan(firstPersonCameraComp->FieldOfView * 0.5f);
				halvedPlaneSize.Y = halvedPlaneSize.X / firstPersonCameraComp->AspectRatio;
				break;
			case AspectRatio_MaintainYFOV:
				[[fallthrough]];
			case AspectRatio_MajorAxisFOV:
				[[fallthrough]];
			default:
				halvedPlaneSize.Y = nearClipDist * FMath::Tan(firstPersonCameraComp->FieldOfView * 0.5f);
				halvedPlaneSize.X = halvedPlaneSize.Y * firstPersonCameraComp->AspectRatio;
				break;
			}
		}
		else {
			halvedPlaneSize.Y = nearClipDist * FMath::Tan(firstPersonCameraComp->FieldOfView * 0.5f);
			halvedPlaneSize.X = halvedPlaneSize.Y * firstPersonCameraComp->AspectRatio;
		}

	}
	else {
		halvedPlaneSize.Y = nearClipDist * FMath::Tan(firstPersonCameraComp->FieldOfView * 0.5f);
		halvedPlaneSize.X = halvedPlaneSize.Y * firstPersonCameraComp->AspectRatio;
	}*/

	switch (transformSpace) {
	case RTS_World:
	{
		FTransform cameraWorld{ firstPersonCamera->GetComponentTransform() };
		return cameraWorld.GetLocation() +
			cameraWorld.GetRotation().RotateVector({ nearClipDist, halvedPlaneSize.X * normCoord.X, halvedPlaneSize.Y * normCoord.Y });
	}
	break;
	case RTS_Actor:
		return firstPersonCamera->GetRelativeLocation() +
			firstPersonCamera->GetRelativeRotation().RotateVector({ nearClipDist, halvedPlaneSize.X * normCoord.X, halvedPlaneSize.Y * normCoord.Y });
		break;
	case RTS_ParentBoneSpace:
	{
		FTransform armToCameraRelativeTransform{ cameraSpringArm->GetRelativeTransform() };
		return armToCameraRelativeTransform.GetLocation() +
			armToCameraRelativeTransform.GetRotation().RotateVector({ nearClipDist, halvedPlaneSize.X * normCoord.X, halvedPlaneSize.Y * normCoord.Y });
	}
	break;
	case RTS_Component:
		[[fallthrough]];
	default:
		return { nearClipDist, halvedPlaneSize.X * normCoord.X, halvedPlaneSize.Y * normCoord.Y };
		break;
	}
}

USceneComponent* APlayerCharacter::GetEquipmentAttachParent() const
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
	if (actorInteractingWith) {
		//negative means button was just pressed
		if (timeSinceLongInteractionStart > 0.f) {
			//long interaction is currently in proccess, checks are inside OnInteracting()
			timeSinceLongInteractionStart += DeltaTime;
			if (actorInteractingWith->GetLongInteractionTime() < timeSinceLongInteractionStart) {
				//finish long interaction
				if (actorInteractingWith->OnInteract(this, false)) {
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
		if (auto* traceResult{ CameraForwardLineTrace(ECC_Visibility) }) {
			if (traceResult->IsA(AInteractableActor::StaticClass())) {
				if (traceResult == actorInFocus) {
					//start showing prompt
				}
				else {
					actorInFocus = traceResult;
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

AActor* APlayerCharacter::CameraForwardLineTrace(ECollisionChannel channel)
{
	check(GetWorld());
	auto cameraLocation{ firstPersonCamera->GetComponentLocation() };
	if (GetWorld()->LineTraceSingleByChannel(interactionShotResult, cameraLocation,
		(firstPersonCamera->GetForwardVector() * maxInteractionDistance) + cameraLocation, channel,
		FCollisionQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam)) {
		return interactionShotResult.GetActor();
	}
	else {
		return nullptr;
	}
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
			actorInteractingWith = interactableInFocus;
			timeSinceLongInteractionStart = -1.f;
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
				actorInteractingWith = interactableInFocus;
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
	check(actorInteractingWith);
	if (actorInteractingWith->GetLongInteractionTime() == 0.f) {
		return;
	}
	if (VerifyInteraction(*actorInteractingWith)) {
		//proceed
		if (timeSinceLongInteractionStart < 0.f) {
			//actually start long interaction
			timeSinceLongInteractionStart = 0.25f;

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
	if (VerifyInteraction(*actorInteractingWith)) {
		if (timeSinceLongInteractionStart < 0.f) {
			//instant interaction
			timeSinceLongInteractionStart = 0.25f;
			if (actorInteractingWith->OnInteract(this)) {
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
	actorInteractingWith = nullptr;
}

bool APlayerCharacter::VerifyInteraction(const AInteractableActor& other) const
{
	return other.IsCurrentlyInteractable(this) &&
		FVector::DistSquared(firstPersonCamera->GetComponentLocation(), other.GetActorLocation())
		< (maxInteractionDistance + 10.f) * maxInteractionDistance;
}
