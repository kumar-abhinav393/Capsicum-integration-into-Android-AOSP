# START AOSP BUILD PROCESS

### SETUP `repo`

  - `mkdir -p ~/bin`
  - `curl https://storage.googleapis.com/git-repo-downloads/repo > ~/bin/repo`
  - `chmod a+x ~/bin/repo`
  - `export PATH=~/bin:$PATH`
  
- Explanation: Creates a personal `bin` folder, downloads Google's repo helper, makes it executable, and puts that folder at the front of your `PATH`. Repo is y Python wrapper around Git that understands Android's multi-repo layout.

### CREATE A WORKSPACE
  - `mkdir ~/aosp-android13`
  - `cd ~/aosp-android13`
    
- Explanation: Makes a clean directory to hold the entire AOSP tree and moves into it.

### INTIIALIZE THE REPO
  - `repo init -u https://android.googlesource.com/platform/manifest -b android-13.0.0_r35`
    
  - Explanation: Tells the repo which manifest to use (the master list of Git projects) and which tag/branch to track.

### SYNC SOURCES
- `repo sync -c -j$(nproc)`

- Explanation: Downloads every Git project referenced by the manifest. `-c` pulls only the branch we initialized; `-j$(nproc)` runs as many parallel jobs as we have CPU cores for faster fetch. 

### LOAD BUILD HELPER FUNCTIONS
  - `source build/envsetup.sh`
  
  - Explanation: Populates our shell with dozens of build helpers (`lunch`, `m`, `croot`, etc).

### CHOOSE A BUILD TARGET
  - `lunch aosp-cf-x86_64_phone-userdebug`
  
  - Explanation: Selects the Cuttlefish x86-64 phone configuration in userdebug mode (debugging but with production-like SELinux and signature checks).

### VERIFY PRODUCT APPEARS IN ANDROIDPRODUCTS.mk
  - Explanation: Double-checks that `aosp-cf-x86_64_phone-userdebug` is listed in `COMMON_LUNCH_CHOICES`; guards against typos.

### KICK OFF THE BUILD
  - `m`
  
  - Explanation: Invokes the AOSP "mega-make" wrapper, building everything needed for our chosen product: platform, kernel (prebuilt for Cuttlefish),  images, OTA tools, etc.

----------------------------------------------------------------------

### WHAT IS `AndroidProducts.mk`?
It's a small makefile, usually found in each device or product directory (for Cuttlefish: `device/google/cuttlefish/AndroidProducts.mk`). Inside, it listed product definitions and tells the build system: "_Hey, these product names exist and map to these `.mk` files._"

### WHY LOOK INSIDE `AndroidProducts.mk` AT ALL?
 - The `Android-13.0.0_r35` manifest pulls hundreds of device and products directories. Inside each of those directories we'll usually find an `AndroidProducts.mk` file that registers one or more products for that family of devices.
 - When we type `source build/envsetup.sh`, the helper functions in that script do a global scan through the whole tree and parse the `AndroidProducts.mk` files.
 - So when we pick the Cuttlefish with phone-userdebug, what actually happens:
   - The scan saw `device/google/cuttlefish/AndroidProducts.mk`.
   - That file listed `aosp_cf_x86_64_phone.mk` in `PRODUCT_MAKEFILES` and the `...-usedebug` entry in `COMMON_LUNCH_CHOICES`.
   - The helper script added that lunch choice to the master list, so `lunch` could recognise it.
  
