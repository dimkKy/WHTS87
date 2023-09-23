// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "WHTS87Utils.h"

/**
 * 
 */
class WHTS87_API FItemPropertiesBase/* : public TSharedFromThis<FItemPropertiesBase, ESPMode::NotThreadSafe>*/
{
public:
	template <WHTS87Utils::ChildOf<FItemPropertiesBase> TProperties, class...Args>
	static TUniquePtr<FItemPropertiesBase> MakeUniquePtr(const Args&&...args) {

		return TUniquePtr<FItemPropertiesBase>(
			static_cast<FItemPropertiesBase*>(new TProperties(std::forward<Args>(args)...)));
	}

	
	virtual ~FItemPropertiesBase() = default;
protected:
	FItemPropertiesBase();
};
