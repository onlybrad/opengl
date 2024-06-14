#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "../Util/util.h"

#if !defined K || !defined V
    #error "Missing parameter K or V"
#endif

#define HASHMAP_INITIAL_CAPACITY 16
#define BUCKET MAKE_NAME(Bucket,MAKE_NAME(K,V))
#define RESULT MAKE_NAME(Result, V)
#define HASHMAP MAKE_NAME(Hashmap,MAKE_NAME(K,V))
#define HASHMAP_RESIZE MAKE_NAME(HASHMAP, resize)
#define HASHMAP_GET_BUCKET MAKE_NAME(HASHMAP, get_bucket)
#define HASHMAP_INIT MAKE_NAME(HASHMAP, init)
#define HASHMAP_FREE MAKE_NAME(HASHMAP, free)
#define HASHMAP_EXISTS MAKE_NAME(HASHMAP, exists)
#define HASHMAP_INSERT MAKE_NAME(HASHMAP, insert)
#define HASHMAP_GET MAKE_NAME(HASHMAP, get)
#define HASHMAP_REMOVE MAKE_NAME(HASHMAP, remove)

#if defined HASHMAP_PRINT_KEY_FORMAT && defined HASHMAP_PRINT_VALUE_FORMAT && defined HASHMAP_PRINT_KEY_ARGUMENTS && defined HASHMAP_PRINT_VALUE_ARGUMENTS
#define HASHMAP_PRINT MAKE_NAME(HASHMAP, print)
#define HASHMAP_TO_STRING MAKE_NAME(HASHMAP, to_string)
#endif //HASHMAP_PRINT_KEY_FORMAT HASHMAP_PRINT_VALUE_FORMAT HASHMAP_PRINT_KEY_ARGUMENTS HASHMAP_PRINT_VALUE_ARGUMENTS

#ifndef HASHMAP_IMPLEMENTATION

#define T V
#include "Result.h"
#undef T

typedef struct BUCKET {
    K key;
    V value;
    bool used;
} BUCKET;
typedef struct HASHMAP {
    BUCKET* buckets;
    size_t capacity;
    size_t (*hash_function)(K);
    bool (*compare_function)(K, K);
} HASHMAP;

void HASHMAP_INIT(HASHMAP *const hashmap, size_t (*hash_function)(K), bool (*compare_function)(K, K));
void HASHMAP_FREE(HASHMAP *const hashmap);
bool HASHMAP_EXISTS(HASHMAP *const hashmap, K key);
void HASHMAP_INSERT(HASHMAP *const hashmap, K key, V value);
RESULT HASHMAP_GET(HASHMAP *const hashmap, K key);
bool HASHMAP_REMOVE(HASHMAP *const hashmap, K key);

#if defined HASHMAP_PRINT_KEY_FORMAT && defined HASHMAP_PRINT_VALUE_FORMAT && defined HASHMAP_PRINT_KEY_ARGUMENTS && defined HASHMAP_PRINT_VALUE_ARGUMENTS
void HASHMAP_PRINT(const HASHMAP *const hashmap);
char *HASHMAP_TO_STRING(const HASHMAP *const hashmap);
#endif //HASHMAP_PRINT_KEY_FORMAT HASHMAP_PRINT_VALUE_FORMAT HASHMAP_PRINT_KEY_ARGUMENTS HASHMAP_PRINT_VALUE_ARGUMENTS

#endif

#if 0
#define HASHMAP_IMPLEMENTATION
#endif

#if defined HASHMAP_IMPLEMENTATION

static void HASHMAP_RESIZE(HASHMAP *const hashmap, const size_t capacity) {
    BUCKET *const new_buckets = calloc(capacity, sizeof(BUCKET));
    assert(new_buckets != NULL);
    BUCKET *const old_buckets = hashmap->buckets;
    const size_t old_capacity = hashmap->capacity;
    hashmap->capacity = capacity;
    hashmap->buckets = new_buckets;

    for(size_t i=0; i<old_capacity; i++) {
        if(old_buckets[i].used) {
            HASHMAP_INSERT(hashmap, old_buckets[i].key, old_buckets[i].value);
        }
    }

    free(old_buckets);
}

