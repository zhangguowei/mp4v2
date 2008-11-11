#ifndef MP4V2_PLATFORM_POSIX_H
#define MP4V2_PLATFORM_POSIX_H

///////////////////////////////////////////////////////////////////////////////

#include "libplatform/config.h"

///////////////////////////////////////////////////////////////////////////////

// constant macros are not usually used with C++ so the standard mechanism to
// activate it is to define before stdint.h
#ifndef __STDC_CONSTANT_MACROS
#   define __STDC_CONSTANT_MACROS
#endif

// format macros are not usually used with C++ so the standard mechanism to
// activate it is to define before inttypes.h
#ifndef __STDC_FORMAT_MACROS
#   define __STDC_FORMAT_MACROS
#endif

#ifdef NEED_LFS_ACTIVATION
#   ifndef _LARGEFILE_SOURCE
#       define _LARGEFILE_SOURCE
#       define _FILE_OFFSET_BITS 64
#   endif
#endif

///////////////////////////////////////////////////////////////////////////////

#include <string>

#include <cassert>
#include <cctype>
#include <cerrno>
#include <climits>
#include <cmath>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cwchar>
#include <cwctype>

#include <inttypes.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

#include <mp4v2/mp4v2.h>

///////////////////////////////////////////////////////////////////////////////

namespace mp4v2 { namespace platform {
    using namespace std;

    using ::int8_t;
    using ::int16_t;
    using ::int32_t;
    using ::int64_t;

    using ::uint8_t;
    using ::uint16_t;
    using ::uint32_t;
    using ::uint64_t;
}} // namespace mp4v2::platform

///////////////////////////////////////////////////////////////////////////////

#define MP4V2_PATH_MAX PATH_MAX

///////////////////////////////////////////////////////////////////////////////

// win32 platform requires O_BINARY when using old open() calls so we add
// this harmless bit-flag for posix to avoid .cpp platform conditionals
#ifndef O_BINARY
#   define O_BINARY  0
#endif

///////////////////////////////////////////////////////////////////////////////

#endif // MP4V2_PLATFORM_POSIX_H