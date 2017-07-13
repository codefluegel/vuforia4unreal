/*
	Copyright(c) 2016-2017 Codefluegel GMBH All Rights Reserved.
    Distributed under the BSD 3-Clause license.
*/

#pragma once

#include "ProceduralMeshComponent.h"

#include "CFVuforiaVideoMaterial.h"

#include "CFVuforiaVideoPlaneComponent.generated.h"

#define AR_PLANE_DISTANCE 32000

/**
 * A Videoplane, which renders the camera stream in the scene background
 */
UCLASS()
class UVuforiaVideoPlaneComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()
public:
	void Create();

	void Init(const FVector2D& Scale);

	void Update();

private:
	VuforiaVideoMaterial mVideoMaterial;
};
