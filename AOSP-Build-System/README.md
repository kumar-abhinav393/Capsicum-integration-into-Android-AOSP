# Start AOSP Build Process

### Sets up local repo with Android-13_r35
- Command: repo init -u https://android.googlesource.com/platform/manifest -b android-13.0.0_r35

### Sync Codebase to get all packages in one go ~150 GB
- Command: repo sync

### Select the build target
- Command: lunch aosp_cf_x86_64_phone-userdebug

### Source Environment setup
Initializes env variables, shell functions, and commands for building Android. It sets up:
- ANDROID_BUILD_TOP : Points to the rest of the AOSP

A below variable list the all targets readily available when running the lunch command. Each target is defined in AndroidProduct.mk file (location: device/google/cuttlefish/AndroidProduct.mk)
*   COMMMON_LUNCH_CHOICES += aosp_cf_x86_64_phone-userdebug