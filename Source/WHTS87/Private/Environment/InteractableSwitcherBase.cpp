// by Dmitry Kolontay


#include "Environment/InteractableSwitcherBase.h"
#include "Components/StaticMeshComponent.h"

AInteractableSwitcherBase::AInteractableSwitcherBase() :switcherFrame{ CreateDefaultSubobject<UStaticMeshComponent>("switcherFrameComponent") },
switcherBody{ CreateDefaultSubobject<UStaticMeshComponent>("switcherBodyComponent") },
animationTimeline{ CreateDefaultSubobject<UTimelineComponent>("animationTimelineComponent") },
animationCurve{ nullptr }, timeSincePressed{ 777.f }
{
	SetActorTickEnabled(false);
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(switcherFrame);
	switcherFrame->SetMobility(EComponentMobility::Stationary);
	switcherFrame->SetCollisionObjectType(ECC_WorldStatic);
	switcherFrame->SetGenerateOverlapEvents(false);

	//no collision
	switcherBody->SetupAttachment(switcherFrame);
	switcherBody->SetMobility(EComponentMobility::Movable);
	switcherBody->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	animationTimeline->SetLooping(false);
	animationTimeline->SetTimelineLength(1.f);
	animationTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
}

void AInteractableSwitcherBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	timeSincePressed += DeltaTime;
	if (animationCurve == nullptr && timeSincePressed > 1.f) {
		SetActorTickEnabled(false);
	}
}

void AInteractableSwitcherBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

bool AInteractableSwitcherBase::IsCurrentlyInteractable(AActor* caller) const
{
	if (animationCurve) {
		return !animationTimeline->IsPlaying();
	}
	else {
		return timeSincePressed > 1.f ? true : false;
	}
}

void AInteractableSwitcherBase::BeginPlay()
{
	Super::BeginPlay();
	if (animationCurve) {
		onTimeLineUpdated.BindDynamic(this, &AInteractableSwitcherBase::UpdateBodyPosition);
		animationTimeline->AddInterpFloat(animationCurve, onTimeLineUpdated);
		onTimelineFinished.BindUObject(this, &AInteractableSwitcherBase::OnTimelineFinished);
		animationTimeline->SetTimelineFinishedFunc(onTimelineFinished);
	}
}

UE_NODISCARD bool AInteractableSwitcherBase::OnInstantInteraction(AActor* caller)
{
	if (animationCurve) {
		animationTimeline->PlayFromStart();
	}
	else {
		timeSincePressed = 0.f;
		SetActorTickEnabled(true);
		//OnEventDispatch();
	}
	return true;
}

UE_NODISCARD bool AInteractableSwitcherBase::OnLongInteraction(AActor* caller)
{
	return Super::OnLongInteraction(caller);
}
