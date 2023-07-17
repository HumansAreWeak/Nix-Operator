//
// Created by ich on 13.07.23.
//
#include "hashtable.h"
#include <stdlib.h>

static void clean_node(h_query_node_t *node)
{
    free(node->key);
    free(node->request);
    free(node->response);
    free(node);
}

static int get_hash(h_table_t *table, const char* key, unsigned long key_size)
{
    return (int) (komihash(key, key_size, 0) % table->size);
}

h_query_node_t* hashtable_create_node(const char* key, size_t request_size, const char* request, size_t response_size, const char* response)
{
    h_query_node_t *node = malloc(sizeof(h_query_node_t));

    node->key_size = strlen(key);
    node->key = malloc(node->key_size);
    strcpy(node->key, key);

    node->request_size = request_size;
    node->request = malloc(node->request_size);
    strcpy(node->request, request);

    node->response_size = response_size;
    node->response = malloc(node->response_size);
    strcpy(node->response, response);

    return node;
}

h_query_node_t* hashtable_create_node_implicit(const char* key, const char* request, const char* response)
{
    return hashtable_create_node(key, strlen(request), request, strlen(response), response);
}

h_table_t* hashtable_create_table(int size)
{
    h_table_t *table = malloc(sizeof(h_table_t));
    table->size = size;
    table->count = 0;
    table->nodes = calloc(table->size, sizeof(h_query_node_t));

    for(int i = 0; i < table->size; i++) table->nodes[i] = NULL;

    return table;
}

void hashtable_table_clean(h_table_t *table)
{
    for(int i = 0; i < table->size; i++)
    {
        if(table->nodes[i] != NULL)
        {
            clean_node(table->nodes[i]);
        }
    }

    free(table->nodes);
    free(table);
}

void hashtable_insert(h_table_t *table, const char* key, const char* request, const char* response)
{
    int index = get_hash(table, key, strlen(key));
    h_query_node_t* current_node = table->nodes[index];

    if(current_node != NULL && strcmp(current_node->key, key) == 0) return;

    h_query_node_t *node = hashtable_create_node_implicit(key, request, response);

    // Not perfect, but good enough
    if(current_node == NULL)
    {
        table->nodes[index] = node;
        table->count++;
    }
    else
    {
        if(strcmp(current_node->key, node->key) != 0)
        {
            clean_node(current_node);
            table->nodes[index] = node;
        }
    }
}

h_query_node_t* hashtable_search(h_table_t *table, const char* key)
{
    int index = get_hash(table, key, strlen(key));
    return table->nodes[index];
}
