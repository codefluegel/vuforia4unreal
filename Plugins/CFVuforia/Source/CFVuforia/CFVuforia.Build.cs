/*
	Copyright(c) 2016-2017 Codefluegel GMBH All Rights Reserved.
    Distributed under the BSD 3-Clause license.
*/

using UnrealBuildTool;
using System.IO;

public class CFVuforia : ModuleRules
{
	public CFVuforia(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "RHI", "RenderCore" });

		PrivateDependencyModuleNames.AddRange(new string[] { "ProceduralMeshComponent" });

		PrivateIncludePaths.Add(PrivateIncludePath);

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
			PrivateIncludePaths.Add(Path.Combine(VuforiaSDKPath, "build", "include"));
            PublicAdditionalLibraries.Add(Path.Combine(VuforiaSDKPath, "build", "lib", "armeabi-v7a", "libVuforia.so"));

            AdditionalPropertiesForReceipt.Add(new ReceiptProperty("AndroidPlugin", Path.Combine(ModulePath, "Vuforia.xml")));
        }
    }

    private string ModulePath
    {
        get
        {
            return ModuleDirectory;
        }
    }

    private string PrivateIncludePath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "Private")); }
    }

    private string VuforiaSDKPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../VuforiaSDK")); }
    }
}
