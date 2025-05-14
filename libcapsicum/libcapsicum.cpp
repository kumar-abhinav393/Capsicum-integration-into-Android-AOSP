// libcapsicum.cpp
//
// This file implements a minimal Capsicum API for Android,
// providing functions to create capability-wrapped file descriptors,
// manage a capability file descriptor list (fdlist), and
// transition a process into capability mode.
//
// Author: Abhinav
// Date: 07-Feb-2025
// Thesis: "To extend the isolated process feature in Android with file system and Binder IPC capabilities"
//         (Capsicum mode for Isolated Processes in Android)

#include "libcapsicum.h"
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unordered_map>
#include <mutex>
#include <android/log.h>
#define LOG_TAG "libcapsicum"
#ifndef ALOGE
#define ALOGE(fmt, ...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##__VA_ARGS__)
#define ALOGW(fmt, ...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, fmt, ##__VA_ARGS__)
#endif

namespace capsicum {

// -----------------------------------------------------------------------------
// Placeholder definitions for system call numbers.
// Replace these with the actual numbers provided by your kernel modifications.
#ifndef SYS_cap_new
#define SYS_cap_new 350   // Example placeholder value
#endif

#ifndef SYS_cap_enter
#define SYS_cap_enter 351 // Example placeholder value
#endif
// -----------------------------------------------------------------------------

// Global mutex to protect the fdlist map during concurrent access.
std::mutex fdListMutex;

// Global fdlist: a mapping between process-local names and capability-wrapped file descriptors.
static std::unordered_map<std::string, int> fdList;

/**
 * @brief Wraps an existing file descriptor with capability restrictions.
 *
 * This function calls the cap_new system call to create a capability-wrapped
 * file descriptor from an existing file descriptor `fd` with specified `rights`.
 *
 * @param fd The existing file descriptor to wrap.
 * @param rights An integer representing the rights/permissions to associate with the capability.
 * @return On success, returns the new capability-wrapped file descriptor.
 *         On failure, returns -1.
 */
int createCapabilityFd(int fd, int rights) {
    // Invoke the cap_new system call.
    int capFd = syscall(SYS_cap_new, fd, rights);
    if (capFd == -1) {
        ALOGE("cap_new failed for fd %d with rights %d: %s",
              fd, rights, strerror(errno));
        return -1;
    }
    return capFd;
}

/**
 * @brief Adds a capability-wrapped file descriptor to the fdlist.
 *
 * This function associates a process-local name with the given capability-wrapped file
 * descriptor, so that later the capability can be looked up using the name.
 *
 * @param name The process-local name to associate with the file descriptor.
 * @param capFd The capability-wrapped file descriptor.
 * @return true on success, false if the name already exists or another error occurred.
 */
bool addCapabilityFd(const std::string &name, int capFd) {
    std::lock_guard<std::mutex> lock(fdListMutex);
    // Check if the name is already used.
    if (fdList.find(name) != fdList.end()) {
        ALOGE("Capability name '%s' already exists in fdList.", name.c_str());
        return false;
    }
    fdList[name] = capFd;
    return true;
}

/**
 * @brief Retrieves a capability-wrapped file descriptor by its process-local name.
 *
 * @param name The process-local name used when adding the capability.
 * @return The file descriptor if found; otherwise, -1.
 */
int getCapabilityFd(const std::string &name) {
    std::lock_guard<std::mutex> lock(fdListMutex);
    auto it = fdList.find(name);
    if (it == fdList.end()) {
        ALOGE("Capability name '%s' not found in fdList.", name.c_str());
        return -1;
    }
    return it->second;
}

/**
 * @brief Removes and closes a named capability FD from the fdList.
 *
 * @param name The name of the capability to remove.
 * @return true if removed, false if not found or error.
 */
bool removeCapabilityFd(const std::string &name) {
    std::lock_guard<std::mutex> lock(fdListMutex);
    auto it = fdList.find(name);
    if (it == fdList.end()) {
        ALOGE("removeCapabilityFd: Capability name '%s' not found", name.c_str());
        return false;
    }
    close(it->second);
    fdList.erase(it);
    return true;
}

/**
 * @brief Transitions the current process into capability mode.
 *
 * Once in capability mode, the process is restricted from accessing global namespaces;
 * only the capabilities explicitly passed to it will be available.
 *
 * @return 0 on success; -1 on failure.
 */
int enableCapabilityMode() {
    int ret = syscall(SYS_cap_enter);
    if (ret == -1) {
        ALOGE("cap_enter failed: %s", strerror(errno));
        return -1;
    }
    return 0;
}

} // namespace capsicum