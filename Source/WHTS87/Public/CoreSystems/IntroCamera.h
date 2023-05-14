// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IntroCamera.generated.h"

class UCameraComponent;
class AMonitor;

UCLASS()
class WHTS87_API AIntroCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	AIntroCamera();
	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UCameraComponent* camera;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, DuplicateTransient)
		float transitionDuration;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		TSoftObjectPtr <AMonitor> targetMonitor;

	FVector targetPos;
};
