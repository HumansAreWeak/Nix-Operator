//
// Created by ich on 13.07.23.
//

#ifndef NIX_OPERATOR_SEARCH_REPOSITORY_OP_H
#define NIX_OPERATOR_SEARCH_REPOSITORY_OP_H
#include "err_codes.h"
#include "stringplus.h"
#include <json-c/json.h>
#include <stdlib.h>
#include "json_schemas.h"

typedef struct search_repository search_repository_t;

typedef struct search_repository_op
{
    int index;
    search_repository_t *rep;
} search_repository_op_t;

void add_handle(search_repository_t *rep, CURL *curl)
{
    curl_multi_add_handle(rep->curl_multihandle, curl);
}

void remove_handle(search_repository_t *rep, CURL *curl)
{
    curl_multi_remove_handle(rep->curl_multihandle, curl);
}

int add_curl_to_buffer(search_repository_t *rep, CURL *curl)
{
    for(int i = 0; i < SEARCH_REPOSITORY_BUFFER_SIZE; i++)
    {
        if(rep->handlers[i].curl == NULL)
        {
            rep->handlers[i].curl = curl;
            return i;
        }
    }

    return ERR_CURL_BUFFER_OVERFLOW;
}

void clear_curl_buffer(search_repository_t *rep)
{
    for(int i = 0; i < SEARCH_REPOSITORY_BUFFER_SIZE; i++)
    {
        if(rep->handlers[i].curl != NULL)
        {
            curl_easy_cleanup(rep->handlers[i].curl);
            rep->handlers[i].curl = NULL;
        }
    }
}

void clear_all_handles(search_repository_t *rep)
{
    for(int i = 0; i < SEARCH_REPOSITORY_BUFFER_SIZE; i++)
    {
        if(rep->handlers[i].curl != NULL)
        {
            remove_handle(rep, rep->handlers[i].curl);
            free(rep->handlers[i].response);
            free(rep->handlers[i].request);

            if(rep->handlers[i].curl_slist != NULL)
            {
                curl_slist_free_all(rep->handlers[i].curl_slist);
                rep->handlers[i].curl_slist = NULL;
            }
        }
    }
    clear_curl_buffer(rep);
}

void set_request(search_repository_handler_t *handler, json_object* (*fn)(void), const char* search, const char* replace)
{
    // Create JSON object from a file
    json_object *json = fn();

    if(!json)
    {
        fprintf(stderr, "JSON not initialized\n");
        return;
    }

    const char* json_str = json_object_to_json_string_ext(json, JSON_C_TO_STRING_PLAIN);
    char* str = str_replace(json_str, search, replace);

    handler->request = str;
    json_schemas_clean(json);
}

#endif //NIX_OPERATOR_SEARCH_REPOSITORY_OP_H
