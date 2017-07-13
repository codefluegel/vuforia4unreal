/*
	Copyright(c) 2016-2017 Codefluegel GMBH All Rights Reserved.
    Distributed under the BSD 3-Clause license.
*/

#include "CFVuforiaPrivatePCH.h"
 
#include "CFVuforia.h"
#include "CFVuforiaSettings.h"

#include "Developer/Settings/Public/ISettingsModule.h"
#include "Developer/Settings/Public/ISettingsSection.h"

#define LOCTEXT_NAMESPACE "FCFVuforiaModule"

void CFVuforiaImpl::StartupModule()
{
	// register settings
    auto settingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if(settingsModule)
	{
		settingsModule->RegisterSettings("Project", "Plugins", "Vuforia",
			LOCTEXT("VuforiaSettingsName", "Vuforia"),
			LOCTEXT("VuforiaSettingsDescription", "Configure the Vuforia plug-in."),
			GetMutableDefault<UVuforiaSettings>());
	}
}
 
void CFVuforiaImpl::ShutdownModule()
{
    // unregister settings
    auto settingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if(settingsModule)
	{
		settingsModule->UnregisterSettings("Project", "Plugins", "Vuforia");
    }
}
 
IMPLEMENT_MODULE(CFVuforiaImpl, CFVuforia)