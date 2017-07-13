/*
	Copyright(c) 2016-2017 Codefluegel GMBH All Rights Reserved.
    Distributed under the BSD 3-Clause license.
*/

#pragma once

#include "Components/ActorComponent.h"
#include "CFVuforiaActorComponent.generated.h"

#if PLATFORM_ANDROID
namespace Vuforia
{
	class DataSet;
}
#endif

DECLARE_DELEGATE(FCameraDelegate);

/**
* Encapsulates Vuforia and Camera device initialisation
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UVuforiaActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UVuforiaActorComponent();

	virtual void InitializeComponent() override;

	FCameraDelegate CameraDelegate;
private:
#if PLATFORM_ANDROID
	void InitTrackerAndDataset();
	void StartCameraAndTracker();
	void StopCameraAndTracker();
	void DeinitTrackerAndDataset();

	void HandleAppPause();
	void HandleAppResume();
	void HandleAppDestroy();

	Vuforia::DataSet* DataSet;
#endif
};
