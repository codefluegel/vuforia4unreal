/*
    Copyright(c) 2016-2017 Codefluegel GMBH All Rights Reserved.
    Distributed under the BSD 3-Clause license.
*/

#pragma once

#include "LogMacros.h"

DECLARE_LOG_CATEGORY_EXTERN(CFVuforiaLog, Log, All);

#if PLATFORM_ANDROID
namespace Vuforia
{
	struct Matrix44F;
}
#endif

namespace CFVuforiaUtils
{
#if PLATFORM_ANDROID
	FMatrix glToUE(const Vuforia::Matrix44F& modelMatrix);
#endif
}

