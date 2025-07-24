#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../Util/util.h"

#if 0
typedef const char *str;
#define HASHMAP_K str
#define HASHMAP_V int 
#endif

#if !defined HASHMAP_K || !defined HASHMAP_V
    #error "Missing parameter HASHMAP_K or HASHMAP_V"
#endif

#define HASHMAP_INITIAL_CAPACITY 16
#define BUCKET OB_MAKE_NAME(Bucket, OB_MAKE_NAME(HASHMAP_K,HASHMAP_V))
#define HASHMAP OB_MAKE_NAME(Hashmap, OB_MAKE_NAME(HASHMAP_K,HASHMAP_V))
#define HASHMAP_RESIZE OB_MAKE_NAME(HASHMAP, resize)
#define HASHMAP_GET_BUCKET OB_MAKE_NAME(HASHMAP, get_bucket)
#define HASHMAP_INIT OB_MAKE_NAME(HASHMAP, init)
#define HASHMAP_FREE OB_MAKE_NAME(HASHMAP, free)
#define HASHMAP_EXISTS OB_MAKE_NAME(HASHMAP, exists)
#define HASHMAP_INSERT OB_MAKE_NAME(HASHMAP, insert)
#define HASHMAP_GET OB_MAKE_NAME(HASHMAP, get)
#define HASHMAP_REMOVE OB_MAKE_NAME(HASHMAP, remove)

#ifndef HASHMAP_IMPLEMENTATION

struct BUCKET {
    bool used;
    HASHMAP_K key;
    HASHMAP_V value;
};

struct HASHMAP {
    struct BUCKET* buckets;
    size_t capacity;
    size_t (*hash_function)(HASHMAP_K);
    bool (*compare_function)(HASHMAP_K, HASHMAP_K);
};

bool HASHMAP_INIT(struct HASHMAP *hashmap, size_t (*hash_function)(HASHMAP_K), bool (*compare_function)(HASHMAP_K, HASHMAP_K));
void HASHMAP_FREE(struct HASHMAP *hashmap);
bool HASHMAP_EXISTS(struct HASHMAP *hashmap, HASHMAP_K key);
void HASHMAP_INSERT(struct HASHMAP *hashmap, HASHMAP_K key, HASHMAP_V value);
HASHMAP_V HASHMAP_GET(struct HASHMAP *hashmap, HASHMAP_K key, bool *success);
bool HASHMAP_REMOVE(struct HASHMAP *hashmap, HASHMAP_K key);

#if defined HASHMAP_PRINT_KEY_FORMAT && defined HASHMAP_PRINT_VALUE_FORMAT && defined HASHMAP_PRINT_KEY_ARGUMENTS && defined HASHMAP_PRINT_VALUE_ARGUMENTS
void HASHMAP_PRINT(const HASHMAP *hashmap);
char *HASHMAP_TO_STRING(const HASHMAP *hashmap);
#endif //HASHMAP_PRINT_KEY_FORMAT HASHMAP_PRINT_VALUE_FORMAT HASHMAP_PRINT_KEY_ARGUMENTS HASHMAP_PRINT_VALUE_ARGUMENTS

#endif //HASHMAP_IMPLEMENTATION

#if 0
#define HASHMAP_IMPLEMENTATION
#endif

#ifdef HASHMAP_IMPLEMENTATION

static struct BUCKET *HASHMAP_GET_BUCKET(struct HASHMAP *hashmap, HASHMAP_K key, bool ignore_unused) {
    assert(hashmap != NULL);

    size_t i = hashmap->hash_function(key) % hashmap->capacity;
    const size_t start = i;

    do {
        if(
            hashmap->compare_function(key, hashmap->buckets[i].key) ||
            (!ignore_unused && !hashmap->buckets[i].used)
        ) {
            return hashmap->buckets + i;
        }

        i = (i + 1) % hashmap->capacity;
    } while(i != start);

    return NULL;
}

static bool HASHMAP_RESIZE(struct HASHMAP *hashmap, size_t capacity) {
    assert(hashmap != NULL);

    struct BUCKET *const new_buckets = (struct BUCKET *)calloc(capacity, sizeof(struct BUCKET));
    
    if(new_buckets == NULL) {
        return false;
    }

    struct BUCKET *const old_buckets = hashmap->buckets;
    const size_t old_capacity = hashmap->capacity;
    hashmap->capacity = capacity;
    hashmap->buckets = new_buckets;

    for(size_t i = 0; i < old_capacity; i++) {
        if(old_buckets[i].used) {
            struct BUCKET *bucket = HASHMAP_GET_BUCKET(hashmap, old_buckets[i].key, false);
            bucket->key = old_buckets[i].key;
            bucket->value = old_buckets[i].value;
            bucket->used = true;
        }
    }

    free(old_buckets);

    return true;
}

bool HASHMAP_INIT(struct HASHMAP *hashmap, size_t (*hash_function)(HASHMAP_K), bool (*compare_function)(HASHMAP_K, HASHMAP_K)) {
    assert(hash_function != NULL);
    assert(compare_function != NULL);

    hashmap->capacity = HASHMAP_INITIAL_CAPACITY;
    hashmap->hash_function = hash_function;
    hashmap->compare_function = compare_function;
    hashmap->buckets = (struct BUCKET*)calloc(HASHMAP_INITIAL_CAPACITY, sizeof(struct BUCKET));

    return hashmap->buckets != NULL;
}

void HASHMAP_FREE(struct HASHMAP *hashmap) {
    free(hashmap->buckets);
    memset(hashmap, 0, sizeof(*hashmap));
}

bool HASHMAP_EXISTS(struct HASHMAP *hashmap, HASHMAP_K key) {
    return HASHMAP_GET_BUCKET(hashmap, key, true) != NULL;
}

void HASHMAP_INSERT(struct HASHMAP *hashmap, HASHMAP_K key, HASHMAP_V value) {
    struct BUCKET *bucket = HASHMAP_GET_BUCKET(hashmap, key, false);
    
    if(bucket == NULL) {
        HASHMAP_RESIZE(hashmap, 2 * hashmap->capacity);
        bucket = HASHMAP_GET_BUCKET(hashmap, key, false);
    }

    bucket->key = key;
    bucket->value = value;
    bucket->used = true;
}

HASHMAP_V HASHMAP_GET(struct HASHMAP *hashmap, HASHMAP_K key, bool *success) {
    struct BUCKET *const bucket = HASHMAP_GET_BUCKET(hashmap, key, true);

    if(bucket == NULL) {
        *success = false;
        return 0;
    }

    *success = true;
    return bucket->value;
}

bool HASHMAP_REMOVE(struct HASHMAP *hashmap, HASHMAP_K key) {
    struct BUCKET *const bucket = HASHMAP_GET_BUCKET(hashmap, key, true);
    if(bucket == NULL) {
        return false;
    }
    
    memset(bucket, 0, sizeof(*bucket));

    return true;
}

#endif //HASHMAP_IMPLEMENTATION

#undef HASHMAP_K
#undef HASHMAP_V
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

#ifdef HASHMAP_IMPLEMENTATION
#undef HASHMAP_IMPLEMENTATION
#endif
