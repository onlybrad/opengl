#include <stdlib.h>
#include <stdbool.h>
#include "../Util/util.h"

#if 0
typedef const char *str;
#define T str
#endif

#ifndef T
    #error "Missing parameter T"
#endif

#define BINARY_TREE_INITIAL_CAPACITY 16
#define NODE MAKE_NAME(Node,T)
#define BINARY_TREE MAKE_NAME(BinaryTree,T)
#define BINARY_TREE_RESIZE MAKE_NAME(BINARY_TREE, resize)
#define BINARY_TREE_INIT MAKE_NAME(BINARY_TREE, init)
#define BINARY_TREE_FREE MAKE_NAME(BINARY_TREE, free)
#define BINARY_TREE_COPY MAKE_NAME(BINARY_TREE, copy)
#define BINARY_TREE_ADD_DIRECTION MAKE_NAME(BINARY_TREE, add_direction)
#define BINARY_TREE_ADD_LEFT MAKE_NAME(BINARY_TREE, add_left)
#define BINARY_TREE_ADD_RIGHT MAKE_NAME(BINARY_TREE, add_right)
#define BINARY_TREE_GET_VALUE MAKE_NAME(BINARY_TREE, get_value)
#define BINARY_TREE_GET_NODE MAKE_NAME(BINARY_TREE, get_node)
#define BINARY_TREE_SET_VALUE MAKE_NAME(BINARY_TREE, set_value)
#define BINARY_TREE_GET_ROOT MAKE_NAME(BINARY_TREE, get_root)
#define BINARY_TREE_GET_DIRECTION MAKE_NAME(BINARY_TREE, get_direction)
#define BINARY_TREE_GET_LEFT MAKE_NAME(BINARY_TREE, get_left)
#define BINARY_TREE_GET_RIGHT MAKE_NAME(BINARY_TREE, get_right)
#define BINARY_TREE_GET_PARENT MAKE_NAME(BINARY_TREE, get_parent)
#define BINARY_TREE_GET_DEEPEST MAKE_NAME(BINARY_TREE, get_deepest)
#define BINARY_TREE_GET_DEPTH MAKE_NAME(BINARY_TREE, get_depth)
#define BINARY_TREE_GET_NODE_DEPTH MAKE_NAME(BINARY_TREE, get_node_depth)
#define BINARY_TREE_GET_CHILDREN MAKE_NAME(BINARY_TREE, get_children)
#define BINARY_TREE_GET_CHILDREN_COUNT MAKE_NAME(BINARY_TREE, get_children_count)
#define BINARY_TREE_DETACH MAKE_NAME(BINARY_TREE, detach)

#if defined BINARY_TREE_PRINT_FORMAT && defined BINARY_TREE_PRINT_ARGUMENT
#define BINARY_TREE_PRINT MAKE_NAME(BINARY_TREE, print)
#endif

#ifndef _BINARY_TREE_H
#define _BINARY_TREE_H
typedef enum Direction {
    DirectionLeft = 1,
    DirectionRight = 2
} Direction;
typedef size_t TreeNode;
#define TreeNode_NULL ((TreeNode)-1)
#define TreeNode_ROOT ((TreeNode)0)
#endif

#ifndef BINARY_TREE_IMPLEMENTATION

typedef struct NODE {
    bool used;
    T value;
} NODE;

typedef struct BINARY_TREE {
    NODE* nodes;
    TreeNode deepest;
    size_t capacity;
} BINARY_TREE;

