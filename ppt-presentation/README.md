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
#### 1. Why we need Capcisum oon Android
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
  - We need `cap_new()` to create a capability and `cap_enter()` to lock the process into this strict mode.

----------------------------------------------------------------------------------------------------------

### Stage 2 Steps - User Space
#### 1. Write the code
  - Drop two short C++ files in `libacapsicum.c` into a new libcapsicum folder: one wraps file descriptor(`cap_new(fd, rights)`), the other puts isolated process into capability mode with wrapped file descriptor (`cap_enter()`).
#### 2. Add an Android.bp file
  - Tell Soong how to compile the library and spit out `libcapsicum.so` with the right ABI flags.
#### 3. Create the JNI bridge
  - One small Java class loads the native lib and exposes `createCapabilityFd()`
----------------------------------------------------------------------------------------------------------



