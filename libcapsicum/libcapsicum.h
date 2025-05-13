#ifndef LIBCAPSICUM_H
#define LIBCAPSICUM_H

#include <sys/types.h>         // for pid_t, etc.
#include <sys/capsicum.h>      // for cap_rights_t
#include <sys/caprights.h>     // for CAP_READ, CAP_WRITE, etc.
#include <asm/unistd.h>        // for __NR_cap_enter, __NR_cap_new

#ifdef __cplusplus
extern "C" {
#endif

#define SYS_cap_enter  __NR_cap_enter
#define SYS_cap_new    __NR_cap_new

int createCapabilityFd(int fd, int rights);
int enableCapabilityMode(void);

#ifdef __cplusplus
}
#endif

#endif  // LIBCAPSICUM_H