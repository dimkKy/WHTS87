// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "CineCameraActor.h"
#include "IntroCamera.generated.h"

class AMonitor;

UCLASS()
class WHTS87_API AIntroCamera : public ACineCameraActor
{
	GENERATED_BODY()
	
public:	
	AIntroCamera(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& context) const override;
#endif
protected:
	virtual void BeginPlay() override;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		//UCameraComponent* camera;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, DuplicateTransient)
		float transitionDuration;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		TSoftObjectPtr <AMonitor> targetMonitor;

	FVector targetPos;
	FVector targetRot;
};