static BUCKET *HASHMAP_GET_BUCKET(HASHMAP *const hashmap, K key, const bool ignore_used) {
    size_t i = hashmap->hash_function(key) % hashmap->capacity;
    const size_t start = i;

    do {
        if(
            hashmap->compare_function(key, hashmap->buckets[i].key) ||
            (!ignore_used && !hashmap->buckets[i].used)
        ) {
            return &hashmap->buckets[i];
        }

        i = (i + 1) % hashmap->capacity;
    } while(i != start);

    return NULL;
}

void HASHMAP_INIT(HASHMAP *const hashmap, size_t (*hash_function)(K), bool (*compare_function)(K, K)) {
    hashmap->capacity = HASHMAP_INITIAL_CAPACITY;
    hashmap->hash_function = hash_function;
    hashmap->compare_function = compare_function;
    hashmap->buckets = calloc(HASHMAP_INITIAL_CAPACITY, sizeof(BUCKET));
    assert(hashmap->buckets != NULL);
}

void HASHMAP_FREE(HASHMAP *const hashmap) {
    free(hashmap->buckets);
    memset(hashmap, 0, sizeof(HASHMAP));
}

bool HASHMAP_EXISTS(HASHMAP *const hashmap, K key) {
    return HASHMAP_GET_BUCKET(hashmap, key, true) != NULL;
}

void HASHMAP_INSERT(HASHMAP *const hashmap, K key, V value) {
    BUCKET *bucket = HASHMAP_GET_BUCKET(hashmap, key, false);
    
    if(bucket == NULL) {
        HASHMAP_RESIZE(hashmap, 2 * hashmap->capacity);
        bucket = HASHMAP_GET_BUCKET(hashmap, key, false);
    }

    bucket->key = key;
    bucket->value = value;
    bucket->used = true;
}

RESULT HASHMAP_GET(HASHMAP *const hashmap, K key) {
    BUCKET *bucket = HASHMAP_GET_BUCKET(hashmap, key, true);

    return bucket == NULL 
        ? (RESULT) {.success = false}
        : (RESULT) {.success = true, .value = bucket->value};
}

bool HASHMAP_REMOVE(HASHMAP *const hashmap, K key) {
    BUCKET *bucket = HASHMAP_GET_BUCKET(hashmap, key, true);

    if(bucket == NULL) {
        return false;
    }

    memset(bucket, 0, sizeof(BUCKET));
    return true;
}

#if defined HASHMAP_PRINT_KEY_FORMAT && defined HASHMAP_PRINT_VALUE_FORMAT && defined HASHMAP_PRINT_KEY_ARGUMENTS && defined HASHMAP_PRINT_VALUE_ARGUMENTS
void HASHMAP_PRINT(const HASHMAP *const hashmap) {
    putchar('[');

    size_t i;
    for(i = 0; i<hashmap->capacity; i++) {
        if(hashmap->buckets[i].used) {
            printf(HASHMAP_PRINT_KEY_FORMAT " => " HASHMAP_PRINT_VALUE_FORMAT, HASHMAP_PRINT_KEY_ARGUMENTS(hashmap->buckets[i].key), HASHMAP_PRINT_VALUE_ARGUMENTS(hashmap->buckets[i].value));
            i++;
            break;
        }
    }

    for(;i<hashmap->capacity; i++) {
        if(hashmap->buckets[i].used) {
            printf(", " HASHMAP_PRINT_KEY_FORMAT " => " HASHMAP_PRINT_VALUE_FORMAT, HASHMAP_PRINT_KEY_ARGUMENTS(hashmap->buckets[i].key), HASHMAP_PRINT_VALUE_ARGUMENTS(hashmap->buckets[i].value));
        }
    }

    puts("]");
}

