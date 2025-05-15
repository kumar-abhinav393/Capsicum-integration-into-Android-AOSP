# Google Capsicum-Test (Dev Branch) Analysis
(https://github.com/google/capsicum-test/tree/dev)

## Repository Structure and Supporting files
- The Google Capsicum-Test repository is a Capsicum user-space test suite. 
- It primarily targets FreeBSD (which natively supports Capsicum) and a modified Linux (via the Capsicum-Linux project).
- In the dev branch, the core capsicum library code resides in the 'libcaprights/' subdirectory.

### Key files in dev branch
- libcaprights/capsicum.c : Implements Capsicum system call wrappers and related helper functions (largely for Linux).
- libcaprights/capsicum.h : Header declaring the capsicum user-space API (types, constants, and function prototypes) for the library.
- capsicum_rights.h : Header that defines capability rights bits and provides compatibility macros to handle differences between FreeBSD versions.

### Additional Support files in libcaprights/:
- linux-bpf-capmode.c/.h : Implements 'cap_enter' and capability-mode sandboxing on Linux (using seccomp BPF and prctl).
- procdesc.c/.h : Implements process descriptors (pdfork, pdkill, etc) for platforms that support or emulate them.
- signal.c : Helpers for signal and process-handling (used by procdesc logic).

### OS-specific headers in the root directory:
- 'capsicum-freeBSD.h' and 'capsicum-linux.h' -- Setup macros and constants depending on the target OS.


