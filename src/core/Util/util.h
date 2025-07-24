#ifndef _UTIL_H
#define _UTIL_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "String.h"

#if defined(__clang__) || defined(__GNUC__)
#define FALLTHROUGH {__attribute__((fallthrough));}
#elif defined(_MSC_VER)
#define FALLTHROUGH {__fallthrough;}
#elif defined(__INTEL_COMPILER)
#define FALLTHROUGH {[[fallthrough]];}
#else
#define FALLTHROUGH
#endif

#if _WIN32
#include <stringapiset.h>
#endif

#define _MAKE_NAME(a,b) a ## _ ## b
#define MAKE_NAME(a,b) _MAKE_NAME(a,b)

#define SIZE64 SIZE_MAX == ((1ULL << 8*CHAR_BIT-1)-1 << 1 | 1)

#define _STRINGIFY(a) #a
#define STRINGIFY(a) _STRINGIFY(a)

#define INT_TO_PTR(INT) (void *)(uintptr_t)(INT)
#define ARRAY_LEN(ARRAY) (sizeof(ARRAY)/sizeof(ARRAY[0]))
#define MEMBER_SIZE(STRUCT, MEMBER) (sizeof(((STRUCT *)0)->MEMBER)) 
#define ARRAY_MEMBER_LEN(STRUCT, MEMBER) (MEMBER_SIZE(STRUCT, MEMBER)/MEMBER_SIZE(STRUCT, MEMBER[0]))

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif


FILE *file_open(const char *filename, const char *mode);
String file_get_contents(const char *path);
void *unconst(const void *const_var);

#endif