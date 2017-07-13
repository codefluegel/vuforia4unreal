/*
	Copyright(c) 2016-2017 Codefluegel GMBH All Rights Reserved.
    Distributed under the BSD 3-Clause license.
*/

#pragma once

#include "Camera/CameraActor.h"

#include "ProceduralMeshComponent.h"

#include "CFVuforiaActor.generated.h"


class FViewport;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FVuforiaTrackingEvent);

/**
* VuforiaActor positions the camera according to the transformation received from the Vuforia SDK
* and renders the camera stream.
*/
UCLASS(config = DefaultEngine, defaultconfig)
class CFVUFORIA_API AVuforiaActor : public ACameraActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AVuforiaActor();

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable, Category = "Vuforia")
	FVuforiaTrackingEvent TrackingFound;

	UPROPERTY(BlueprintAssignable, Category = "Vuforia")
	FVuforiaTrackingEvent TrackingLost;
private:
	FVector2D GetArPlaneScale();

	void OnCameraActive();

	void HideDefaultPawnCollisionComponent();

	class UVuforiaVideoPlaneComponent* mVideoPlane;
};
