// by Dmitry Kolontay


#include "Player/PlayerVitalsComponent.h"

UPlayerVitalsComponent::UPlayerVitalsComponent() : sanityPercentRegenPerSecond{0.01f}, currentSanityPercent{100.f}
{
}

void UPlayerVitalsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TickType == ELevelTick::LEVELTICK_All || TickType == ELevelTick::LEVELTICK_TimeOnly) {
		currentSanityPercent += ModifyQueue(sanityModifiersQueue, DeltaTime);
		currentSanityPercent += sanityPercentRegenPerSecond * DeltaTime;
		if (currentSanityPercent > 100.f) {
			currentSanityPercent = 100.f;
		}
		else {
			if (currentSanityPercent <= 0.f)
				OnSanityEnded();
		}

	}
}

void UPlayerVitalsComponent::ApplyMentalDamage(float amount, float duration)
{
	if (amount != 0.f && duration >= 0)
		sanityModifiersQueue.AddTail({ amount, duration });
}

void UPlayerVitalsComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerVitalsComponent::OnSanityEnded()
{
}
