#ifndef _UTIL_H
#define _UTIL_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "String.h"

#if defined(__clang__) || defined(__GNUC__)
#define OB_FALLTHROUGH {__attribute__((fallthrough));}
#elif defined(_MSC_VER)
#define OB_FALLTHROUGH {__fallthrough;}
#elif defined(__INTEL_COMPILER)
#define OB_FALLTHROUGH {[[fallthrough]];}
#else
#define OB_FALLTHROUGH
#endif

#if _WIN32
#include <stringapiset.h>
#endif

#define _OB_MAKE_NAME(a,b) a ## _ ## b
#define OB_MAKE_NAME(a,b) _OB_MAKE_NAME(a,b)

#define OB_INT_TO_PTR(INT) (void *)(uintptr_t)(INT)
#define OB_ARRAY_LEN(ARRAY) (sizeof(ARRAY)/sizeof(ARRAY[0]))
#define OB_MEMBER_SIZE(STRUCT, MEMBER) (sizeof(((STRUCT *)0)->MEMBER)) 
#define OB_ARRAY_MEMBER_LEN(STRUCT, MEMBER) (OB_MEMBER_SIZE(STRUCT, MEMBER)/OB_MEMBER_SIZE(STRUCT, MEMBER[0]))

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

FILE *OB_file_open(const char *filename, const char *mode);
struct OB_String OB_file_get_contents(const char *path);
void *OB_unconst(const void *const_var);

#endif