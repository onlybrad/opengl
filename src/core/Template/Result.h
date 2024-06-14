#include <stdbool.h>
#include "../Util/util.h"

#ifndef T
    #error "Missing parameter T"
#endif

#define RESULT_T MAKE_NAME(Result, T)

typedef struct RESULT_T {
    bool success;
    T value;
} RESULT_T;

#undef RESULT_T
