#if defined(__clang__) || defined(__GNUC__)
#define FALLTHROUGH {__attribute__((fallthrough));}
#elif defined(_MSC_VER)
#define FALLTHROUGH {__fallthrough;}
#elif defined(__INTEL_COMPILER)
#define FALLTHROUGH {[[fallthrough]];}
#else
#define FALLTHROUGH
#endif

#define INT_TO_PTR(INT) (void *)(uintptr_t)(INT)
#define ARRAY_LENGTH(ARRAY) (sizeof(ARRAY)/sizeof(ARRAY[0]))
#define MEMBER_SIZE(STRUCT, MEMBER) (sizeof(((STRUCT *)0)->MEMBER)) 
#define ARRAY_MEMBER_LENGTH(STRUCT, MEMBER) (MEMBER_SIZE(STRUCT, MEMBER)/MEMBER_SIZE(STRUCT, MEMBER[0]))
#define GL_CALL(CODE) GLClearError();\
CODE;\
GLCheckError();
#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

void GLClearError(void);
void GLCheckError(void);
char *file_get_contents(size_t *const length, const char *const path);
void *unconst(const void *var);