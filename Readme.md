Vuforia4Unreal
=============

Vuforia for Unreal is a Plugin which integrates the tracking functionality of the Vuforia AR SDK with the rendering power of Unreal Engine 4.

Installation
--------
### Dependencies
- [Unreal Engine 4.16.2](https://www.unrealengine.com) with [Android Platform](https://docs.unrealengine.com/latest/INT/Platforms/Android/GettingStarted/) set up 
- [Vuforia Android SDK 6.2.10](https://developer.vuforia.com/downloads/sdk)

### Build instructions
- Clone this Repository
- Copy the content of the [Vuforia SDK (vuforia-sdk-android-6-2-10)](https://developer.vuforia.com/downloads/sdk) inside the `Plugins/CFVuforia/VuforiaSDK` folder
- Open the Project, go to `Project Settings` > `Plugins > Vuforia` and insert your [Vuforia License Key](https://developer.vuforia.com/license-manager)
- Launch the Project on an Android device

How to integrate into your own Project
--------
- Copy the CFVuforia Plugin inside your Project's Plugin folder
- Copy `BlankTexture.uasset` and `M_DynamicTexture.uasset` from the PLugins Content folder to the Project Content folder
- Copy the content of the [Vuforia SDK (vuforia-sdk-android-6-2-10)](https://developer.vuforia.com/downloads/sdk) inside the `Plugins/CFVuforia/VuforiaSDK` folder
- Open your Project, drop the Vuforia Actor in the World and be sure to select "Player 0" in the Auto Player Activation select box.
- Open the Project, go to `Project Settings` > `Plugins > Vuforia` and insert your [Vuforia License Key](https://developer.vuforia.com/license-manager)

Troubleshooting
--------
### Build failed
- Ensure that you have copied the Vuforia SDK inside the Plugins VuforiaSDK folder

### Black screen
- Ensure you have set your Vuforia License Key in the Project Settings
- Ensure you have copied the two Assets (BlankTexture.uasset and M_DynamicTexture.uasset) inside your Project Content folder
- Remove the Vuforia Actor from the World and readd it to the World, don't forget to set the "Auto Player Activation" to "Player 0"

### No Content shown on the Target
- If you can see the Camera stream, but your Content is not shown on the Target (stonesandchips), be sure that your Content is placed at 0,0,0 in World coordinates. The Plugin currently only supports one Target, whos Content needs to be in the middle of the World.
- Ensure that you react on the TrackingLost/Found Events (for an example how to use them look at the level blueprint in the project of this Repository)

Copyright and License
--------
Vuforia4Unreal is provided under the [BSD-3-Clause](https://opensource.org/licenses/BSD-3-Clause).

Copyright 2016-2017 [Georg Vienna](https://github.com/geovie), [Elias Tappeiner](https://github.com/elitap) @ [Codeflügel GmbH](https://github.com/codefluegel)

All product names, logos, and brands are property of their respective owners. All company, product and service names used are for identification purposes only. Use of these names, logos, and brands does not imply endorsement.