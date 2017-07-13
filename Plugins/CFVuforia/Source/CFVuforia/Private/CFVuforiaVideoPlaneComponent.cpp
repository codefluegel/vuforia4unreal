/*
	Copyright(c) 2016-2017 Codefluegel GMBH All Rights Reserved.
*/

#include "CFVuforiaPrivatePCH.h"
#include "CFVuforiaVideoPlaneComponent.h"
#include "CFVuforiaActorComponent.h"

#if PLATFORM_ANDROID
	#include <Vuforia/RenderingPrimitives.h>
	#include <Vuforia/Device.h>
	#include <Vuforia/Vectors.h>
#endif
void UVuforiaVideoPlaneComponent::Create()
{
	TArray<FVector> vertices;
#define WIDTH 0.5
	vertices.Add(FVector(0, -WIDTH, -WIDTH));
	vertices.Add(FVector(0, WIDTH, WIDTH));
	vertices.Add(FVector(0, -WIDTH, WIDTH));
	vertices.Add(FVector(0, -WIDTH, -WIDTH));
	vertices.Add(FVector(0, WIDTH, -WIDTH));
	vertices.Add(FVector(0, WIDTH, WIDTH));

	TArray<FVector2D> UV0;
	UV0.Add(FVector2D(0, 1));
	UV0.Add(FVector2D(1, 0));
	UV0.Add(FVector2D(0, 0));
	UV0.Add(FVector2D(0, 1));
	UV0.Add(FVector2D(1, 1));
	UV0.Add(FVector2D(1, 0));

	TArray<int32> Triangles;
	for (int32 i = 0; i < 6; i++) Triangles.Add(i);

	TArray<FVector> normals;
	for (int32 i = 0; i < 6; i++) normals.Add(FVector(-1, 0, 0));

	TArray<FColor> vertexColors;
	for (int32 i = 0; i < 6; i++) vertexColors.Add(FColor(255, 100, 100, 100));

	TArray<FProcMeshTangent> tangents;
	for (int32 i = 0; i < 6; i++) tangents.Add(FProcMeshTangent(1, 1, 1));

	this->CreateMeshSection(0, vertices, Triangles, normals, UV0, vertexColors, tangents, false);

	// assign Material and Video texture
	mVideoMaterial.Init(this);
}

static void updatePlane(UVuforiaVideoPlaneComponent* comp) {
#if PLATFORM_ANDROID
	Vuforia::RenderingPrimitives* renderingPrimitives = new Vuforia::RenderingPrimitives(Vuforia::Device::getInstance().getRenderingPrimitives());

	const Vuforia::Mesh& vbMesh = renderingPrimitives->getVideoBackgroundMesh(Vuforia::VIEW_SINGULAR);
	int verticesCount = vbMesh.getNumVertices();
	const Vuforia::Vec3F* verts = vbMesh.getPositions();
	const Vuforia::Vec2F* uvs = vbMesh.getUVs();
	const unsigned short* triangles = vbMesh.getTriangles();
#define WIDTH 0.5
	TArray<FVector> vertices;
	TArray<FVector2D> UV0;
	TArray<FVector> normals;
	TArray<FColor> vertexColors;
	TArray<FProcMeshTangent> tangents;
	for (int32 i = 0; i < verticesCount; i++) {
		vertices.Add(FVector(0, verts[i].data[0]*WIDTH, verts[i].data[1]*WIDTH));
		UV0.Add(FVector2D(uvs[i].data[0], uvs[i].data[1]));
		normals.Add(FVector(-1, 0, 0));
		vertexColors.Add(FColor(255, 100, 100, 100));
		tangents.Add(FProcMeshTangent(1, 1, 1));
	}

	int triangleVertCount = vbMesh.getNumTriangles() * 3;

	TArray<int32> Triangles;
	for (int32 i = 0; i < triangleVertCount; i++) {
		Triangles.Add(triangles[i]);
	}
	comp->ClearMeshSection(0);
	comp->CreateMeshSection(0, vertices, Triangles, normals, UV0, vertexColors, tangents, false);
#endif
}

void UVuforiaVideoPlaneComponent::Init(const FVector2D& Scale)
{
	updatePlane(this);
	UE_LOG(CFVuforiaLog, Warning, TEXT("UVuforiaVideoPlaneComponent::Init %f, %f"), Scale.X, Scale.Y);
	this->SetRelativeLocation(FVector(AR_PLANE_DISTANCE, 0, 0));
	this->SetWorldScale3D(FVector(0, Scale.X, Scale.Y));
	mVideoMaterial.CreateDynamic(this);
}

void UVuforiaVideoPlaneComponent::Update()
{
#if PLATFORM_ANDROID
	if (!mVideoMaterial.Initialized())
	{
		mVideoMaterial.Setup();
	}
	else
	{
		mVideoMaterial.Update();
	}
#endif
}