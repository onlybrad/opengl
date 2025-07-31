#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../util/util.h"

#if !defined HASHMAP_K || !defined HASHMAP_V
    #error "Missing parameter HASHMAP_K or HASHMAP_V"
#endif

#define HASHMAP_CAPACITY  16
#define ENTRY             OB_MAKE_NAME(Entry,  OB_MAKE_NAME(HASHMAP_K,HASHMAP_V))
#define HASHMAP           OB_MAKE_NAME(Hashmap, OB_MAKE_NAME(HASHMAP_K,HASHMAP_V))
#define HASHMAP_RESIZE    OB_MAKE_NAME(HASHMAP, resize)
#define HASHMAP_GET_ENTRY OB_MAKE_NAME(HASHMAP, get_entry)
#define HASHMAP_INIT      OB_MAKE_NAME(HASHMAP, init)
#define HASHMAP_FREE      OB_MAKE_NAME(HASHMAP, free)
#define HASHMAP_EXISTS    OB_MAKE_NAME(HASHMAP, exists)
#define HASHMAP_INSERT    OB_MAKE_NAME(HASHMAP, insert)
#define HASHMAP_GET       OB_MAKE_NAME(HASHMAP, get)

#ifndef HASHMAP_IMPLEMENTATION

struct ENTRY {
    bool used;
    HASHMAP_K key;
    HASHMAP_V value;
};

struct HASHMAP {
    struct ENTRY* entries;
    size_t capacity;
    size_t (*hash_function)(HASHMAP_K);
    bool (*compare_function)(HASHMAP_K, HASHMAP_K);
};

bool HASHMAP_INIT     (struct HASHMAP *hashmap, size_t (*hash_function)(HASHMAP_K), bool (*compare_function)(HASHMAP_K, HASHMAP_K));
void HASHMAP_FREE     (struct HASHMAP *hashmap);
bool HASHMAP_EXISTS   (struct HASHMAP *hashmap, HASHMAP_K key);
bool HASHMAP_INSERT   (struct HASHMAP *hashmap, HASHMAP_K key, HASHMAP_V value);
HASHMAP_V HASHMAP_GET (struct HASHMAP *hashmap, HASHMAP_K key, bool *success);

#endif //HASHMAP_IMPLEMENTATION

#ifdef HASHMAP_IMPLEMENTATION

static struct ENTRY *HASHMAP_GET_ENTRY(struct HASHMAP *hashmap, HASHMAP_K key, bool ignore_unused) {
    assert(hashmap != NULL);

    size_t i = hashmap->hash_function(key) % hashmap->capacity;
    const size_t start = i;

    do {
        if(
            hashmap->compare_function(key, hashmap->entries[i].key) ||
            (!ignore_unused && !hashmap->entries[i].used)
        ) {
            return hashmap->entries + i;
        }

        i = (i + 1) % hashmap->capacity;
    } while(i != start);

    return NULL;
}

static bool HASHMAP_RESIZE(struct HASHMAP *hashmap, size_t capacity) {
    assert(hashmap != NULL);

    struct ENTRY *const new_entries = (struct ENTRY *)calloc(capacity, sizeof(struct ENTRY));
    if(new_entries == NULL) {
        return false;
    }

    struct ENTRY *const old_entries = hashmap->entries;
    const size_t old_capacity = hashmap->capacity;
    hashmap->capacity = capacity;
    hashmap->entries = new_entries;

    for(size_t i = 0; i < old_capacity; i++) {
        if(old_entries[i].used) {
            struct ENTRY *entry = HASHMAP_GET_ENTRY(hashmap, old_entries[i].key, false);
            entry->key = old_entries[i].key;
            entry->value = old_entries[i].value;
            entry->used = true;
        }
    }

    free(old_entries);

    return true;
}

bool HASHMAP_INIT(struct HASHMAP *hashmap, size_t (*hash_function)(HASHMAP_K), bool (*compare_function)(HASHMAP_K, HASHMAP_K)) {
    assert(hash_function != NULL);
    assert(compare_function != NULL);

    hashmap->capacity = HASHMAP_CAPACITY;
    hashmap->hash_function = hash_function;
    hashmap->compare_function = compare_function;
    hashmap->entries = (struct ENTRY*)calloc(HASHMAP_CAPACITY, sizeof(struct ENTRY));

    return hashmap->entries != NULL;
}

void HASHMAP_FREE(struct HASHMAP *hashmap) {
    free(hashmap->entries);
    memset(hashmap, 0, sizeof(*hashmap));
}

bool HASHMAP_EXISTS(struct HASHMAP *hashmap, HASHMAP_K key) {
    return HASHMAP_GET_ENTRY(hashmap, key, true) != NULL;
}

bool HASHMAP_INSERT(struct HASHMAP *hashmap, HASHMAP_K key, HASHMAP_V value) {
    struct ENTRY *entry = HASHMAP_GET_ENTRY(hashmap, key, false);
    
    if(entry == NULL) {
        if(!HASHMAP_RESIZE(hashmap, 2 * hashmap->capacity)) {
            return false;
        }
        entry = HASHMAP_GET_ENTRY(hashmap, key, false);
    }

    entry->key = key;
    entry->value = value;
    entry->used = true;

    return true;
}

HASHMAP_V HASHMAP_GET(struct HASHMAP *hashmap, HASHMAP_K key, bool *success) {
    struct ENTRY *const entry = HASHMAP_GET_ENTRY(hashmap, key, true);

    if(entry == NULL) {
        *success = false;
        return 0;
    }

    *success = true;
    return entry->value;
}

#endif //HASHMAP_IMPLEMENTATION

#undef HASHMAP_K
#undef HASHMAP_V
#undef HASHMAP_CAPACITY
#undef HASHMAP
#undef ENTRY
#undef HASHMAP_GET_ENTRY
#undef HASHMAP_INIT
#undef HASHMAP_FREE
#undef HASHMAP_EXISTS
#undef HASHMAP_INSERT
#undef HASHMAP_GET

#ifdef HASHMAP_IMPLEMENTATION
#undef HASHMAP_IMPLEMENTATION
#endif
