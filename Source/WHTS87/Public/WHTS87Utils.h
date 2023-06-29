// by Dmitry Kolontay

#pragma once

//#include "CoreMinimal.h"
#include <type_traits>

class UCameraComponent;
//class UButton;
//class UObject;

#include "UObject/Object.h"
#include "Components/Button.h"

 namespace WHTS87Utils {
	 //if constexpr?
	 template <class S, class O>
	 inline bool IsIn(const S& subject, const O* other);

	 template <class S, class O, class...Os>
	 inline bool IsIn(const S& subject, const O* other, const Os*... others);

	 template <class TDerived, class TBase, bool allowSame = false>
	 concept ChildOf = std::is_base_of<TBase, TDerived>::value &&
		 (allowSame || std::negation<std::is_same<TBase, TDerived>>::value);

	 template <typename TDerived, template<typename> typename TBase>
	 struct is_derived_from_any
	 {
		 template<typename TParam>
		 static constexpr std::true_type is_derived(const volatile TBase<TParam>&);
		 static constexpr std::false_type is_derived(...);
		 using type = decltype(is_derived(std::declval<TDerived&>()));
	 };

	 template <class TDerived, template<typename> typename TBase>
	 concept ChildOfAny = is_derived_from_any<TDerived, TBase>::type::value;

	 constexpr float defaultNearClipPlane = 10.f;
	 float GetCameraNearPlane(const UCameraComponent& camera);

	 FName GetNumberedName(const FString& source, int32 number = 0);

	 template <typename... Args>
	 concept NonEmpty = sizeof...(Args) > 0;

	 /*template <ChildOf<UObject> TObject>
	 void SetupButtons(TObject* object, UButton* button, void (TObject::* func)(void)) {
		 button->SetClickMethod(EButtonClickMethod::PreciseClick);
		 button->OnClicked.AddDynamic(object, func);
		 //static_assert(object != nullptr && button != nullptr, "nulls aree not allowed");
	 }*/

	 template <ChildOf<AActor> TActor>
	 auto* GetFirstActor(UWorld* world);

	 template <ChildOf<AActor> TActor>
	 void ForActorsOfClass(UWorld* world, std::invocable<AActor*> auto&& func);

	 template <ChildOf<AActor> TActor, ChildOf<AActor>...TOthers>
	 void ForActorsOfClass(UWorld* world, std::invocable<AActor*> auto&& func) requires NonEmpty<TOthers...>;

 }

 namespace WHTS87Utils::NamingRules {
	 

	 const FName toNextSWSocket{ "NextFloorSocket" };
	 const FString doorSocket{ "DoorSocket" };
	 const FString doorFrameSocket{ "DoorFrameSocket" };
	 const FString specialRoomSocket{ "SpecialSocket" };
	 const FString subroomSocket{ "SubroomSocket" };
	 const FString volumePoint{ "volumePoint" };

 }

#include "WHTS87Utils.inl"