void BINARY_TREE_INIT(BINARY_TREE binary_tree[static 1]);
void BINARY_TREE_FREE(BINARY_TREE binary_tree[static 1]);
void BINARY_TREE_COPY(BINARY_TREE dest[static 1], BINARY_TREE src[static 1]);
T BINARY_TREE_GET_VALUE(BINARY_TREE binary_tree[static 1], const TreeNode node);
NODE *BINARY_TREE_GET_NODE(BINARY_TREE binary_tree[static 1], const TreeNode node);
void BINARY_TREE_SET_VALUE(BINARY_TREE binary_tree[static 1], const TreeNode node, T value);
TreeNode BINARY_TREE_ADD_LEFT(BINARY_TREE binary_tree[static 1], const TreeNode parent, T value);
TreeNode BINARY_TREE_ADD_RIGHT(BINARY_TREE binary_tree[static 1], const TreeNode parent, T value);
TreeNode BINARY_TREE_GET_LEFT(BINARY_TREE binary_tree[static 1], const TreeNode parent);
TreeNode BINARY_TREE_GET_RIGHT(BINARY_TREE binary_tree[static 1], const TreeNode parent);
TreeNode BINARY_TREE_GET_PARENT(BINARY_TREE binary_tree[static 1], const TreeNode child);
TreeNode BINARY_TREE_GET_DEEPEST(BINARY_TREE binary_tree[static 1]);
unsigned int BINARY_TREE_GET_NODE_DEPTH(BINARY_TREE binary_tree[static 1], const TreeNode node);
unsigned int BINARY_TREE_GET_DEPTH(BINARY_TREE binary_tree[static 1]);
TreeNode *BINARY_TREE_GET_CHILDREN(BINARY_TREE binary_tree[static 1], const TreeNode node, size_t *const count);
TreeNode *BINARY_TREE_GET_NODES(BINARY_TREE binary_tree[static 1], size_t *const count);
size_t BINARY_TREE_GET_CHILDREN_COUNT(BINARY_TREE binary_tree[static 1], const TreeNode node);
size_t BINARY_TREE_GET_NODE_COUNT(BINARY_TREE binary_tree[static 1]);
BINARY_TREE BINARY_TREE_DETACH(BINARY_TREE binary_tree[static 1], const TreeNode node);

#if defined BINARY_TREE_PRINT_FORMAT && defined BINARY_TREE_PRINT_ARGUMENT
void BINARY_TREE_PRINT(BINARY_TREE binary_tree[static 1]);
#endif

#endif //BINARY_TREE_IMPLEMENTATION

#if 0
#define BINARY_TREE_IMPLEMENTATION
#endif

#ifdef BINARY_TREE_IMPLEMENTATION

#include <assert.h>
#include <string.h>
#include <math.h>

static void BINARY_TREE_RESIZE(BINARY_TREE binary_tree[static 1], const size_t new_capacity) {
    NODE *new_nodes = realloc(binary_tree->nodes, sizeof(NODE) * new_capacity);
    assert(new_nodes != NULL);
    memset(new_nodes + binary_tree->capacity, 0, sizeof(NODE) * (new_capacity - binary_tree->capacity));
    binary_tree->nodes = new_nodes;
    binary_tree->capacity = new_capacity;
}

inline static TreeNode BINARY_TREE_ADD_DIRECTION(BINARY_TREE binary_tree[static 1], const TreeNode parent, T value, const Direction direction) {
    assert(parent < binary_tree->capacity);
    assert(binary_tree->nodes[parent].used);
    
    const TreeNode child_index = 2*parent + (TreeNode)direction;
    if(child_index >= binary_tree->capacity) {
        BINARY_TREE_RESIZE(binary_tree, 2 * binary_tree->capacity);
    }

    NODE *const child = binary_tree->nodes + child_index;
    *child = (NODE){.used = true, .value = value};

    binary_tree->deepest = MAX(binary_tree->deepest, child_index);
    
    return child_index;
}

static TreeNode BINARY_TREE_GET_DIRECTION(BINARY_TREE binary_tree[static 1], const TreeNode parent, const Direction direction) {
    assert(parent < binary_tree->capacity);
    assert(binary_tree->nodes[parent].used);

    const TreeNode child = 2*parent + (TreeNode)direction;
    if(child >= binary_tree->capacity || !binary_tree->nodes[child].used) {
        return TreeNode_NULL;
    }

    return child;
}

void BINARY_TREE_INIT(BINARY_TREE binary_tree[static 1]) {
    binary_tree->capacity = BINARY_TREE_INITIAL_CAPACITY;
    binary_tree->nodes = calloc(BINARY_TREE_INITIAL_CAPACITY, sizeof(NODE));
    assert(binary_tree->nodes != NULL);
    binary_tree->deepest = 0;
}

void BINARY_TREE_FREE(BINARY_TREE binary_tree[static 1]) {
    free(binary_tree->nodes);
    *binary_tree = (BINARY_TREE){0};
}

