// by Dmitry Kolontay


#include "Components/VitalsComponent.h"
#include "Containers/List.h"

UVitalsComponent::UVitalsComponent() : maxHPBase{ 100.f }, HPRegenPerSecond{0.5f}, currentHP { maxHPBase }
{
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickInterval(0.5f);
}

void UVitalsComponent::BeginPlay()
{
	Super::BeginPlay();

}

float UVitalsComponent::ModifyQueue(TDoubleLinkedList<FPointsModifier>& queue, float deltaTime)
{
	float result{ 0.f };
	TDoubleLinkedList<FPointsModifier>::TDoubleLinkedListNode* currentModifier{ queue.GetHead() };
	while (currentModifier != nullptr) {
		if (currentModifier->GetValue().durationLeft <= deltaTime) {
			result += currentModifier->GetValue().amountLeft;
			TDoubleLinkedList<FPointsModifier>::TDoubleLinkedListNode* modifierToDelete{ currentModifier };
			currentModifier = currentModifier->GetNextNode();
			queue.RemoveNode(modifierToDelete);
		}
		else {
			float temp{ deltaTime * currentModifier->GetValue().amountLeft / currentModifier->GetValue().durationLeft };
			result += temp;
			currentModifier->GetValue().amountLeft -= temp;
			currentModifier->GetValue().durationLeft -= deltaTime;
			currentModifier = currentModifier->GetNextNode();
		}
	}
	return result;
}

void UVitalsComponent::OnHPEnded()
{

}

void UVitalsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TickType == ELevelTick::LEVELTICK_All || TickType == ELevelTick::LEVELTICK_TimeOnly) {
		currentHP += ModifyQueue(healthModifiersQueue, DeltaTime);
		currentHP += HPRegenPerSecond * DeltaTime;
		if (currentHP > GetMaxHP()) {
			currentHP = GetMaxHP();
		}
		else {
			if (currentHP <= 0.f)
				OnHPEnded();
		}

	}
}

void UVitalsComponent::ApplyPhysicalDamage(float amount, float duration)
{
	if(amount != 0.f && duration >= 0)
		healthModifiersQueue.AddTail({ amount, duration });
}

