//
// Created by ich on 12.07.23.
//
#include "search_repository.h"
#include "search_repository_op.h"
#include <stdlib.h>
#include <string.h>
#include "json_schemas.h"

#define SEARCH_REPOSITORY_VER_CHARS (30)
#define SEARCH_REPOSITORY_BASE_URL "https://search.nixos.org"
static int globalInit = 0;

static size_t write_callback(void *data, size_t size, size_t nmemb, void *clientp)
{
    size_t realsize = size * nmemb;
    search_repository_handler_t *handler = (search_repository_handler_t *) clientp;

    char* ptr = realloc(handler->response, handler->response_size + realsize + 1);
    if(ptr == NULL)
        return 0; /* Out of memory! */


    handler->response = ptr;
    memcpy(&(handler->response[handler->response_size]), data, realsize);
    handler->response_size += realsize;
    handler->response[handler->response_size] = 0;
    return realsize;
}

static size_t read_callback(void *data, size_t size, size_t nmemb, void *clientp)
{
    size_t buf_size = size * nmemb;
    search_repository_handler_t *handler = (search_repository_handler_t *) clientp;

    if(handler->request_size)
    {
        size_t copy_this_much = handler->request_size;
        if(copy_this_much > buf_size) copy_this_much = buf_size;

        memcpy(data, handler->request, copy_this_much);
        return copy_this_much;
    }
    return 0;
}

void search_repository_create(search_repository_t *rep)
{
    // Allow 30 chars for version (+ null terminator)
    rep->version = malloc((SEARCH_REPOSITORY_VER_CHARS + 1) * sizeof(char));
    rep->curl_multihandle = curl_multi_init();
    rep->search_index = 0;

    for(int i = 0; i < SEARCH_REPOSITORY_BUFFER_SIZE; i++)
    {
        rep->handlers[i].response_size = 0;
        rep->handlers[i].response = malloc(sizeof(char));
        rep->handlers[i].curl = NULL;
        rep->handlers[i].request_size = 0;
        rep->handlers[i].request = NULL;
        rep->handlers[i].curl_slist = NULL;
    }

    stpcpy(rep->version, "23.05");

    if(globalInit == 0)
    {
        curl_global_init(3);
        globalInit++;
    }
}

void search_repository_clean(search_repository_t *rep)
{
    free(rep->version);
    curl_multi_cleanup(rep->curl_multihandle);
    clear_all_handles(rep);
}

int search_repository_change_version(search_repository_t *rep, const char* newVersion)
{
    if(strlen(newVersion) > SEARCH_REPOSITORY_VER_CHARS)
    {
        return ERR_STRING_TOO_LONG;
    }

    strcpy(rep->version, newVersion);
    return OK;
}

int search_repository_setup_packages(search_repository_t *rep, const char *query)
{
    char* url;

    if((url = malloc(513 * sizeof(char))) == NULL)
    {
        return ERR_MALLOC_NULLPTR;
    }

    url[0] = '\0';
    strcat(url, SEARCH_REPOSITORY_BASE_URL "/backend/latest-42-nixos-");
    strcat(url, rep->version);
    strcat(url, "/_search");

    CURL* curl = curl_easy_init();

    add_handle(rep, curl);
    int i;

    if((i = add_curl_to_buffer(rep, curl)) == ERR_CURL_BUFFER_OVERFLOW)
    {
        remove_handle(rep, curl);
        curl_easy_cleanup(curl);
        return ERR_CURL_BUFFER_OVERFLOW;
    }

    set_request(&rep->handlers[i], json_schemas_request_payload, "[SEARCH_QUERY]", query);
    rep->handlers[i].request_size = strlen(rep->handlers[i].request);
    rep->handlers[i].curl_slist = curl_slist_append(rep->handlers[i].curl_slist, "Content-Type: application/json");
    rep->handlers[i].curl_slist = curl_slist_append(rep->handlers[i].curl_slist, "charset=UTF-8");

    if(rep->handlers[i].curl_slist == NULL)
    {
        curl_slist_free_all(rep->handlers[i].curl_slist);
        return ERR_MALLOC_NULLPTR;
    }

    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BEARER);
    curl_easy_setopt(curl, CURLOPT_XOAUTH2_BEARER, "Bearer Basic YVdWU0FMWHBadjpYOGdQSG56TDUyd0ZFZWt1eHNmUTljU2g=");
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_READDATA, &rep->handlers[i]);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rep->handlers[i]);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, rep->handlers[i].request_size);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, rep->handlers[i].curl_slist);

    free(url);
    return OK;
}

int search_repository_perform(search_repository_t *rep)
{
    int running_handles;
    int mc = curl_multi_perform(rep->curl_multihandle, &running_handles);

    if(!mc && running_handles)
        mc = curl_multi_poll(rep->curl_multihandle, NULL, 0, 1000, NULL);

    if(mc) {
        fprintf(stderr, "curl_multi_poll() failed, code %d\n", mc);
        return 0;
    }

    return running_handles;
}

const char* search_repository_pop_response(search_repository_t *rep)
{
    for(; rep->search_index < SEARCH_REPOSITORY_BUFFER_SIZE; rep->search_index++)
    {
        if(rep->handlers[rep->search_index].curl != NULL)
        {
            return rep->handlers[rep->search_index++].response;
        }
    }
    return NULL;
}