void BINARY_TREE_COPY(BINARY_TREE dest[static 1], BINARY_TREE src[static 1]) {
    assert(src->capacity > 0);
    assert(src->nodes != NULL);
    assert(src->deepest < src->capacity);

    dest->nodes = calloc(src->capacity, sizeof(NODE));
    assert(dest->nodes != NULL);
    dest->capacity = src->capacity;
    dest->deepest = src->deepest;
}

inline T BINARY_TREE_GET_VALUE(BINARY_TREE binary_tree[static 1], const TreeNode node) {
    assert(node < binary_tree->capacity);
    assert(binary_tree->nodes[node].used);

    return binary_tree->nodes[node].value;
}

inline NODE *BINARY_TREE_GET_NODE(BINARY_TREE binary_tree[static 1], const TreeNode node) {
    assert(node < binary_tree->capacity);
    
    return &binary_tree->nodes[node];
}

inline void BINARY_TREE_SET_VALUE(BINARY_TREE binary_tree[static 1], const TreeNode node, T value) {
    assert(node < binary_tree->capacity);
    assert(BINARY_TREE_GET_PARENT(binary_tree, node) != TreeNode_NULL || node == 0);

    binary_tree->nodes[node].value = value;
    binary_tree->nodes[node].used = true;
}

TreeNode BINARY_TREE_ADD_LEFT(BINARY_TREE binary_tree[static 1], const TreeNode parent, T value) {
    return BINARY_TREE_ADD_DIRECTION(binary_tree, parent, value, DirectionLeft);
}

TreeNode BINARY_TREE_ADD_RIGHT(BINARY_TREE binary_tree[static 1], const TreeNode parent, T value) {
    return BINARY_TREE_ADD_DIRECTION(binary_tree, parent, value, DirectionRight);
}

TreeNode BINARY_TREE_GET_LEFT(BINARY_TREE binary_tree[static 1], const TreeNode parent) {
    return BINARY_TREE_GET_DIRECTION(binary_tree, parent, DirectionLeft);
}

TreeNode BINARY_TREE_GET_RIGHT(BINARY_TREE binary_tree[static 1], const TreeNode parent) {
    return BINARY_TREE_GET_DIRECTION(binary_tree, parent, DirectionRight);
}

TreeNode BINARY_TREE_GET_PARENT(BINARY_TREE binary_tree[static 1], const TreeNode child) {
    assert(child < binary_tree->capacity);
    assert(child == 0 || binary_tree->nodes[child - 1 / 2].used);

    return child == 0
        ? TreeNode_NULL
        : (child - 1) / 2;
}

inline TreeNode BINARY_TREE_GET_DEEPEST(BINARY_TREE binary_tree[static 1]) {
    return binary_tree->deepest;
}

inline unsigned int BINARY_TREE_GET_NODE_DEPTH(BINARY_TREE binary_tree[static 1], const TreeNode node) {
    assert(node < binary_tree->capacity);
    assert(binary_tree->nodes[node].used || node == TreeNode_ROOT);

#if SIZE64
    const long double ret = log2l(node + 1.0L);
#else
    const double ret = log2(node + 1.0);
#endif
    return (unsigned int)ret;
}

inline unsigned int BINARY_TREE_GET_DEPTH(BINARY_TREE binary_tree[static 1]) {
    return BINARY_TREE_GET_NODE_DEPTH(binary_tree, binary_tree->deepest);
}

TreeNode *BINARY_TREE_GET_CHILDREN(BINARY_TREE binary_tree[static 1], const TreeNode node, size_t *const count) {
    if(!binary_tree->nodes[node].used || binary_tree->deepest == TreeNode_ROOT) {
        *count = 0;
        return NULL;
    }
    
    size_t index = 0;
    *count = 0;
    TreeNode *nodes = malloc(sizeof(TreeNode) * binary_tree->deepest);
    TreeNode left, right;

    left = BINARY_TREE_GET_LEFT(binary_tree, node);
    if(left != TreeNode_NULL) {
        nodes[(*count)++] = left;
    }
    right = BINARY_TREE_GET_RIGHT(binary_tree, node);
    if(right != TreeNode_NULL) {
        nodes[(*count)++] = right;
    }

    while(index != *count) {
        left = BINARY_TREE_GET_LEFT(binary_tree, nodes[index]);
        if(left != TreeNode_NULL) {
            nodes[(*count)++] = left;
        }
        right = BINARY_TREE_GET_RIGHT(binary_tree, nodes[index]);
        if(right != TreeNode_NULL) {
            nodes[(*count)++] = right;
        }
        index++;
    }

    if(*count == 0) {
        free(nodes);
        return NULL;
    }

    return nodes;
}

