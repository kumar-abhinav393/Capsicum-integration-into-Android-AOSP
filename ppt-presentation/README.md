# PowerPoint Deck

### Road Map to build a PowerPoint deck
#### Title & Speaker
  - Thesis topic
  - Name & Matriculation Number
  - Supervisor

#### Agenda
  - Motivation
  - Two integration stages
  - Dev branch source code
  - Bottom-to-top workflow
  - Test plan & next steps

#### Why Capsicum for Android?
  - Over-privilege in isolated services
  - Need for least privilege
  - Capsicum's proven model

#### High-level Architecture
  - Zygote hook
  - `libcapsicum` in user space
  - New syscalls in kernel

#### Stage 1 Steps
  1. Add Android.bp file
  2. Write thin C/C++ wrappers
  3. Implement JNI bridge
  4. Build & flash image
  5. Smoke test with an app (Image Editing App)

#### Stage 2 Overview - Kernel
  - Patch in `cap_new()`, `cap_enter()`
  - Import core logic from Capsicum-test dev (dev branch)

#### Stage 2 Steps
  - Cherry-pick Capsicum core
  - Updates headers & Structs
  - Add entries to syscall table
  - Rebuild kernel
  - Run "hello-cap" smoke test

#### Google capsicum-test (dev branch)
  - 2010 research code
  - Contains reference C implementation & unit tests
  - What we use: syscall handlers & rights macros
  - What we rewrite

#### Bottom-to-Top Workflow
  1. Kernel first: bulld + smoke test
  2. User-space library on vanilla AOSP
  3. Zygote hook & manifest intgration
  4. End-to-End tests with isolated image service


----------------------------------------------------------------------------------------------------------

### Agenda - What We'll Cover Today
#### 1. Why we need Capsicum on Android
  - A quick recap of the over-privilege problem in isolated services and why a capability model is the right fix.

#### 2. Three Integration Stages in a Nutshell
  - Stage 1: Runtime Wiring (Patch Zygote + invoke cap_enter())
  - Stage 2: User-space work (Soong module + JNI wrapper)
  - Stage 3: kernel work (new syscalls + Capsicum Core [cap_new() & cap_enter()])

#### 3. Where the code comes from
  - A brief tour of Google's capsicum-test dev branch and specific bits we're re-using.

#### 4. Bottom-to-Top Implementation Flow
  - Start with a patched kernel, add the user-space library, then wire both together through Zygoze--each step smoke-tested before moving on.

#### 5. Validation
  - Security and performance checks, an image-processing demo, and the milestones that take us to a finished thesis.

----------------------------------------------------------------------------------------------------------

### Why Capsicum for Android?
#### 1. Helpers get more power that they need: 
  - Isolated services still inherit broad file-system and Binder access from the main app. One bug can leak data or tamper with user files.
#### 2. Capsicum trims the fat: 
  - Capsicum's idea is simple--wrap each file descriptor in a capability that lists exactly what actions are allowed -- nothing more. If the helper tries anything outside that list, the kernel blocks it on the spot.
#### Two tiny syscalls, big payoff: 
  - We need `cap_new()` to create a capability-wrapped fd and `cap_enter()` to lock the process into a strict capability mode.

----------------------------------------------------------------------------------------------------------

### Stage 3 Steps - Kernel
#### 1. Separate directory 'capsicum/'
  - Firstly create a separate directory 'capsicum' to store files(capsicum.c, Kconfig, Makefile)
#### 2. Syscall placeholders
  - Add the two new entries to the kernel's syscall table.
#### 3. Bring in the Capsicum code
  - Cherry pick the `cap_new()` and `cap_enter()` sources(specific to Linux kernel) from Google's capsicum-test dev branch.
#### 3. Generic system call implementation
  - Define syscalls entry point by creating macros with cherry picked code for both syscalls in 'capsicum.c' file.
#### 4. Hook up the syscalls
  - Hook into the generic syscall table of both 64 and 32 bit generic and provide a fallback stub to compile them.
#### 5. Make it configurable
  - Introduce a `CONFIG_<FEATURE>_SYSCALL` option by creating a Kconfig and gaurd the implementation in Makefile.
#### 6. Run a quick smoke test
  - Execute a tiny 'hello-cap' program to wrap a fd with `cap_new(()` and drops into capability mode with `cap_enter()`.

----------------------------------------------------------------------------------------------------------

### Stage 2 Steps - User Space
#### 1. Make a `libcapsicum` directory
  - Keeps all user-space code in oone neat place.
#### 2. Drops in an `Android.bp` file
  - Tell Soong how to compile the library and spit out `libcapsicum.so` with the right ABI flags.
#### 3. Write `libcapsicum.h`
  - Public header that declares `createCapabilityFd()` and `enable_Capability_Mode()` so any AOSP module can include them.
#### 4. Write two C/C++ helpers
  - `createCapabilityFd()` calls `cap_new()`; `enable_Capability_Mode()` calls `cap_enter()` in `libcapsicum.cpp` file.
#### 5. Create the JNI bridge
  - One Java class loads the .so and exposes the two helpers to Kotlin/Java apps.
#### 6. Link a sample isolated service
  - In the service's `onCreate()`, wrap its file handle and drops into capability mode.
#### 7. Re-build & Flash AOSP
  - Library appears under `/system/lib`; the sample app installs cleanly.
#### 8. Run a quick smoke test
  - Wrapped reads succeed; any unwrapped access fails--proof that the user-space wiring works.

----------------------------------------------------------------------------------------------------------

### Stage 1 Steps - Zygote hook

----------------------------------------------------------------------------------------------------------

### Capsicum-test - dev Branch
#### Purpose & Age
  - Holds all the cross-platform unit tests that exercise capability mode on both FreeBSD and the old Capsicum-Linux prototype.
  - First pulled together in the mid 2010s, but the code base traces back to the 2011-2012 FreeBSD 9.x and Capsicum-Linux work.
  - Still receives tweaks (latest FreeBSD import landed 2022).



