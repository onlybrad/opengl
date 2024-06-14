#include <assert.h>

typedef const char *str;

static void vector_test(void);
static void hashmap_test(void);
static void queue_test(void);

int main(void) {
    vector_test();
    hashmap_test();
    queue_test();
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
#include "../core/Template/Hashmap.h"

#define K str
#define V int
#define HASHMAP_PRINT_KEY_FORMAT "%s" 
#define HASHMAP_PRINT_VALUE_FORMAT "%d" 
#define HASHMAP_PRINT_KEY_ARGUMENTS(k) k
#define HASHMAP_PRINT_VALUE_ARGUMENTS(v) v
#define HASHMAP_IMPLEMENTATION
#include "../core/Template/Hashmap.h"

static void hashmap_test(void) {
    Hashmap_str_int hashmap;
    Hashmap_str_int_init(&hashmap, string_hash_function, string_compare);

    Hashmap_str_int_insert(&hashmap, "one", 1);
    Hashmap_str_int_insert(&hashmap, "two", 2);
    Hashmap_str_int_insert(&hashmap, "three", 3);
    Hashmap_str_int_insert(&hashmap, "four", 4);
    Hashmap_str_int_insert(&hashmap, "five", 5);

    assert(strcmp(Hashmap_str_int_to_string(&hashmap), "[three => 3, four => 4, five => 5, two => 2, one => 1]") == 0);

    assert(! Hashmap_str_int_get(&hashmap, "zero").success);
    assert(Hashmap_str_int_get(&hashmap, "one").success && Hashmap_str_int_get(&hashmap, "one").value == 1);
    assert(Hashmap_str_int_get(&hashmap, "two").success && Hashmap_str_int_get(&hashmap, "two").value == 2);
    assert(Hashmap_str_int_get(&hashmap, "three").success && Hashmap_str_int_get(&hashmap, "three").value == 3);
    assert(Hashmap_str_int_get(&hashmap, "four").success && Hashmap_str_int_get(&hashmap, "four").value == 4);
    assert(Hashmap_str_int_get(&hashmap, "five").success && Hashmap_str_int_get(&hashmap, "five").value == 5);

    Hashmap_str_int_remove(&hashmap, "three");

    assert(Hashmap_str_int_exists(&hashmap, "one"));
    assert(Hashmap_str_int_exists(&hashmap, "two"));
    assert(! Hashmap_str_int_exists(&hashmap, "three"));
    assert(Hashmap_str_int_exists(&hashmap, "four"));
    assert(Hashmap_str_int_exists(&hashmap, "five"));

    static str hashmap_chars[] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t"};

    for(int i=0; i<(int)ARRAY_LEN(hashmap_chars); i++) {
        Hashmap_str_int_insert(&hashmap, hashmap_chars[i], i);
    }

    for(int i=0; i<(int)ARRAY_LEN(hashmap_chars); i++) {
        assert(Hashmap_str_int_get(&hashmap, hashmap_chars[i]).success && Hashmap_str_int_get(&hashmap, hashmap_chars[i]).value == i);
    }

    Hashmap_str_int_free(&hashmap);
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

    assert(! Queue_str_dequeue(&queue).success);

    Queue_str_enqueue(&queue, "Hello");
    assert(strcmp(Queue_str_dequeue(&queue).value, "Hello") == 0);
    assert(! Queue_str_dequeue(&queue).success);

    static str queue_words[] = {"Hello", "World", "A", "B", "C", "1", "2", "3", "ABC", "DEF", "XYZ", "IOP", "HELLO WORLD!", "SOME", "STRING", "WORD", "TEST", "QUEUE", "HI"};

    for(int i=0; i<(int)ARRAY_LEN(queue_words); i++) {
        Queue_str_enqueue(&queue, queue_words[i]);
    }

    char *queue_str = Queue_str_to_string(&queue);
    assert(strcmp(queue_str, "Hello -> World -> A -> B -> C -> 1 -> 2 -> 3 -> ABC -> DEF -> XYZ -> IOP -> HELLO WORLD! -> SOME -> STRING -> WORD -> TEST -> QUEUE -> HI") == 0);
    free(queue_str);

    for(int i=0; i<(int)ARRAY_LEN(queue_words); i++) {
        const Result_str result = Queue_str_dequeue(&queue);
        assert(result.success);
        assert(strcmp(result.value, queue_words[i]) == 0);
    }
    assert(! Queue_str_dequeue(&queue).success);

    Queue_str_free(&queue);
}