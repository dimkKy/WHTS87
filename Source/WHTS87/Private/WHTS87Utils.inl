// by Dmitry Kolontay

#pragma once

#include "WHTS87Utils.h"

#include "GameFramework/Actor.h"


template<class S, class O>
bool WHTS87Utils::IsIn(const S& subject, const O* other)
{
	return &subject == other;
}

template<class S, class O, class ...Os>
bool WHTS87Utils::IsIn(const S& subject, const O* other, const Os * ...others)
{
	return &subject == other ? true : IsIn(subject, others...);
}

template<WHTS87Utils::ChildOf<AActor> TActor>
TActor* WHTS87Utils::GetFirstActor(UWorld* world)
{
	check(world);
	auto it{ TActorIterator<TActor>(world) };
	return it ? *it : nullptr;
}

template<WHTS87Utils::ChildOf<AActor> TActor>
void WHTS87Utils::ForActorsOfClass(UWorld* world, std::invocable<AActor*> auto&& func)
{
	check(world);
	for (TActorIterator<TActor> it(world); it; ++it) {
		func(*it);
	}
}

template<WHTS87Utils::ChildOf<AActor> TActor, WHTS87Utils::ChildOf<AActor>...TOthers>
void WHTS87Utils::ForActorsOfClass(UWorld* world, std::invocable<AActor*> auto&& func)
	requires WHTS87Utils::NonEmpty<TOthers...>
{
	ForActorsOfClass<TActor>(world, func);
	ForActorsOfClass<TOthers...>(world, func);
}
