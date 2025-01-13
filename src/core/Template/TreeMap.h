#include <stdlib.h>
#include <stdbool.h>
#include "../Util/util.h"

#if 1
typedef const char *str;
#define K str
#define V int 
#endif

#if !defined K || !defined V
    #error "Missing parameter K or V"
#endif

#define TREEMAP_INITIAL_CAPACITY 16
#define BUCKET MAKE_NAME(Bucket,MAKE_NAME(K,V))
#define BTREE MAKE_NAME(BinaryTree, BUCKET)
#define BTREE_NODE MAKE_NAME(Node,BUCKET)
#define BTREE_INIT MAKE_NAME(BTREE, init)
#define BTREE_FREE MAKE_NAME(BTREE, free)
#define BTREE_ADD_LEFT MAKE_NAME(BTREE, add_left)
#define BTREE_ADD_RIGHT MAKE_NAME(BTREE, add_right)
#define BTREE_GET_NODE MAKE_NAME(BTREE, get_node)
#define BTREE_GET_LEFT MAKE_NAME(BTREE, get_left)
#define BTREE_GET_RIGHT MAKE_NAME(BTREE, get_right)
#define TREEMAP MAKE_NAME(TreeMap,MAKE_NAME(K,V))
#define TREEMAP_RESIZE MAKE_NAME(TREEMAP, resize)
#define TREEMAP_GET_NODE MAKE_NAME(TREEMAP, get_node)
#define TREEMAP_INIT MAKE_NAME(TREEMAP, init)
#define TREEMAP_FREE MAKE_NAME(TREEMAP, free)
#define TREEMAP_EXISTS MAKE_NAME(TREEMAP, exists)
#define TREEMAP_INSERT MAKE_NAME(TREEMAP, insert)
#define TREEMAP_GET MAKE_NAME(TREEMAP, get)
#define TREEMAP_REMOVE MAKE_NAME(TREEMAP, remove)

#if defined TREEMAP_PRINT_KEY_FORMAT && defined TREEMAP_PRINT_VALUE_FORMAT && defined TREEMAP_PRINT_KEY_ARGUMENTS && defined TREEMAP_PRINT_VALUE_ARGUMENTS
#define TREEMAP_PRINT MAKE_NAME(TREEMAP, print)
#define TREEMAP_TO_STRING MAKE_NAME(TREEMAP, to_string)
#endif //TREEMAP_PRINT_KEY_FORMAT TREEMAP_PRINT_VALUE_FORMAT TREEMAP_PRINT_KEY_ARGUMENTS TREEMAP_PRINT_VALUE_ARGUMENTS

#ifndef TREEMAP_IMPLEMENTATION

typedef struct BUCKET {
    K key;
    V value;
} BUCKET;

#define T BUCKET
#include "BinaryTree.h"

typedef struct TREEMAP {
    BTREE binary_tree;
    int (*compare_function)(K, K);
} TREEMAP;

void TREEMAP_INIT(TREEMAP treemap[static 1], int (*compare_function)(K,K));
void TREEMAP_FREE(TREEMAP treemap[static 1]);
bool TREEMAP_EXISTS(TREEMAP treemap[static 1], K key);
void TREEMAP_INSERT(TREEMAP treemap[static 1], K key, V value);
V TREEMAP_GET(TREEMAP treemap[static 1], K key, bool *const success);
bool TREEMAP_REMOVE(TREEMAP treemap[static 1], K key);

#if defined TREEMAP_PRINT_KEY_FORMAT && defined TREEMAP_PRINT_VALUE_FORMAT && defined TREEMAP_PRINT_KEY_ARGUMENTS && defined TREEMAP_PRINT_VALUE_ARGUMENTS
void TREEMAP_PRINT(const TREEMAP treemap[static 1]);
char *TREEMAP_TO_STRING(const TREEMAP treemap[static 1]);
#endif //TREEMAP_PRINT_KEY_FORMAT TREEMAP_PRINT_VALUE_FORMAT TREEMAP_PRINT_KEY_ARGUMENTS TREEMAP_PRINT_VALUE_ARGUMENTS

#endif //TREEMAP_IMPLEMENTATION

#if 1
#define TREEMAP_IMPLEMENTATION
#endif

#ifdef TREEMAP_IMPLEMENTATION

