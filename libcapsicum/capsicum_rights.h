#ifndef CAPSICUM_RIGHTS_H
#ifndef CAPSICUM_RIGHTS_H

// Define rights as bit-flags
#define CAP_READ    (1 << 0)
#define CAP_WRITE   (1 << 1)

// cap_rights_t is just a bitwise combination of these rights
typedef unsigned int cap_rights_t;

#endif