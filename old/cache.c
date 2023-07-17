//
// Created by ich on 13.07.23.
//
#include "cache.h"
#include "hashtable.h"
#include "search_repository.h"

#define CACHE_SIZE (1024)

static h_table_t *package_cache;
static h_table_t *option_cache;

static search_repository_t search_rep;

const char* cache_get_package(const char* packageName)
{
    h_query_node_t* node = hashtable_search(package_cache, packageName);

    if(node == NULL)
    {

    }

    return node->response;
}

void cache_init()
{
    package_cache = hashtable_create_table(CACHE_SIZE);
    option_cache = hashtable_create_table(CACHE_SIZE);

    search_repository_create(&search_rep);
}

void cache_clean()
{
    hashtable_table_clean(package_cache);
    hashtable_table_clean(option_cache);

    search_repository_clean(&search_rep);
}
