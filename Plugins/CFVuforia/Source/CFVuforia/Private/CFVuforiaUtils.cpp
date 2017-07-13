/*
Copyright(c) 2016-2017 Codefluegel GMBH All Rights Reserved.
*/

#include "CFVuforiaPrivatePCH.h"
#include "CFVuforiaUtils.h"

DEFINE_LOG_CATEGORY(CFVuforiaLog);

#if PLATFORM_ANDROID
	#include <Vuforia/Matrices.h>
#endif

namespace CFVuforiaUtils
{
#if PLATFORM_ANDROID
	FMatrix glToUE(const Vuforia::Matrix44F& modelMatrix)
	{
		//TODO works but there is maybe a way nicer solution :-)
		FMatrix opengl2unreal(
			FPlane(0.f, 1.f, 0.f, 0.f),
			FPlane(1.f, 0.f, 0.f, 0.f),
			FPlane(0.f, 0.f, 1.f, 0.f),
			FPlane(0.f, 0.f, 0.f, 1.f));

		// left handed rotation matrix -90 around y
		FMatrix rot(
			FPlane(0.f, 0.f, 1.f, 0.f),
			FPlane(0.f, 1.f, 0.f, 0.f),
			FPlane(-1.f, 0.f, 0.f, 0.f),
			FPlane(0.f, 0.f, 0.f, 1.f));

		// copy the right handed opengl pose matrix to an unreal FMatrix
		FMatrix mModelMatrix(
			FPlane(modelMatrix.data[0], modelMatrix.data[4], modelMatrix.data[8], modelMatrix.data[12]),
			FPlane(modelMatrix.data[1], modelMatrix.data[5], modelMatrix.data[9], modelMatrix.data[13]),
			FPlane(modelMatrix.data[2], modelMatrix.data[6], modelMatrix.data[10], modelMatrix.data[14]),
			FPlane(modelMatrix.data[3], modelMatrix.data[7], modelMatrix.data[11], modelMatrix.data[15]));

		// right handed tracking target reference frame to left handed unreal tracking target reference frame
		FMatrix unrealModel = opengl2unreal.GetTransposed()*mModelMatrix*opengl2unreal;
		// move camera instead of the model, column major to unreal row major
		FMatrix cameraMatrix = unrealModel.Inverse().GetTransposed();
		// world rotation around 90 degrees to compensate the different camera reference frames
		FMatrix rotatedCamera = rot*cameraMatrix;

		// reset the location in order to perfrom just a local rotation around 90 degrees
		for (int i = 0; i < 4; i++) rotatedCamera.M[3][i] = cameraMatrix.M[3][i];

		return rotatedCamera;
	}
#endif
}