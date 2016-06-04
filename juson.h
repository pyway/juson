#ifndef _JUSON_H_
#define _JUSON_H_

#include <stdio.h>

#define JUSON_OK     (0)
#define JUSON_ERR    (-1)
#define JUSON_CHUNK_SIZE    (32)


typedef enum   juson_type   juson_type_t;
typedef struct juson_value  juson_value_t;
typedef int                 juson_int_t;
typedef float               juson_float_t;
typedef unsigned char       juson_bool_t;

enum juson_type {
    JUSON_OBJECT,
    JUSON_INT,
    JUSON_FLOAT,
    JUSON_ARRAY,
    JUSON_BOOL,
    JUSON_STRING,
    JUSON_NULL,
    JUSON_PAIR,
    JUSON_LIST,
};


struct juson_value {
    juson_type_t t;
    union {
        juson_int_t ival;
        juson_float_t fval;
        juson_bool_t bval;
        
        // String
        struct {
            char* sdata;
            int len;  
        };
        
        // Array
        struct {
            int size;
            int capacity;
            juson_value_t** adata;
        };
        
        // Object
        struct {
            juson_value_t* tail; // pair
            juson_value_t* head; // pair
        };
        
        struct {
            union {
                // Pair
                struct {
                    juson_value_t* key; // string
                    juson_value_t* val;
                };
                // List
                juson_value_t* data;
            };
            juson_value_t* next;
        };
    };
};

typedef union  juson_slot   juson_slot_t;
typedef struct juson_chunk  juson_chunk_t;
typedef struct juson_pool   juson_pool_t;

union juson_slot {
    juson_slot_t* next;
    juson_value_t val;
};

/*
 * A chunk consists of two members:
 * the pointer to the next chunk,
 * and the raw memory for values.
 * Layout of a chunk:
 * ---------------------------------------
 * next | raw memory (chunk_size * width)
 * ---------------------------------------
 */
struct juson_chunk {
    juson_chunk_t* next;
    juson_slot_t slots[JUSON_CHUNK_SIZE];
};

struct juson_pool {
    int allocated_n;
    juson_slot_t* cur;
    juson_chunk_t head;
};

typedef struct {
    char* mem;
    char* p;
    
    int line;
    
    juson_value_t* val;
    juson_value_t arr_list;
    
    juson_pool_t pool;
} juson_doc_t;

char* juson_load(char* file_name);
juson_value_t* juson_parse(juson_doc_t* doc);
juson_value_t* juson_parse_string(juson_doc_t* doc, char* str);
void juson_destroy(juson_doc_t* doc);
juson_value_t* juson_object_get(juson_value_t* obj, char* name);
juson_value_t* juson_array_get(juson_value_t* arr, size_t idx);

#endif
