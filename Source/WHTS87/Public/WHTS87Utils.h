// by Dmitry Kolontay

#pragma once

//#include "CoreMinimal.h"

 namespace WHTS87Utils {

	 template<class S, class O>
	 inline bool IsIn(const S& subject, const O* other);

	 template<class S, class O, class...Os>
	 inline bool IsIn(const S& subject, const O* other, const Os*... others);
 }

#include "WHTS87Utils.inl"
