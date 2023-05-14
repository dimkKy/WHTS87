// by Dmitry Kolontay

#pragma once

//#include "CoreMinimal.h"
#include <type_traits>

class UCameraComponent;

 namespace WHTS87Utils {

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