char *HASHMAP_TO_STRING(const HASHMAP *const hashmap) {
    size_t length = 2;
    size_t i;

    for(i=0; i<hashmap->capacity; i++) {
        if(hashmap->buckets[i].used) {
            const int char_printed = snprintf(NULL, 0, HASHMAP_PRINT_KEY_FORMAT " => " HASHMAP_PRINT_VALUE_FORMAT, HASHMAP_PRINT_KEY_ARGUMENTS(hashmap->buckets[i].key), HASHMAP_PRINT_VALUE_ARGUMENTS(hashmap->buckets[i].value));
            length += (size_t)char_printed;
            i++;            
            break;
        }
    }

    for(;i<hashmap->capacity; i++) {
        if(hashmap->buckets[i].used) {
            const int char_printed = snprintf(NULL, 0, ", " HASHMAP_PRINT_KEY_FORMAT " => " HASHMAP_PRINT_VALUE_FORMAT, HASHMAP_PRINT_KEY_ARGUMENTS(hashmap->buckets[i].key), HASHMAP_PRINT_VALUE_ARGUMENTS(hashmap->buckets[i].value));
            length += (size_t)char_printed;
        }
    }

    char *hashmap_str = malloc(length * sizeof(char) + 1);
    assert(hashmap_str != NULL);
    hashmap_str[0] = '[';

    size_t offset = 1;
    for(i=0; i<hashmap->capacity; i++) {
        if(hashmap->buckets[i].used) {
            const int char_printed = sprintf(hashmap_str + offset, HASHMAP_PRINT_KEY_FORMAT " => " HASHMAP_PRINT_VALUE_FORMAT, HASHMAP_PRINT_KEY_ARGUMENTS(hashmap->buckets[i].key), HASHMAP_PRINT_VALUE_ARGUMENTS(hashmap->buckets[i].value));
            offset += (size_t)char_printed;
            i++;
            break;
        }
    }

    for(;i<hashmap->capacity; i++) {
        if(hashmap->buckets[i].used) {
            const int char_printed = sprintf(hashmap_str + offset, ", " HASHMAP_PRINT_KEY_FORMAT " => " HASHMAP_PRINT_VALUE_FORMAT, HASHMAP_PRINT_KEY_ARGUMENTS(hashmap->buckets[i].key), HASHMAP_PRINT_VALUE_ARGUMENTS(hashmap->buckets[i].value));
            offset += (size_t)char_printed;
        }
    }

    hashmap_str[length - 1] = ']'; 
    hashmap_str[length] = '\0'; 

    return hashmap_str;
}
#endif //HASHMAP_PRINT_KEY_FORMAT HASHMAP_PRINT_VALUE_FORMAT HASHMAP_PRINT_KEY_ARGUMENTS HASHMAP_PRINT_VALUE_ARGUMENTS

#endif //HASHMAP_IMPLEMENTATION

#undef K
#undef V
#undef T
#undef RESULT
#undef HASHMAP_INITIAL_CAPACITY
#undef HASHMAP
#undef BUCKET
#undef HASHMAP_GET_BUCKET
#undef HASHMAP_INIT
#undef HASHMAP_FREE
#undef HASHMAP_EXISTS
#undef HASHMAP_INSERT
#undef HASHMAP_GET
#undef HASHMAP_REMOVE

#if defined HASHMAP_PRINT_KEY_FORMAT && defined HASHMAP_PRINT_VALUE_FORMAT && defined HASHMAP_PRINT_KEY_ARGUMENTS && defined HASHMAP_PRINT_VALUE_ARGUMENTS
#undef HASHMAP_PRINT
#undef HASHMAP_TO_STRING
#undef HASHMAP_PRINT_KEY_FORMAT
#undef HASHMAP_PRINT_VALUE_FORMAT
#undef HASHMAP_PRINT_KEY_ARGUMENTS
#undef HASHMAP_PRINT_VALUE_ARGUMENTS
#endif

#ifdef HASHMAP_IMPLEMENTATION
#undef HASHMAP_IMPLEMENTATION
#endif