#include <assert.h>
#define T BUCKET
#define BINARY_TREE_IMPLEMENTATION
#include "BinaryTree.h"

static BUCKET *TREEMAP_GET_NODE(TREEMAP treemap[static 1], K key, const bool ignore_unused) {
    TreeNode node = TreeNode_ROOT;
    
    while(true) {
        BTREE_NODE *const node_ptr = BTREE_GET_NODE(&treemap->binary_tree, node);

        if(!node_ptr->used) {
            return &node_ptr;
        }

        const int cmp = treemap->compare_function(key, node_ptr->value.key);

        if(cmp < 0) {
            const TreeNode left = BTREE_GET_LEFT(&treemap->binary_tree, node);
            if(left != TreeNode_NULL) {
                node = left;
            } else if(!ignore_unused) {
                node = BTREE_ADD_LEFT(&treemap->binary_tree, node, (BUCKET){.key = key});
                return BTREE_GET_NODE(&treemap->binary_tree, node);
            } else {
                return NULL;
            }
        } else if(cmp > 0) {
            const TreeNode right = BTREE_GET_RIGHT(&treemap->binary_tree, node);
            if(right != TreeNode_NULL) {
                node = right;
            } else if(!ignore_unused) {
                node = BTREE_ADD_RIGHT(&treemap->binary_tree, node, (BUCKET){.key = key});
                return BTREE_GET_NODE(&treemap->binary_tree, node);
            } else {
                return NULL;
            }
        } else {
            return BTREE_GET_NODE(&treemap->binary_tree, node);
        }
    }
}

void TREEMAP_INIT(TREEMAP treemap[static 1], int (*compare_function)(K, K)) {
    assert(compare_function != NULL);
    BTREE_INIT(&treemap->binary_tree);
    treemap->compare_function = compare_function;
}

void TREEMAP_FREE(TREEMAP treemap[static 1]) {
    BTREE_FREE(&treemap->binary_tree);
    *treemap = (TREEMAP){0};
}

bool TREEMAP_EXISTS(TREEMAP treemap[static 1], K key) {
    return TREEMAP_GET_NODE(treemap, key, true) != NULL;
}

void TREEMAP_INSERT(TREEMAP treemap[static 1], K key, V value) {
    BTREE_NODE *const node = TREEMAP_GET_NODE(treemap, key, false);
    node->used = true;
    node->value.key = key;
    node->value.value = value;
}

V TREEMAP_GET(TREEMAP treemap[static 1], K key, bool *const success) {
    BTREE_NODE *const node = TREEMAP_GET_NODE(treemap, key, true);

    if(node == NULL) {
        *success = false;
        return (V){0};
    }

    *success = true;
    return node->value.value;
}

bool TREEMAP_REMOVE(TREEMAP treemap[static 1], K key) {
    BTREE_NODE *const node = TREEMAP_GET_NODE(treemap, key, true);
    if(node == NULL) {
        return false;
    }
    
    *node = (BTREE_NODE){0};

    return true;
}

#if defined TREEMAP_PRINT_KEY_FORMAT && defined TREEMAP_PRINT_VALUE_FORMAT && defined TREEMAP_PRINT_KEY_ARGUMENTS && defined TREEMAP_PRINT_VALUE_ARGUMENTS
void TREEMAP_PRINT(const TREEMAP treemap[static 1]) {
    putchar('[');

    size_t i;
    for(i = 0; i<treemap->capacity; i++) {
        if(treemap->buckets[i].used) {
            printf(TREEMAP_PRINT_KEY_FORMAT " => " TREEMAP_PRINT_VALUE_FORMAT, TREEMAP_PRINT_KEY_ARGUMENTS(treemap->buckets[i].key), TREEMAP_PRINT_VALUE_ARGUMENTS(treemap->buckets[i].value));
            i++;
            break;
        }
    }

    for(;i<treemap->capacity; i++) {
        if(treemap->buckets[i].used) {
            printf(", " TREEMAP_PRINT_KEY_FORMAT " => " TREEMAP_PRINT_VALUE_FORMAT, TREEMAP_PRINT_KEY_ARGUMENTS(treemap->buckets[i].key), TREEMAP_PRINT_VALUE_ARGUMENTS(treemap->buckets[i].value));
        }
    }

    puts("]");
}

