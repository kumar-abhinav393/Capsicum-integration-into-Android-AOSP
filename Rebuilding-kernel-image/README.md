# Kernel Side (new syscalls + Capsicum core)

## How to build Android 13 (android13-5.10) Generic Kernel Image(GKI) on Mac OS using Docker
Nothing here touches the full AOSP tree yet--we'll end up with a fresh kernel `boot.img` that later drops straight into an Android 13_r35 build.

### Install Pre-requisites
- Docker desktop (or Colima): https://docs.docker.com/desktop/mac/install/
- Homebrew Git (if not already ): `brew install git`

### Grab the Android-13 GKI source
  * `mkdir -p ~/android-kernel`
  * `cd ~/android-kernel`
- Clone just the kernel, not all of AOSP
  * `git clone https://android.googlesource.com/kernel/common -b android13-5.10`
  * `cd common`
- What we have now:
<img width="662" alt="image" src="https://github.com/user-attachments/assets/b78ebaba-3160-473c-9e04-4e3fc1dd2e6d" />

### Kick off thr Docker-driven build
Point `BUILD_CONFIG` at a ready-made "build.config.*" file, then run `build/build.sh`. The script spawns a container, mounts the source tree, fetches the right Clang/LLD, and compiles.

1. Build the x86-64 GKI (works in Cuttlefish/emu)
  * `export BUILD_CONFIG=common/build.config.gki.x86_64`     [from inside `common/`]
  * `export CCACHE_DIR=$HOME/.kernel_ccache`                 [speeds up the rebuilds (optional)]
  * `export DIST_DIR=$PWD/out/dist`                          [final artifacts land here (optional)]
  * `./build/build.sh`                                       [fire!]
 
- What happens:
  * `build/build.sh` notices it's on macOS -> automatically uses Docker.
  * Docker downloads the image `gcr.io/android-kernel/build:<hash>` [(~1.7 GB once)]
  * Inside the container it runs Kleaf/Bazel, Clang 14, etc
  * Objects go in `out/...`; final images land in `$DIST_DIR`

<img width="691" alt="image" src="https://github.com/user-attachments/assets/eedcc2a8-a915-41c0-8fb2-049810f2a60c" />

2. For arm64 (for Cuttlefish arm64)
  * `export BUILD_CONFIG=common/build.config.gki.aarch64`
  * `./build/build.sh`
  Everything else is identical. The products drop in the same `out/dist/`.

### Smoke-test the fresh kernel (optional but reassuring)
1. Boot in Cuttlefish (x86-64)
   * `cvd start --boot_image=path/to/boot.img`               [Still on mac: 1. copy boot.img somewhere convenient 2. Launch cuttlefish telling it to use boot.img]
   The emulator should come up normally.
   * SSH in (`adb shell`)
     `uname -a`                                              [Should show 5.10.x-gki-android13]

2. Verify the syscall numbers exist (will be NO-SYS yet)
   * `adb shell`
   * `strace -e trace=cap_new,cap_enter -- cat /dev/null`    [We should see `cap_new` unresolved -> ENOSYS (until we add Capsicum later)]



