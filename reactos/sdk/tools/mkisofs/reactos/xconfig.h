/*
 * PROJECT:  MKISOFS for the ReactOS Build System
 * LICENSE:  GNU GPLv2 as published by the Free Software Foundation
 * AUTHORS:  Colin Finck <colin@reactos.org>
 */

/*
 * When mkisofs is part of Schily tools, this file is generated by the Schily Build System
 * and checks the existence of hundreds of headers and functions.
 * For the ReactOS Build System, we provide a written version of this file with sufficient
 * information to build mkisofs and universal among our supported build platforms.
 */

#ifndef __XCONFIG_H
#define __XCONFIG_H

/*
 * Header Files
 */
#define PROTOTYPES 1
#define HAVE_STDARG_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STDDEF_H 1
#define HAVE_STRING_H 1
#define STDC_HEADERS 1

#ifndef WIN32
    #define HAVE_UNISTD_H 1
#endif

#define HAVE_LIMITS_H 1
#define HAVE_FCNTL_H 1

#ifdef WIN32
    #define HAVE_IO_H 1
#endif

#define HAVE_INTTYPES_H 1
#define HAVE_STDINT_H 1

#ifndef WIN32
    #define HAVE_DIRENT_H 1
    #define HAVE_SYS_TIME_H 1
    #define TIME_WITH_SYS_TIME 1
#endif

#define HAVE_SYS_TYPES_H 1
#define HAVE_MATH_H 1
#define HAVE_ERRNO_DEF 1

#ifdef WIN32
    #define HAVE_WINDOWS_H 1
    #define WBOOL WINBOOL
#endif

#define HAVE_LOCALE_H 1
#define HAVE_CTYPE_H 1
#define HAVE_WCTYPE_H 1
#define HAVE_WCHAR_H 1

/*
 * Convert to SCHILY name
 */
#define HAVE_STDC_HEADERS

/*
 * Library Functions
 */
#define HAVE_STRERROR 1
#define HAVE_MEMCHR 1
#define HAVE_MEMCPY 1
#define HAVE_MEMMOVE 1
#define HAVE_MEMSET 1

#ifndef WIN32
    #define HAVE_GETTIMEOFDAY 1
#endif

#define HAVE_TIME 1
#define HAVE_VA_COPY 1
#define HAVE_ECVT 1
#define HAVE_FCVT 1
#define HAVE_GCVT 1
#define HAVE_C99_ISINF 1
#define HAVE_C99_ISNAN 1

#define HAVE_SNPRINTF 1
#ifdef _MSC_VER
    #define snprintf _snprintf
#endif

#define HAVE_STRCAT 1
#define HAVE_STRNCAT 1
#define HAVE_STRCMP 1
#define HAVE_STRNCMP 1
#define HAVE_STRCPY 1

#ifdef __APPLE__
    #define HAVE_STRLCAT 1
    #define HAVE_STRLCPY 1
#endif

#define HAVE_STRNCPY 1
#define HAVE_STRDUP 1
#define HAVE_STRLEN 1
#define HAVE_STRCHR 1
#define HAVE_STRRCHR 1
#define HAVE_STRSTR 1
#define HAVE_STRCASECMP 1
#define HAVE_STRNCASECMP 1

#ifndef WIN32
    #define HAVE_USLEEP 1
#endif

#define HAVE_WCSCAT 1
#define HAVE_WCSNCAT 1
#define HAVE_WCSCMP 1
#define HAVE_WCSNCMP 1
#define HAVE_WCSCPY 1

#ifdef __APPLE__
    #define HAVE_WCSLCAT 1
    #define HAVE_WCSLCPY 1
#endif

#define HAVE_WCSNCPY 1
#define HAVE_WCSDUP 1
#define HAVE_WCSNDUP 1
#define HAVE_WCSLEN 1
#define HAVE_WCSNLEN 1
#define HAVE_WCSCHR 1
#define HAVE_WCSRCHR 1
#define HAVE_WCSSTR 1

#ifndef WIN32
    #define HAVE_WCWIDTH 1
#endif

#define HAVE_SETVBUF 1
#define HAVE_ISWPRINT 1
#define HAVE_MBSINIT 1
#define HAVE_MBTOWC 1
#define HAVE_WCTOMB 1
#define HAVE_MBRTOWC 1
#define HAVE_WCRTOMB 1

#if !defined(WIN32) && !defined(__APPLE__)
    #define HAVE_RAW_FEXECVE 1
#endif

/*
 * Misc OS stuff
 */
#ifndef WIN32
    #define HAVE__DEV_NULL 1
#endif

/*
 * Structures
 */
#ifndef WIN32
    #define HAVE_STRUCT_TIMEVAL 1
#endif

#define HAVE_STRUCT_TIMEZONE 1
#define HAVE_STRUCT_TIMESPEC 1

/*
 * Types/Keywords
 */
#define HAVE_LONGLONG 1

#ifndef WIN32
    #define VA_LIST_IS_ARRAY 1
#endif

#ifdef WIN32
    #define uid_t int
    #define gid_t int
    #define nlink_t unsigned long
    
    #ifdef _MSC_VER
        #define mode_t int
    #endif
#endif

#define HAVE_TYPE_INTMAX_T 1
#define HAVE_TYPE_UINTMAX_T 1

/*
 * ReactOS additions
 */
#ifdef _MSC_VER
    #define ssize_t int
#endif

/* Would need additional fprformat.c, less portable */
#define NO_FPRFORMAT

/* For a nice "mkisofs -version" :) */
#include <reactos/buildno.h>
#define HOST_CPU "ReactOS"
#define HOST_VENDOR KERNEL_VERSION_BUILD_STR
#define HOST_OS REACTOS_COMPILER_NAME "-" REACTOS_COMPILER_VERSION

#endif
