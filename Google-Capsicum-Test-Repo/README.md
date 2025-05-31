# Google Capsicum-Test (Dev Branch) Analysis
(https://github.com/google/capsicum-test/tree/dev)

The Capsicum-Test repository's dev branch contains a mix of source files and platform-specific headers, plus a subdirectory for a Linux compatibility library. Key components include:
- Platform headers: `capsicum-freebsd.h` and `capsicum-linux.h` (included via a unified `capsicum.h`). These define OS-specific constants feature meacros.
- Portability layer: `capsicum.h` and `capsicum-rights.h` - provide a consistent Capsicum API across FreeBSD and Linux.
- Test framework: `capsicum-test.h` (macros/utilities for Google Test) and `capsicum-test-main.cc` (the main function and test environment setup).
- Capsicum userspace library (Linux only): The `libcaprights/` subdirectory contains C files (e.g., `capsicum.c`, `procdesc.c`, etc) implementing Capsicum syscalls for Linux userland. This is built as lbrary (`libcaprights`) on Linux systems.
- Test case sources: Numerous `.cc` (and a few `.c`) files implement the actual unit tests. Each focuses on a subsystem: e.g., file descriptors and rights (`capability-fd*.cc`, `fcntl.cc`, `ioctl.cc`), capability mode behaviour (`capmode.cc`, `smoketest.c`)


