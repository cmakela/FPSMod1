// Charles Makela

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraDirector.generated.h"

UCLASS()
class FPSMOD1_API ACameraDirector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraDirector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = Cameras)
		AActor* CameraOne;

	UPROPERTY(EditAnywhere, Category = Cameras)
		AActor* CameraTwo;

	UPROPERTY(EditAnywhere, Category = Cameras)
		AActor* CameraThree;

	float TimeToNextCameraChange;


};