size_t BINARY_TREE_GET_CHILDREN_COUNT(BINARY_TREE binary_tree[static 1], const TreeNode node) {   
    size_t count;
    TreeNode *nodes = BINARY_TREE_GET_CHILDREN(binary_tree, node, &count);
    free(nodes);

    return count;
}

BINARY_TREE BINARY_TREE_DETACH(BINARY_TREE binary_tree[static 1], const TreeNode node) {
    assert(node < binary_tree->capacity);
    assert(binary_tree->nodes[node].used);

    BINARY_TREE detached_tree;
    if(node == TreeNode_ROOT) {
        BINARY_TREE_COPY(&detached_tree, binary_tree);
        memset(binary_tree->nodes, 0, sizeof(NODE) * binary_tree->deepest);
        binary_tree->deepest = TreeNode_ROOT;
        return detached_tree;
    }
    BINARY_TREE_INIT(&detached_tree);

    size_t count;
    unsigned int depth = BINARY_TREE_GET_NODE_DEPTH(binary_tree, node);
    TreeNode *nodes = BINARY_TREE_GET_CHILDREN(binary_tree, node, &count);

    for(size_t i=0; i<count; i++) {
        //insertion logic here
        binary_tree->nodes[nodes[i]] = (NODE){0};
    }
    detached_tree.nodes[TreeNode_ROOT] = binary_tree->nodes[node];
    binary_tree->nodes[node] = (NODE){0};

    const TreeNode last = count == 0 ? node : nodes[count - 1];
    free(nodes);
    if(binary_tree->deepest != last) {
        return detached_tree;
    }

    //update the deepest node
    TreeNode i;
    for(i=binary_tree->deepest - 1; i != TreeNode_ROOT; i--) {
        if(binary_tree->nodes[i].used) {
            binary_tree->deepest = i;
            break;
        }
    }
    if(i == TreeNode_ROOT) {
        binary_tree->deepest = TreeNode_ROOT;
    }

    return detached_tree;
}

#if defined BINARY_TREE_PRINT_FORMAT && defined BINARY_TREE_PRINT_ARGUMENT
#define BINARY_TREE_PRINT MAKE_NAME(BINARY_TREE, print)
#endif

#endif //BINARY_TREE_IMPLEMENTATION

#undef BINARY_TREE_INITIAL_CAPACITY
#undef NODE
#undef BINARY_TREE
#undef BINARY_TREE_RESIZE
#undef BINARY_TREE_INIT
#undef BINARY_TREE_FREE
#undef BINARY_TREE_COPY
#undef BINARY_TREE_ADD_DIRECTION 
#undef BINARY_TREE_ADD_LEFT 
#undef BINARY_TREE_ADD_RIGHT 
#undef BINARY_TREE_GET_VALUE
#undef BINARY_TREE_GET_NODE
#undef BINARY_TREE_SET_VALUE
#undef BINARY_TREE_GET_ROOT
#undef BINARY_TREE_GET_DIRECTION
#undef BINARY_TREE_GET_LEFT
#undef BINARY_TREE_GET_RIGHT
#undef BINARY_TREE_GET_PARENT
#undef BINARY_TREE_GET_DEEPEST
#undef BINARY_TREE_GET_DEPTH
#undef BINARY_TREE_GET_NODE_DEPTH
#undef BINARY_TREE_GET_CHILDREN
#undef BINARY_TREE_GET_CHILDREN_LOGIC
#undef BINARY_TREE_GET_CHILDREN_COUNT
#undef BINARY_TREE_DETACH

#ifdef BINARY_TREE_IMPLEMENTATION
#undef BINARY_TREE_IMPLEMENTATION
#endif
