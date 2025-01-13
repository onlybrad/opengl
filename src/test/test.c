#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

typedef const char *str;

static void vector_test(void);
static void hashmap_test(void);
static void queue_test(void);
static void binary_tree_test(void);

int main(void) {  
    vector_test();
    hashmap_test();
    queue_test();
    binary_tree_test();
    return 0;
}

#define T int
#define VECTOR_PRINT_FORMAT
#define VECTOR_PRINT_ARGUMENTS
#include "../core/Template/Vector.h"

#define T int
#define VECTOR_IMPLEMENTATION
#define VECTOR_PRINT_FORMAT "%i" 
#define VECTOR_PRINT_ARGUMENTS(i) i
#include "../core/Template/Vector.h"

#define T str
#define VECTOR_PRINT_FORMAT
#define VECTOR_PRINT_ARGUMENTS
#include "../core/Template/Vector.h"

#define T str
#define VECTOR_IMPLEMENTATION
#define VECTOR_PRINT_FORMAT "%s" 
#define VECTOR_PRINT_ARGUMENTS(s) s
#include "../core/Template/Vector.h"

static void vector_test(void) {
    Vector_int vector;
    Vector_int_init(&vector, 0);

    for(int i=0; i<100; i++) {
        Vector_int_push(&vector, i);
        assert(Vector_int_at(&vector, (size_t)i) == i);
    }

    for(int i=99; i>=0; i--) {
        const int val = Vector_int_pop(&vector);
        assert(val == i);
    }

    for(int i=0; i<10; i++) {
        Vector_int_push(&vector, i);
    }
    Vector_int_set(&vector, 5, 999);
    for(size_t i=0; i<vector.length; i++) {
        if(i == 5) {
            assert(vector.buffer[i] == 999);
        } else {
            assert(vector.buffer[i] == (int)i);
        }
    }
    Vector_int_set(&vector, 5, 5);

    Vector_int_set(&vector, 10, 888);
    for(size_t i=0; i<vector.length; i++) {
        if(i == 10) {
            assert(vector.buffer[i] == 888);
        } else {
            assert(vector.buffer[i] == (int)i);
        }
    }
    Vector_int_set(&vector, 10, 10);

    Vector_int_set(&vector, 50, 777);
    for(size_t i=0; i<vector.length; i++) {
        if(i <= 10) {
            assert(vector.buffer[i] == (int)i);
        } else if(i != 50) {
            assert(vector.buffer[i] == 0);
        } else {
            assert(vector.buffer[50] == 777);
        }
    }
    Vector_int_set(&vector, 50, 0);

    Vector_int_set(&vector, 200, 666);
    for(size_t i=0; i<vector.length; i++) {
        if(i <= 10) {
            assert(vector.buffer[i] == (int)i);
        } else if(i < 200) {
            assert(vector.buffer[i] == 0);
        } else {
            assert(vector.buffer[i] == 666);
        }
    }

    vector.length = 0;
    for(int i=0; i<10; i++) {
        Vector_int_push(&vector, i);
    }
    Vector_int_remove(&vector, 5);

    for(size_t i=0; i<vector.length; i++) {
        assert(i >= 5 
            ? vector.buffer[i] == (int)i+1 
            : vector.buffer[i] == (int)i
        );
    }   

    char* vector_str = Vector_int_to_string(&vector);
    assert(strcmp("[0, 1, 2, 3, 4, 6, 7, 8, 9]", vector_str) == 0);
    free(vector_str);
    
    Vector_int_free(&vector);

    static str vector_chars = "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";
    Vector_str vector2;
    Vector_str_init(&vector2, 0);

    for(int i=0; i<100; i++) {
        Vector_str_push(&vector2, vector_chars+i);
        assert(Vector_str_at(&vector2, (size_t)i) == vector_chars+i);
    }

    for(int i=99; i>=0; i--) {
        str val = Vector_str_pop(&vector2);
        assert(val[0] == vector_chars[i]);
    }

    for(int i=0; i<10; i++) {
        Vector_str_push(&vector2, vector_chars+i);
    }
    Vector_str_set(&vector2, 5, "999");
    for(size_t i=0; i<vector2.length; i++) {
        if(i == 5) {
            assert(strcmp(vector2.buffer[i],"999") == 0);
        } else {
            assert(vector2.buffer[i][0] == vector_chars[i]);
        }
    }
    Vector_str_set(&vector2, 5, "5");

    Vector_str_set(&vector2, 10, "888");
    for(size_t i=0; i<vector2.length; i++) {
        if(i == 10) {
            assert(strcmp(vector2.buffer[i],"888") == 0);
        } else {
            assert(vector2.buffer[i][0] == vector_chars[i]);
        }
    }
    Vector_str_set(&vector2, 10, "0");

    Vector_str_set(&vector2, 50, "777");
    for(size_t i=0; i<vector2.length; i++) {
        if(i <= 10) {
            assert(vector2.buffer[i][0] == vector_chars[i]);
        } else if(i != 50) {
            assert(vector2.buffer[i] == NULL);
        } else {
            assert(strcmp(vector2.buffer[50],"777") == 0);
        }
    }
    Vector_str_set(&vector2, 50, NULL);

    Vector_str_set(&vector2, 200, "666");
    for(size_t i=0; i<vector2.length; i++) {
        if(i <= 10) {
            assert(vector2.buffer[i][0] == vector_chars[i]);
        } else if(i < 200) {
            assert(vector2.buffer[i] == NULL);
        } else {
            assert(strcmp(vector2.buffer[i],"666") == 0);
        }
    }

    vector2.length = 0;
    for(int i=0; i<10; i++) {
        Vector_str_push(&vector2, vector_chars + i);
    }
    Vector_str_remove(&vector2, 5);

    for(size_t i=0; i<vector2.length; i++) {
        assert(i >= 5 
            ? vector2.buffer[i][0] == vector_chars[i+1] 
            : vector2.buffer[i][0] == vector_chars[i]
        );
    }

    vector2.length = 0;
    static str words[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    for(int i=0; i<10; i++) {
        Vector_str_push(&vector2, words[i]);
    }

    char* vector_str2 = Vector_str_to_string(&vector2);
    assert(strcmp("[zero, one, two, three, four, five, six, seven, eight, nine]", vector_str2) == 0);
    free(vector_str2);

    Vector_str_free(&vector2);
}

#define K str
#define V int
#define HASHMAP_PRINT_KEY_FORMAT
#define HASHMAP_PRINT_VALUE_FORMAT 
#define HASHMAP_PRINT_KEY_ARGUMENTS
#define HASHMAP_PRINT_VALUE_ARGUMENTS
#include "../core/Template/HashMap.h"

#define K str
#define V int
#define HASHMAP_PRINT_KEY_FORMAT "%s" 
#define HASHMAP_PRINT_VALUE_FORMAT "%d" 
#define HASHMAP_PRINT_KEY_ARGUMENTS(k) k
#define HASHMAP_PRINT_VALUE_ARGUMENTS(v) v
#define HASHMAP_IMPLEMENTATION
#include "../core/Template/HashMap.h"

static void hashmap_test(void) {
    HashMap_str_int hashmap;
    HashMap_str_int_init(&hashmap, string_hash_function, string_compare);

    HashMap_str_int_insert(&hashmap, "one", 1);
    HashMap_str_int_insert(&hashmap, "two", 2);
    HashMap_str_int_insert(&hashmap, "three", 3);
    HashMap_str_int_insert(&hashmap, "four", 4);
    HashMap_str_int_insert(&hashmap, "five", 5);

    char *hashmap_str = HashMap_str_int_to_string(&hashmap);
    assert(strcmp(hashmap_str, "[three => 3, four => 4, five => 5, two => 2, one => 1]") == 0);
    free(hashmap_str);

    bool success;
    int value;

    HashMap_str_int_get(&hashmap, "zero", &success);
    assert(!success);
    value = HashMap_str_int_get(&hashmap, "one", &success);
    assert(success && value == 1);
    value = HashMap_str_int_get(&hashmap, "two", &success);
    assert(success && value == 2);
    value = HashMap_str_int_get(&hashmap, "three", &success);
    assert(success && value == 3);
    value = HashMap_str_int_get(&hashmap, "four", &success);
    assert(success && value == 4);
    value = HashMap_str_int_get(&hashmap, "five", &success);
    assert(success && value == 5);

    HashMap_str_int_remove(&hashmap, "three");

    assert(HashMap_str_int_exists(&hashmap, "one"));
    assert(HashMap_str_int_exists(&hashmap, "two"));
    assert(! HashMap_str_int_exists(&hashmap, "three"));
    assert(HashMap_str_int_exists(&hashmap, "four"));
    assert(HashMap_str_int_exists(&hashmap, "five"));

    static str hashmap_chars[] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t"};

    for(int i=0; i<(int)ARRAY_LEN(hashmap_chars); i++) {
        HashMap_str_int_insert(&hashmap, hashmap_chars[i], i);
    }

    for(int i=0; i<(int)ARRAY_LEN(hashmap_chars); i++) {
        value = HashMap_str_int_get(&hashmap, hashmap_chars[i], &success);
        assert(success && value == i);
    }

    HashMap_str_int_free(&hashmap);
}

#define T str
#define QUEUE_PRINT_FORMAT
#define QUEUE_PRINT_ARGUMENTS
#include "../core/Template/Queue.h"

#define T str
#define QUEUE_PRINT_FORMAT "%s"
#define QUEUE_PRINT_ARGUMENTS(s) s
#define QUEUE_IMPLEMENTATION
#include "../core/Template/Queue.h"

static void queue_test(void) {
    Queue_str queue;
    Queue_str_init(&queue);

    bool success;
    Queue_str_dequeue(&queue, &success);
    assert(!success);

    Queue_str_enqueue(&queue, "Hello");
    assert(strcmp(Queue_str_dequeue(&queue, &success), "Hello") == 0);
    Queue_str_dequeue(&queue, &success);
    assert(!success);

    static str queue_words[] = {"Hello", "World", "A", "B", "C", "1", "2", "3", "ABC", "DEF", "XYZ", "IOP", "HELLO WORLD!", "SOME", "STRING", "WORD", "TEST", "QUEUE", "HI"};

    for(int i=0; i<(int)ARRAY_LEN(queue_words); i++) {
        Queue_str_enqueue(&queue, queue_words[i]);
    }

    char *queue_str = Queue_str_to_string(&queue);
    assert(strcmp(queue_str, "Hello -> World -> A -> B -> C -> 1 -> 2 -> 3 -> ABC -> DEF -> XYZ -> IOP -> HELLO WORLD! -> SOME -> STRING -> WORD -> TEST -> QUEUE -> HI") == 0);
    free(queue_str);

    for(int i=0; i<(int)ARRAY_LEN(queue_words); i++) {
        str result = Queue_str_dequeue(&queue, &success);
        assert(success && strcmp(result, queue_words[i]) == 0);
    }
    Queue_str_dequeue(&queue, &success);
    assert(! success);

    Queue_str_free(&queue);
}

#define T str
#include "../core/Template/BinaryTree.h"

#define T str
#define BINARY_TREE_IMPLEMENTATION
#include "../core/Template/BinaryTree.h"

static void binary_tree_test(void) {
    BinaryTree_str tree;
    BinaryTree_str detached;
    BinaryTree_str_init(&tree);
    assert(BinaryTree_str_get_children_count(&tree, TreeNode_ROOT) == 0);
    BinaryTree_str_set_value(&tree, TreeNode_ROOT, "");

    const size_t root_depth = BinaryTree_str_get_node_depth(&tree, TreeNode_ROOT);
    const size_t tree_depth = BinaryTree_str_get_depth(&tree);
    assert(root_depth == 0);
    assert(root_depth == tree_depth);

    const TreeNode l = BinaryTree_str_add_left(&tree,TreeNode_ROOT,"l");
    const TreeNode r = BinaryTree_str_add_right(&tree,TreeNode_ROOT,"r");
    assert(tree.deepest == r);
    assert(BinaryTree_str_get_children_count(&tree,TreeNode_ROOT) == 2);

    const TreeNode ll = BinaryTree_str_add_left(&tree,l,"ll");
    const TreeNode lr = BinaryTree_str_add_right(&tree,l,"lr");
    assert(tree.deepest == lr);
    assert(BinaryTree_str_get_children_count(&tree,TreeNode_ROOT) == 4);
    const TreeNode rl = BinaryTree_str_add_left(&tree,r,"rl");
    const TreeNode rr = BinaryTree_str_add_right(&tree,r,"rr");
    assert(tree.deepest == rr);
    assert(BinaryTree_str_get_children_count(&tree,TreeNode_ROOT) == 6);

    const TreeNode lll = BinaryTree_str_add_left(&tree,ll,"lll");
    const TreeNode llr = BinaryTree_str_add_right(&tree,ll,"llr");
    assert(tree.deepest == llr);
    assert(BinaryTree_str_get_children_count(&tree,TreeNode_ROOT) == 8);
    const TreeNode lrl = BinaryTree_str_add_left(&tree,lr,"lrl");
    const TreeNode lrr = BinaryTree_str_add_right(&tree,lr,"lrr");
    assert(tree.deepest == lrr);
    assert(BinaryTree_str_get_children_count(&tree,TreeNode_ROOT) == 10);

    const TreeNode lrrl = BinaryTree_str_add_left(&tree,lrr,"lrrl");
    assert(tree.deepest == lrrl);
    assert(BinaryTree_str_get_children_count(&tree,TreeNode_ROOT) == 11);
    const TreeNode lrrlr = BinaryTree_str_add_right(&tree,lrrl,"lrrlr");
    assert(tree.deepest == lrrlr);
    assert(BinaryTree_str_get_children_count(&tree,TreeNode_ROOT) == 12);
    const TreeNode llrl = BinaryTree_str_add_left(&tree, llr, "llrl");
    const TreeNode llrll = BinaryTree_str_add_left(&tree, llrl, "llrll");
    BinaryTree_str_add_left(&tree,TreeNode_ROOT,"new l");
    assert(BinaryTree_str_get_children_count(&tree,TreeNode_ROOT) == 14);

    assert(tree.deepest == lrrlr);

    /*
                         root                      depth = 0
                     /            \
                    /              \
                  new l               r            depth = 1
              /          \         /    \
             /            \       /      \
           ll             lr     rl       rr       depth = 2
           /  \           / \
          /    \         /   \
         lll    llr    lrl    lrr                  depth = 3
                /             /
               /             /
            llrl           lrrl                    depth = 4
            /                  \
           /                    \
          llrll                 lrrlr              depth = 5
    */ 

    assert(BinaryTree_str_get_node_depth(&tree, l) == 1);
    assert(BinaryTree_str_get_node_depth(&tree, r) == 1);
    assert(BinaryTree_str_get_parent(&tree, TreeNode_ROOT) == TreeNode_NULL);
    assert(BinaryTree_str_get_parent(&tree, l) == TreeNode_ROOT);
    assert(BinaryTree_str_get_parent(&tree, r) == TreeNode_ROOT);
    assert(BinaryTree_str_get_left(&tree, TreeNode_ROOT) == l);
    assert(BinaryTree_str_get_right(&tree, TreeNode_ROOT) == r);

    assert(BinaryTree_str_get_node_depth(&tree, ll) == 2);
    assert(BinaryTree_str_get_node_depth(&tree, lr) == 2);
    assert(BinaryTree_str_get_parent(&tree, ll) == l);
    assert(BinaryTree_str_get_parent(&tree, lr) == l);
    assert(BinaryTree_str_get_left(&tree, l) == ll);
    assert(BinaryTree_str_get_right(&tree, l) == lr);

    assert(BinaryTree_str_get_node_depth(&tree, rl) == 2);
    assert(BinaryTree_str_get_node_depth(&tree, rr) == 2);
    assert(BinaryTree_str_get_parent(&tree, rl) == r);
    assert(BinaryTree_str_get_parent(&tree, rr) == r);
    assert(BinaryTree_str_get_left(&tree, r) == rl);
    assert(BinaryTree_str_get_right(&tree, r) == rr);

    assert(BinaryTree_str_get_node_depth(&tree, lll) == 3);
    assert(BinaryTree_str_get_node_depth(&tree, llr) == 3);
    assert(BinaryTree_str_get_parent(&tree, lll) == ll);
    assert(BinaryTree_str_get_parent(&tree, llr) == ll);
    assert(BinaryTree_str_get_left(&tree, ll) == lll);
    assert(BinaryTree_str_get_right(&tree, ll) == llr);

    assert(BinaryTree_str_get_node_depth(&tree, lrl) == 3);
    assert(BinaryTree_str_get_node_depth(&tree, lrr) == 3);
    assert(BinaryTree_str_get_parent(&tree, lrl) == lr);
    assert(BinaryTree_str_get_parent(&tree, lrr) == lr);
    assert(BinaryTree_str_get_left(&tree, lr) == lrl);
    assert(BinaryTree_str_get_right(&tree, lr) == lrr);

    assert(BinaryTree_str_get_node_depth(&tree, lrrl) == 4);
    assert(BinaryTree_str_get_node_depth(&tree, lrrlr) == 5);
    assert(BinaryTree_str_get_parent(&tree, lrrl) == lrr);
    assert(BinaryTree_str_get_parent(&tree, lrrlr) == lrrl);
    assert(BinaryTree_str_get_left(&tree, lrr) == lrrl);
    assert(BinaryTree_str_get_right(&tree, lrr) == TreeNode_NULL);
    assert(BinaryTree_str_get_right(&tree, lrrl) == lrrlr);
    assert(BinaryTree_str_get_left(&tree, lrrl) == TreeNode_NULL);

    assert(BinaryTree_str_get_depth(&tree) == 5);
    detached = BinaryTree_str_detach(&tree, lrrl);
    BinaryTree_str_free(&detached);
    assert(BinaryTree_str_get_children_count(&tree,TreeNode_ROOT) == 12);
    /*
                         root                      depth = 0
                     /            \
                    /              \
                   l                  r            depth = 1
              /          \         /    \
             /            \       /      \
           ll             lr     rl       rr       depth = 2
           /  \           / \
          /    \         /   \
         lll    llr    lrl    lrr                  depth = 3
                /             
               /              
            llrl             (removed)             depth = 4
            /                  
           /                    
          llrll              (removed)             depth = 5
    */

    assert(BinaryTree_str_get_depth(&tree) == 5);
    assert(BinaryTree_str_get_left(&tree, lrr) == TreeNode_NULL);
    assert(tree.deepest == llrll);

    detached = BinaryTree_str_detach(&tree, lll);
    BinaryTree_str_free(&detached);
    assert(BinaryTree_str_get_children_count(&tree,TreeNode_ROOT) == 11);
    /*
                         root                      depth = 0
                     /            \
                    /              \
                   l                  r            depth = 1
              /          \         /    \
             /            \       /      \
           ll             lr     rl       rr       depth = 2
              \           / \
               \         /   \
    (removed)  llr    lrl    lrr                   depth = 3
                /             
               /              
            llrl                                   depth = 4
            /                  
           /                    
          llrll                                    depth = 5
    */
    assert(BinaryTree_str_get_depth(&tree) == 5);
    assert(BinaryTree_str_get_left(&tree, ll) == TreeNode_NULL);
    assert(tree.deepest == llrll);

    detached = BinaryTree_str_detach(&tree, llrl);
    BinaryTree_str_free(&detached);
    assert(BinaryTree_str_get_children_count(&tree,TreeNode_ROOT) == 9);
    /*
                         root                      depth = 0
                     /            \
                    /              \
                   l                  r            depth = 1
              /          \         /    \
             /            \       /      \
           ll             lr     rl       rr       depth = 2
              \           / \
               \         /   \
               llr    lrl    lrr                   depth = 3             
                             
            (removed)                               
                                             
            (removed)                                    
    */
    assert(BinaryTree_str_get_depth(&tree) == 3);
    assert(BinaryTree_str_get_left(&tree, llr) == TreeNode_NULL);
    assert(tree.deepest == lrr);

    detached = BinaryTree_str_detach(&tree, l);
    BinaryTree_str_free(&detached);
    assert(BinaryTree_str_get_children_count(&tree,TreeNode_ROOT) == 3);
    /*
                            root                   depth = 0
                                      \
                                       \
               (removed)                 r         depth = 1
                                        / \
                                       /   \
           (removed)      (removed)   rl    rr     depth = 2


           (removed)   (removed)  (removed)             
                             
                                   
    */
    assert(BinaryTree_str_get_depth(&tree) == 2);
    assert(BinaryTree_str_get_left(&tree, TreeNode_ROOT) == TreeNode_NULL);
    assert(tree.deepest == rr);

    detached = BinaryTree_str_detach(&tree, TreeNode_ROOT);
    BinaryTree_str_free(&detached);
    assert(BinaryTree_str_get_depth(&tree) == 0);
    assert(tree.deepest == TreeNode_ROOT);
    assert(BinaryTree_str_get_children_count(&tree,TreeNode_ROOT) == 0);

    BinaryTree_str_free(&tree);
}