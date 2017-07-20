/*
	Copyright(c) 2016-2017 Codefluegel GMBH All Rights Reserved.
    Distributed under the BSD 3-Clause license.
*/

#include "CFVuforiaPrivatePCH.h"
#include "CFVuforiaActor.h"
#include "Private/CFVuforiaActorComponent.h"
#include "Private/CFVuforiaUtils.h"
#include "Private/CFVuforiaVideoPlaneComponent.h"

#include "Camera/CameraComponent.h"
#include "Camera/CameraTypes.h"
#include "Components/StaticMeshComponent.h"
#include "EngineUtils.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/DefaultPawn.h"
#include "SceneView.h"

#if PLATFORM_ANDROID
  #include <Vuforia/Vuforia.h>
  #include <Vuforia/State.h>
  #include <Vuforia/StateUpdater.h>
  #include <Vuforia/Trackable.h>
  #include <Vuforia/TrackableResult.h>
  #include <Vuforia/TrackerManager.h>
  #include <Vuforia/Tool.h>
  #include <Vuforia/CameraDevice.h>
  #include <Vuforia/Image.h>
  #include <Vuforia/Frame.h>
#endif

AVuforiaActor::AVuforiaActor()
{
	PrimaryActorTick.bCanEverTick = true;

	UVuforiaActorComponent* ActorComponent = CreateDefaultSubobject<UVuforiaActorComponent>("VuforiaComponent");
	AddOwnedComponent(ActorComponent);
	ActorComponent->CameraDelegate.BindUObject(this, &AVuforiaActor::OnCameraActive);

	mVideoPlane = CreateDefaultSubobject<UVuforiaVideoPlaneComponent>(TEXT("GeneratedMesh"));
	mVideoPlane->Create();
	mVideoPlane->AttachToComponent(GetCameraComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	AddInstanceComponent(mVideoPlane);
}

FVector2D AVuforiaActor::GetArPlaneScale()
{
	FViewport* const Viewport = GEngine->GameViewport->ViewportFrame->GetViewport();

	FSceneViewInitOptions ViewInitOptions;
	FMinimalViewInfo camViewInfo;
	const EAspectRatioAxisConstraint AspectRatioAxisConstraint = GetDefault<ULocalPlayer>()->AspectRatioAxisConstraint;

	GetCameraComponent()->GetCameraView(0, camViewInfo);
	camViewInfo.Location = FVector4(0, 0, 0, 0);
	camViewInfo.Rotation = FRotator(0, 0, 0);

	GetCameraComponent()->GetCameraView(0, camViewInfo);

	FMinimalViewInfo::CalculateProjectionMatrixGivenView(camViewInfo, AspectRatioAxisConstraint, Viewport, ViewInitOptions);

	FMatrix model(
		FPlane(1.f, 0.f, 0.f, 0.f),
		FPlane(0.f, 1.f, 0.f, 0.f),
		FPlane(0.f, 0.f, 1.f, 0.f),
		FPlane(0.f, 0.f, 0.f, 1.f));

	FMatrix vectorWrap(
		FPlane(1, 0.f, 0.f, 0.f),
		FPlane(1, 0.f, 0.f, 0.f),
		FPlane(0.f, 0.f, 0.f, 0.f),
		FPlane(0.f, 0.f, 0.f, 0.f));

	FMatrix scale = (ViewInitOptions.ProjectionMatrix * model).Inverse() * vectorWrap;

	return FVector2D(scale.M[0][0] * 2.f * (float)AR_PLANE_DISTANCE, scale.M[1][0] * 2.f * (float)AR_PLANE_DISTANCE);
}

void AVuforiaActor::HideDefaultPawnCollisionComponent()
{
	for (TActorIterator<ADefaultPawn> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ADefaultPawn *pawn = *ActorItr;
		pawn->GetMeshComponent()->SetVisibility(false);
	}
}

void AVuforiaActor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(CFVuforiaLog, Warning, TEXT("AVuforiaActor::BeginPlay"));
	HideDefaultPawnCollisionComponent();
	if (TrackingLost.IsBound())
	{
		TrackingLost.Broadcast();
	}
}

void AVuforiaActor::OnCameraActive()
{
#if PLATFORM_ANDROID
	static bool once = false;
	if (!once)
	{
		const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
		float ScreenAspect = (float)ViewportSize.X / (float)ViewportSize.Y;
		UE_LOG(CFVuforiaLog, Warning, TEXT("Viewport size = %d,%d"), (int)ViewportSize.X, (int)ViewportSize.Y);

		const Vuforia::CameraCalibration& calibration = Vuforia::CameraDevice::getInstance().getCameraCalibration();
		UE_LOG(CFVuforiaLog, Warning, TEXT("CameraActive %f"), calibration.getFieldOfViewRads().data[0] * 180.f / M_PI);
		GetCameraComponent()->FieldOfView = calibration.getFieldOfViewRads().data[0] * 180.f / M_PI;
		GetCameraComponent()->AspectRatio = ScreenAspect;

		mVideoPlane->Init(GetArPlaneScale());
		once = true;
	}
#endif
}

// Called every frame
void AVuforiaActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
#if PLATFORM_ANDROID
	
	Vuforia::TrackerManager& trackerManager = Vuforia::TrackerManager::getInstance();
	Vuforia::StateUpdater& stateUpdater = trackerManager.getStateUpdater();
	Vuforia::State state = stateUpdater.updateState();
	if (state.getNumTrackableResults() == 1)
	{
		if (TrackingFound.IsBound())
		{
			TrackingFound.Broadcast();
		}

		const Vuforia::TrackableResult* result = state.getTrackableResult(0);
		const Vuforia::Trackable& trackable = result->getTrackable();

		Vuforia::Matrix44F mat =
			Vuforia::Tool::convertPose2GLMatrix(result->getPose());

		SetActorTransform(FTransform(CFVuforiaUtils::glToUE(mat)));
	}
	else
	{
		if (TrackingLost.IsBound())
		{
			TrackingLost.Broadcast();
		}
	}
	mVideoPlane->Update();
#endif
}