char *TREEMAP_TO_STRING(const TREEMAP treemap[static 1]) {
    size_t length = 2;
    size_t i;

    for(i=0; i<treemap->capacity; i++) {
        if(treemap->buckets[i].used) {
            const int char_printed = snprintf(NULL, 0, TREEMAP_PRINT_KEY_FORMAT " => " TREEMAP_PRINT_VALUE_FORMAT, TREEMAP_PRINT_KEY_ARGUMENTS(treemap->buckets[i].key), TREEMAP_PRINT_VALUE_ARGUMENTS(treemap->buckets[i].value));
            length += (size_t)char_printed;
            i++;            
            break;
        }
    }

    for(;i<treemap->capacity; i++) {
        if(treemap->buckets[i].used) {
            const int char_printed = snprintf(NULL, 0, ", " TREEMAP_PRINT_KEY_FORMAT " => " TREEMAP_PRINT_VALUE_FORMAT, TREEMAP_PRINT_KEY_ARGUMENTS(treemap->buckets[i].key), TREEMAP_PRINT_VALUE_ARGUMENTS(treemap->buckets[i].value));
            length += (size_t)char_printed;
        }
    }

    char *treemap_str = malloc((length + 1) * sizeof(char));
    assert(treemap_str != NULL);
    treemap_str[0] = '[';

    size_t offset = 1;
    for(i=0; i<treemap->capacity; i++) {
        if(treemap->buckets[i].used) {
            const int char_printed = sprintf(treemap_str + offset, TREEMAP_PRINT_KEY_FORMAT " => " TREEMAP_PRINT_VALUE_FORMAT, TREEMAP_PRINT_KEY_ARGUMENTS(treemap->buckets[i].key), TREEMAP_PRINT_VALUE_ARGUMENTS(treemap->buckets[i].value));
            offset += (size_t)char_printed;
            i++;
            break;
        }
    }

    for(;i<treemap->capacity; i++) {
        if(treemap->buckets[i].used) {
            const int char_printed = sprintf(treemap_str + offset, ", " TREEMAP_PRINT_KEY_FORMAT " => " TREEMAP_PRINT_VALUE_FORMAT, TREEMAP_PRINT_KEY_ARGUMENTS(treemap->buckets[i].key), TREEMAP_PRINT_VALUE_ARGUMENTS(treemap->buckets[i].value));
            offset += (size_t)char_printed;
        }
    }

    treemap_str[length - 1] = ']'; 
    treemap_str[length] = '\0'; 

    return treemap_str;
}
#endif //TREEMAP_PRINT_KEY_FORMAT TREEMAP_PRINT_VALUE_FORMAT TREEMAP_PRINT_KEY_ARGUMENTS TREEMAP_PRINT_VALUE_ARGUMENTS

#endif //TREEMAP_IMPLEMENTATION

#undef K
#undef V
#undef T
#undef RESULT
#undef BTREE
#undef BTREE_INIT
#undef BTREE_FREE
#undef BTREE_ADD_LEFT
#undef BTREE_ADD_RIGHT
#undef BTREE_GET_NODE
#undef BTREE_GET_LEFT
#undef BTREE_GET_RIGHT
#undef TREEMAP_INITIAL_CAPACITY
#undef TREEMAP
#undef BUCKET
#undef TREEMAP_GET_NODE
#undef TREEMAP_INIT
#undef TREEMAP_FREE
#undef TREEMAP_EXISTS
#undef TREEMAP_INSERT
#undef TREEMAP_GET
#undef TREEMAP_REMOVE

#if defined TREEMAP_PRINT_KEY_FORMAT && defined TREEMAP_PRINT_VALUE_FORMAT && defined TREEMAP_PRINT_KEY_ARGUMENTS && defined TREEMAP_PRINT_VALUE_ARGUMENTS
#undef TREEMAP_PRINT
#undef TREEMAP_TO_STRING
#undef TREEMAP_PRINT_KEY_FORMAT
#undef TREEMAP_PRINT_VALUE_FORMAT
#undef TREEMAP_PRINT_KEY_ARGUMENTS
#undef TREEMAP_PRINT_VALUE_ARGUMENTS
#endif

#ifdef TREEMAP_IMPLEMENTATION
#undef TREEMAP_IMPLEMENTATION
#endif
