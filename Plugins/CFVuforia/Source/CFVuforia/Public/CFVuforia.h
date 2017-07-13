/*
	Copyright(c) 2016-2017 Codefluegel GMBH All Rights Reserved.
*/
#pragma once
 
#include "ModuleManager.h"
 
class CFVuforiaImpl : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	void StartupModule();
	void ShutdownModule();
};