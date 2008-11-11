/*
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is MPEG4IP.
 *
 * The Initial Developer of the Original Code is Cisco Systems Inc.
 * Portions created by Cisco Systems Inc. are
 * Copyright (C) Cisco Systems Inc. 2001.  All Rights Reserved.
 *
 * Contributor(s):
 *      Dave Mackie     dmackie@cisco.com
 */

#ifndef MP4V2_IMPL_MP4UTIL_H
#define MP4V2_IMPL_MP4UTIL_H

namespace mp4v2 { namespace impl {

///////////////////////////////////////////////////////////////////////////////

#include <assert.h>

#define LIBMPV42_STRINGIFY(x)  #x

#ifndef ASSERT
#   define ASSERT(expr) \
        if (!(expr)) { \
            throw new MP4Error("assert failure", LIBMPV42_STRINGIFY((expr))); \
        }
#endif

#define WARNING(expr) \
    if (expr) { \
        fflush(stdout); \
        fprintf(stderr, "Warning (%s) in %s at line %u\n", \
            LIBMPV42_STRINGIFY(expr), __FILE__, __LINE__); \
    }

#define VERBOSE(exprverbosity, verbosity, expr) \
    if (((exprverbosity) & (verbosity)) == (exprverbosity)) { expr; }

#define VERBOSE_ERROR(verbosity, expr)      \
    VERBOSE(MP4_DETAILS_ERROR, verbosity, expr)

#define VERBOSE_WARNING(verbosity, expr)        \
    VERBOSE(MP4_DETAILS_WARNING, verbosity, expr)

#define VERBOSE_READ(verbosity, expr)       \
    VERBOSE(MP4_DETAILS_READ, verbosity, expr)

#define VERBOSE_READ_TABLE(verbosity, expr) \
    VERBOSE((MP4_DETAILS_READ | MP4_DETAILS_TABLE), verbosity, expr)

#define VERBOSE_READ_SAMPLE(verbosity, expr)    \
    VERBOSE((MP4_DETAILS_READ | MP4_DETAILS_SAMPLE), verbosity, expr)

#define VERBOSE_READ_HINT(verbosity, expr)  \
    VERBOSE((MP4_DETAILS_READ | MP4_DETAILS_HINT), verbosity, expr)

#define VERBOSE_WRITE(verbosity, expr)      \
    VERBOSE(MP4_DETAILS_WRITE, verbosity, expr)

#define VERBOSE_WRITE_TABLE(verbosity, expr)    \
    VERBOSE((MP4_DETAILS_WRITE | MP4_DETAILS_TABLE), verbosity, expr)

#define VERBOSE_WRITE_SAMPLE(verbosity, expr)   \
    VERBOSE((MP4_DETAILS_WRITE | MP4_DETAILS_SAMPLE), verbosity, expr)

#define VERBOSE_WRITE_HINT(verbosity, expr) \
    VERBOSE((MP4_DETAILS_WRITE | MP4_DETAILS_HINT), verbosity, expr)

#define VERBOSE_FIND(verbosity, expr)       \
    VERBOSE(MP4_DETAILS_FIND, verbosity, expr)

#define VERBOSE_ISMA(verbosity, expr)       \
    VERBOSE(MP4_DETAILS_ISMA, verbosity, expr)

#define VERBOSE_EDIT(verbosity, expr)       \
    VERBOSE(MP4_DETAILS_EDIT, verbosity, expr)

///////////////////////////////////////////////////////////////////////////////

#define CHECK_AND_FREE(a) if ((a) != NULL) { free((void *)(a)); (a) = NULL;}

#define NUM_ELEMENTS_IN_ARRAY(name) ((sizeof((name))) / (sizeof(*(name))))

///////////////////////////////////////////////////////////////////////////////

inline void Indent(FILE* pFile, uint8_t depth) {
    fprintf(pFile, "%*c", depth, ' ');
}

static inline void MP4Printf(const char* fmt, ...) MP4V2_WFORMAT_PRINTF(1,2);

static inline void MP4Printf(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    // TBD API call to set error_msg_func instead of just printf
    vprintf(fmt, ap);
    va_end(ap);
}

class MP4Error {
public:
    MP4Error() {
        m_errno = 0;
        m_errstring = NULL;
        m_where = NULL;
        m_free = 0;
    }
    ~MP4Error() {
        if (m_free != 0) {
            free((void *)m_errstring);
        }
    }
    MP4Error(int err, const char* where = NULL) {
        m_errno = err;
        m_errstring = NULL;
        m_where = where;
        m_free = 0;
    }
    MP4Error(const char *format, const char *where, ...) {
        char *string;
        m_errno = 0;
        string = (char *)malloc(512);
        m_where = where;
        if (string) {
            va_list ap;
            va_start(ap, where);
            vsnprintf(string, 512, format, ap);
            va_end(ap);
            m_errstring = string;
            m_free = 1;
        } else {
            m_errstring = format;
            m_free = 0;
        }
    }
    MP4Error(int err, const char* format, const char* where, ...) {
        char *string;
        m_errno = err;
        string = (char *)malloc(512);
        m_where = where;
        if (string) {
            va_list ap;
            va_start(ap, where);
            vsnprintf(string, 512, format, ap);
            va_end(ap);
            m_errstring = string;
            m_free = 1;
        } else {
            m_errstring = format;
            m_free = 0;
        }
    }

    void Print(FILE* pFile = stderr);
    int m_free;
    int m_errno;
    const char* m_errstring;
    const char* m_where;
};

void MP4HexDump(
    uint8_t* pBytes, uint32_t numBytes,
    FILE* pFile = stdout, uint8_t indent = 0);

inline void* MP4Malloc(size_t size) {
    if (size == 0) return NULL;
    void* p = malloc(size);
    if (p == NULL && size > 0) {
        throw new MP4Error(errno);
    }
    return p;
}

inline void* MP4Calloc(size_t size) {
    if (size == 0) return NULL;
    return memset(MP4Malloc(size), 0, size);
}

inline char* MP4Stralloc(const char* s1) {
    char* s2 = (char*)MP4Malloc(strlen(s1) + 1);
    strcpy(s2, s1);
    return s2;
}

inline void* MP4Realloc(void* p, uint32_t newSize) {
    // workaround library bug
    if (p == NULL && newSize == 0) {
        return NULL;
    }
    p = realloc(p, newSize);
    if (p == NULL && newSize > 0) {
        throw new MP4Error(errno);
    }
    return p;
}

inline uint32_t STRTOINT32(const char* s) {
    return MP4V2_NTOHL(*(uint32_t *)s);
}

inline void INT32TOSTR(uint32_t i, char* s) {
    *(uint32_t *)s = MP4V2_HTONL(i);
    s[4] = 0;
}

MP4Timestamp MP4GetAbsTimestamp();

uint64_t MP4ConvertTime(uint64_t t,
                        uint32_t oldTimeScale, uint32_t newTimeScale);

bool MP4NameFirstMatches(const char* s1, const char* s2);

bool MP4NameFirstIndex(const char* s, uint32_t* pIndex);

char* MP4NameFirst(const char *s);

const char* MP4NameAfterFirst(const char *s);

char* MP4ToBase16(const uint8_t* pData, uint32_t dataSize);

char* MP4ToBase64(const uint8_t* pData, uint32_t dataSize);

const char* MP4NormalizeTrackType(const char* type,
                                  uint32_t verbosity);

///////////////////////////////////////////////////////////////////////////////

}} // namespace mp4v2::impl

#endif // MP4V2_IMPL_MP4UTIL_H