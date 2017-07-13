/*
	Copyright(c) 2016-2017 Codefluegel GMBH All Rights Reserved.
    Distributed under the BSD 3-Clause license.
*/

#pragma once

/**
 * Helper to update the video plane texture
 */
class VuforiaVideoMaterial
{
public:
	VuforiaVideoMaterial();

	void Init(class UProceduralMeshComponent* Mesh);

	void CreateDynamic(class UProceduralMeshComponent* Mesh);

	void Setup();

	bool Initialized();

	void Update();
private:

	UMaterialInstanceDynamic* mDynamicMaterialInstance;
	UTexture2D *mDynamicTexture;
};
