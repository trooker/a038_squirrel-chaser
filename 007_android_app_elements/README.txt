Contents of folders shown require use of the MIT App2Invnetor development tool.  The "project" file for 
android-otg-wifi project is found in the 0007.002_android_gamepad_otg_wifi_a2i folder.  The two "key" 
extensions that must be imported are found in 007.01_import_extension_g038_serial.aix and 
007.04_import_extension_UrsAI2UDP.aix.  The "Build" of the project can then be executed and should 
yield the same Android Application Package as that found in 007.03_android_gaempad_otg_wifi_apk.

Uploading the Android APK file and installing it can be executed directly without the "Imports and Build"
if the user so elects since the Android will connect with the WIFI Access Point using the
authenication (name/passwoard pair) already set in the device's Settings.  The Android APK will 
establish the addresses and ports using default UDP values.  
