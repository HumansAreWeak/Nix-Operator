//
// Created by ich on 13.07.23.
//

#ifndef NIX_OPERATOR_HASHTABLE_H
#define NIX_OPERATOR_HASHTABLE_H
#include "komihash.h"

typedef struct h_query_node {
    char* key;
    unsigned long key_size;

    unsigned long request_size;
    char* request;
    unsigned long response_size;
    char* response;
} h_query_node_t;

typedef struct h_table {
    h_query_node_t** nodes;
    int size;
    int count;
} h_table_t;

h_query_node_t* hashtable_create_node(const char* key, size_t request_size, const char* request, size_t response_size, const char* response);
h_query_node_t* hashtable_create_node_implicit(const char* key, const char* request, const char* response);

h_table_t* hashtable_create_table(int size);
void hashtable_table_clean(h_table_t *table);

h_query_node_t* hashtable_search(h_table_t *table, const char* key);
void hashtable_insert(h_table_t *table, const char* key, const char* request, const char* response);

#endif //NIX_OPERATOR_HASHTABLE_H
