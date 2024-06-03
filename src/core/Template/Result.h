#include <stdbool.h>
#include "../Util/util.h"

//USAGE EXAMPLE
//pointer types and multi word types must be typedefed into a single word
//Result_{T}
#if 0
typedef const char *str;
#define T str
//#include Result.h
#endif

#ifndef T
    #error "Missing parameter T"
#endif

#define RESULT_T MAKE_NAME(Result, T)

typedef struct RESULT_T {
    bool success;
    T value;
} RESULT_T;

#undef RESULT_T
#undef T