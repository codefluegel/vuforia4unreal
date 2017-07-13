/*
	Copyright(c) 2016-2017 Codefluegel GMBH All Rights Reserved.
*/

#include "CFVuforiaPrivatePCH.h"
#include "CFVuforiaVideoMaterial.h"

#include "ProceduralMeshComponent.h"

#include "CFVuforiaActorComponent.h"
#include "CFVuforiaUtils.h"

#include "Engine/Texture2D.h"
#include "Materials/MaterialInstanceDynamic.h"

#if PLATFORM_ANDROID
	#include <Vuforia/Renderer.h>
	#include <Vuforia/GLRenderer.h>
	#include <Vuforia/TrackerManager.h>
	#include <Vuforia/StateUpdater.h>
	#include <Vuforia/VideoBackgroundTextureInfo.h>
#endif

VuforiaVideoMaterial::VuforiaVideoMaterial()
{
	mDynamicMaterialInstance = nullptr;
	mDynamicTexture = nullptr;
}

void VuforiaVideoMaterial::Init(UProceduralMeshComponent* Mesh)
{
	UMaterial* material = LoadObject<UMaterial>(NULL, TEXT("/Game/M_DynamicTexture.M_DynamicTexture"), NULL, LOAD_None, NULL);
	Mesh->SetMaterial(0, material);
}

void VuforiaVideoMaterial::CreateDynamic(UProceduralMeshComponent* Mesh)
{
	mDynamicMaterialInstance = Mesh->CreateAndSetMaterialInstanceDynamic(0);
}

static void setVideoTexture(void* resource)
{
#if PLATFORM_ANDROID
	int32 textureID = *reinterpret_cast<int32*>(resource);
	if (!Vuforia::Renderer::getInstance().setVideoBackgroundTexture(Vuforia::GLTextureData(textureID))) {
		UE_LOG(CFVuforiaLog, Warning, TEXT("setVideoBackgroundTexture failed"));
	}
	else {
		UE_LOG(CFVuforiaLog, Warning, TEXT("setVideoBackgroundTexture succeded"));
	}
#endif
}

void VuforiaVideoMaterial::Setup()
{
	mDynamicTexture = UTexture2D::CreateTransient(1, 1, PF_FloatRGBA);
	mDynamicTexture->AddToRoot();
	mDynamicTexture->UpdateResource();

	// UpdateResource initializes the resource in the rendering thread,
	// so we enqueue our setVideoBackgroundTexture after it.
	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
		InitCommandTest,
		UTexture2D*, mDynamicTexture, mDynamicTexture,
		{
			setVideoTexture(mDynamicTexture->Resource->TextureRHI->GetNativeResource());
		});
	
	mDynamicMaterialInstance->SetTextureParameterValue("DynamicTextureParam", mDynamicTexture);
}

bool VuforiaVideoMaterial::Initialized()
{
	return mDynamicTexture != nullptr;
}

void VuforiaVideoMaterial::Update()
{
#if PLATFORM_ANDROID
	Vuforia::Renderer::getInstance().begin();
	if (!Vuforia::Renderer::getInstance().updateVideoBackgroundTexture(nullptr))
	{
		UE_LOG(CFVuforiaLog, Warning, TEXT("updateVideoBackgroundTexture failed"));
	}
	Vuforia::Renderer::getInstance().end();
#endif
}