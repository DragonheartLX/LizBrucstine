#pragma once

// OS
#if defined(WIN64) || defined(_WIN64)
    #define LIZ_OS_WIN64
    #define LIZ_OS_WIN32
#elif defined(WIN32) || defined(_WIN32)
    #define LIZ_OS_WIN32
#elif defined(ANDROID) || defined(__ANDROID__)
    #define LIZ_OS_ANDROID
    #define LIZ_OS_LINUX
#elif defined(linux) || defined(__linux) || defined(__linux__)
    #define LIZ_OS_LINUX
#elif defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
    #include <TargetConditionals.h>
    #if defined(TARGET_OS_MAC) && TARGET_OS_MAC
        #include <AvailabilityMacros.h>
        #define LIZ_OS_MAC
    #elif defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
        #define LIZ_OS_IOS
    #endif
    #define LIZ_OS_DARWIN
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
    #define LIZ_OS_FREEBSD
    #define LIZ_OS_BSD
#elif defined(__NetBSD__)
    #define LIZ_OS_NETBSD
    #define LIZ_OS_BSD
#elif defined(__OpenBSD__)
    #define LIZ_OS_OPENBSD
    #define LIZ_OS_BSD
#elif defined(sun) || defined(__sun) || defined(__sun__)
    #define LIZ_OS_SOLARIS
#else
    #warning "Untested operating system platform!"
#endif

#if defined(LIZ_OS_WIN32) || defined(LIZ_OS_WIN64)
    #undef LIZ_OS_UNIX
    #define LIZ_OS_WIN
#else
    #undef LIZ_OS_WIN
    #define LIZ_OS_UNIX
#endif

// ARCH
#if defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(_M_X64)
    #define LIZ_ARCH_X64
    #define LIZ_ARCH_X86_64
#elif defined(__i386) || defined(__i386__) || defined(_M_IX86)
    #define LIZ_ARCH_X86
    #define LIZ_ARCH_X86_32
#elif defined(__aarch64__) || defined(__ARM64__) || defined(_M_ARM64)
    #define LIZ_ARCH_ARM64
#elif defined(__arm__) || defined(_M_ARM)
    #define LIZ_ARCH_ARM
#elif defined(__mips64__)
    #define LIZ_ARCH_MIPS64
#elif defined(__mips__)
    #define LIZ_ARCH_MIPS
#elif defined(__riscv)
    #define LIZ_ARCH_RISCV
#elif defined(__ppc64__) || defined(__powerpc64__)
    #define LIZ_ARCH_PPC64
#elif defined(__ppc__) || defined(__powerpc__)
    #define LIZ_ARCH_PPC
#else
    #warning "Untested hardware architecture!"
#endif

// COMPILER
#if defined(_MSC_VER)
    #define LIZ_COMPILER_MSVC

    #if (_MSC_VER < 1200) // Visual C++ 6.0
        #define MSVS_VERSION 1998
        #define MSVC_VERSION 60
    #elif (_MSC_VER >= 1200) && (_MSC_VER < 1300) // Visual Studio 2002, MSVC++ 7.0
        #define MSVS_VERSION 2002
        #define MSVC_VERSION 70
    #elif (_MSC_VER >= 1300) && (_MSC_VER < 1400) // Visual Studio 2003, MSVC++ 7.1
        #define MSVS_VERSION 2003
        #define MSVC_VERSION 71
    #elif (_MSC_VER >= 1400) && (_MSC_VER < 1500) // Visual Studio 2005, MSVC++ 8.0
        #define MSVS_VERSION 2005
        #define MSVC_VERSION 80
    #elif (_MSC_VER >= 1500) && (_MSC_VER < 1600) // Visual Studio 2008, MSVC++ 9.0
        #define MSVS_VERSION 2008
        #define MSVC_VERSION 90
    #elif (_MSC_VER >= 1600) && (_MSC_VER < 1700) // Visual Studio 2010, MSVC++ 10.0
        #define MSVS_VERSION 2010
        #define MSVC_VERSION 100
    #elif (_MSC_VER >= 1700) && (_MSC_VER < 1800) // Visual Studio 2012, MSVC++ 11.0
        #define MSVS_VERSION 2012
        #define MSVC_VERSION 110
    #elif (_MSC_VER >= 1800) && (_MSC_VER < 1900) // Visual Studio 2013, MSVC++ 12.0
        #define MSVS_VERSION 2013
        #define MSVC_VERSION 120
    #elif (_MSC_VER >= 1900) && (_MSC_VER < 1910) // Visual Studio 2015, MSVC++ 14.0
        #define MSVS_VERSION 2015
        #define MSVC_VERSION 140
    #elif (_MSC_VER >= 1910) && (_MSC_VER < 1920) // Visual Studio 2017, MSVC++ 15.0
        #define MSVS_VERSION 2017
        #define MSVC_VERSION 150
    #elif (_MSC_VER >= 1920) && (_MSC_VER < 2000) // Visual Studio 2019, MSVC++ 16.0
        #define MSVS_VERSION 2019
        #define MSVC_VERSION 160
    #endif

#elif defined(__GNUC__)
    #define LIZ_COMPILER_GCC

#elif defined(__clang__)
    #define LIZ_COMPILER_CLANG

#elif defined(__MINGW32__) || defined(__MINGW64__)
    #define LIZ_COMPILER_MINGW

#elif defined(__MSYS__)
    #define LIZ_COMPILER_MSYS

#elif defined(__CYGWIN__)
    #define LIZ_COMPILER_CYGWIN

#else
    #warning "Untested compiler!"
#endif

#ifdef LIZ_OS_WIN64
    #ifdef LIZ_BUILD_SHARED
        #ifdef LIZ_BUILD_DLL
            #define LIZ_API __declspec(dllexport)
        #else
            #define LIZ_API __declspec(dllimport)
        #endif
    #else
        #define LIZ_API
    #endif
#else
    #define LIZ_API __attribute__((visibility("default")))
#endif