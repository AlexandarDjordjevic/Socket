# Sockets

The Sockets library provides the foundations of encapsulating in C++ classes the POSIX Sockets. The purpose of this library is to simplify POSIX socket usage.

## Supported platforms / recommended toolchains

---

This is a C++11 library which depends only on the C++11 compiler and standard library.  
Supported platform are:

- Linux -- clang or gcc
- MacOS -- XCode (clang)

## Building

---

This project can stand alone or be included in larger projects. CMake files are included for your convenience to generate a build system to compile the source code and link them into programs you can run.

There are two distinct steps in the build process using CMake:

Generation of the build system, using CMake
Compiling, linking, etc., using CMake-compatible toolchain

## Prerequisites

---

CMake version 3.8 or newer
C++11 toolchain compatible with CMake for your development platform
