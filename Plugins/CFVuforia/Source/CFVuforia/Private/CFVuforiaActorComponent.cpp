/*
	Copyright(c) 2016-2017 Codefluegel GMBH All Rights Reserved.
*/

#include "CFVuforiaPrivatePCH.h"
#include "CFVuforiaActorComponent.h"

#include "CFVuforiaActor.h"
#include "CFVuforiaUtils.h"

#if PLATFORM_ANDROID
  #include <Vuforia/Vuforia.h>
  #include <Vuforia/CameraDevice.h>
  #include <Vuforia/Tracker.h>
  #include <Vuforia/TrackerManager.h>
  #include <Vuforia/Trackable.h>
  #include <Vuforia/ObjectTracker.h>
  #include <Vuforia/DataSet.h>

  #include "Misc/CoreDelegates.h"
#endif

UVuforiaActorComponent::UVuforiaActorComponent()
{
	bWantsInitializeComponent = true;
#if PLATFORM_ANDROID
	DataSet = nullptr;
#endif
}

void UVuforiaActorComponent::InitializeComponent()
{
	Super::InitializeComponent();

#if PLATFORM_ANDROID
	FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddUObject(this, &UVuforiaActorComponent::HandleAppPause);
	FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddUObject(this, &UVuforiaActorComponent::HandleAppResume);
	FCoreDelegates::ApplicationWillTerminateDelegate.AddUObject(this, &UVuforiaActorComponent::HandleAppDestroy);

	InitTrackerAndDataset();
#endif
}

#if PLATFORM_ANDROID
void UVuforiaActorComponent::HandleAppPause()
{
	StopCameraAndTracker();
	Vuforia::onPause();
}

void UVuforiaActorComponent::HandleAppResume()
{
	Vuforia::onResume();
	StartCameraAndTracker();
	CameraDelegate.Execute();
}

void UVuforiaActorComponent::HandleAppDestroy()
{
	DeinitTrackerAndDataset();
	Vuforia::deinit();
}

void UVuforiaActorComponent::InitTrackerAndDataset()
{
	// Tracker needs to be initialised before initialising and activating camera
	Vuforia::TrackerManager& trackerManager = Vuforia::TrackerManager::getInstance();
	Vuforia::ObjectTracker* tracker = static_cast<Vuforia::ObjectTracker*>(trackerManager.initTracker(Vuforia::ObjectTracker::getClassType()));
	if (tracker == nullptr)
	{
		UE_LOG(CFVuforiaLog, Error, TEXT("Failed to initialize ObjectTracker."));
		return;
	}

	UE_LOG(CFVuforiaLog, Log, TEXT("Successfully initialized ObjectTracker."));

	DataSet = tracker->createDataSet();
	if (DataSet == nullptr)
	{
		UE_LOG(CFVuforiaLog, Error, TEXT("Failed to create a new tracking data."));
		return;
	}
	// Load the data set
	if (!DataSet->load("StonesAndChips.xml", Vuforia::STORAGE_APPRESOURCE))
	{
		UE_LOG(CFVuforiaLog, Error, TEXT("Failed to load data set."));
		return;
	}
	// Activate the data set
	if (!tracker->activateDataSet(DataSet))
	{
		UE_LOG(CFVuforiaLog, Error, TEXT("Failed to activate data set."));
		return;
	}

	for (int tIdx = 0; tIdx < DataSet->getNumTrackables(); tIdx++)
	{
		Vuforia::Trackable* trackable = DataSet->getTrackable(tIdx);
		if (!trackable->startExtendedTracking())
		{
			UE_LOG(CFVuforiaLog, Error, TEXT("Failed to start extended tracking."));
		}
	}
	UE_LOG(CFVuforiaLog, Log, TEXT("Successfully loaded and activated data set."));
}

void UVuforiaActorComponent::DeinitTrackerAndDataset()
{
	Vuforia::TrackerManager& trackerManager = Vuforia::TrackerManager::getInstance();
	Vuforia::ObjectTracker* objectTracker = static_cast<Vuforia::ObjectTracker*>(trackerManager.getTracker(Vuforia::ObjectTracker::getClassType()));
	if (objectTracker == nullptr)
	{
		UE_LOG(CFVuforiaLog, Warning, TEXT("Failed to deinit Tracker: not initialized."));
		return;

	}
	// destroy data set
	if (DataSet != nullptr)
	{
		if (objectTracker->getActiveDataSet(0) == DataSet &&
			!objectTracker->deactivateDataSet(DataSet))
		{
			UE_LOG(CFVuforiaLog, Warning, TEXT("Failed to destroy dataset: could not be deactivated."));
			return;
		}

		if (!objectTracker->destroyDataSet(DataSet))
		{
			UE_LOG(CFVuforiaLog, Warning, TEXT("Failed to destroy tracking dataset."));
			return;
		}
		DataSet = nullptr;
	}

	// deinit tracker
	if (!trackerManager.deinitTracker(Vuforia::ObjectTracker::getClassType()))
	{
		UE_LOG(CFVuforiaLog, Error, TEXT("Failed to deinit Tracker."));
		return;
	}
}

void UVuforiaActorComponent::StartCameraAndTracker()
{
	if (!Vuforia::CameraDevice::getInstance().init())
	{
		UE_LOG(CFVuforiaLog, Error, TEXT("CameraDevice::init failed"));
		return;
	}
	if (!Vuforia::CameraDevice::getInstance().start())
	{
		UE_LOG(CFVuforiaLog, Error, TEXT("CameraDevice::start failed"));
		return;
	}

	Vuforia::TrackerManager& trackerManager = Vuforia::TrackerManager::getInstance();
	Vuforia::Tracker* objectTracker = trackerManager.getTracker(Vuforia::ObjectTracker::getClassType());
	if (!objectTracker || !objectTracker->start())
	{
		UE_LOG(CFVuforiaLog, Error, TEXT("ObjectTracker::start failed"));
		return;
	}
}

void UVuforiaActorComponent::StopCameraAndTracker()
{
	Vuforia::TrackerManager& trackerManager = Vuforia::TrackerManager::getInstance();
	Vuforia::Tracker* objectTracker = trackerManager.getTracker(Vuforia::ObjectTracker::getClassType());
	if (objectTracker != nullptr)
	{
		objectTracker->stop();
	}

	if (!Vuforia::CameraDevice::getInstance().stop())
	{
		UE_LOG(CFVuforiaLog, Error, TEXT("Failed to stop camera"));
		return;
	}
	if (!Vuforia::CameraDevice::getInstance().deinit())
	{
		UE_LOG(CFVuforiaLog, Error, TEXT("Failed to deinit camera"));
		return;
	}
}

#endif