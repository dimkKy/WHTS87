// by Dmitry Kolontay

#pragma once

#include "CoreMinimal.h"
#include "Environment/InteractableActor.h"
#include "Monitor.generated.h"

class UWidgetComponent;
class UUserWidget;
/**
 * 
 */
UCLASS()
class WHTS87_API AMonitor : public AInteractableActor
{
	GENERATED_BODY()
public:
	AMonitor();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;
	//bool UE_NODISCARD TrySetSourceCamera(ASurveillanceCamera* newSourceCamera);
	//bool UE_NODISCARD TrySetSourceCamera(TSoftObjectPtr<ASurveillanceCamera> newSourceCamera);

	FVector GetCloseViewingPos(float cameraFOV, float aspectRatio, float nearClipPlane);

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& context) const override;
#endif

protected:
	virtual void BeginPlay() override;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> widgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UWidgetComponent* display;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		//UTextureRenderTarget2D* renderTarget;

	/*UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Config")
		ASurveillanceCamera* sourceCamera;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Config")
		TSoftObjectPtr <ASurveillanceCamera> sourceCameraSoft;*/
	//UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Config")
		//TSoftObjectPtr <ASurveillanceCamera> sourceCameraSoft;

};
