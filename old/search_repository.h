//
// Created by ich on 12.07.23.
//

#ifndef NIX_OPERATOR_SEARCH_REPOSITORY_H
#define NIX_OPERATOR_SEARCH_REPOSITORY_H
#include <curl/curl.h>

#define SEARCH_REPOSITORY_BUFFER_SIZE 8

typedef struct search_repository_op search_repository_op_t;

typedef struct search_repository_handler {
    size_t request_size;
    size_t response_size;
    char* request;
    char* response;

    struct curl_slist *curl_slist;
    CURL *curl;
} search_repository_handler_t;

typedef struct search_repository {
    char *version;
    int search_index;

    CURLM *curl_multihandle;
    search_repository_handler_t handlers[SEARCH_REPOSITORY_BUFFER_SIZE];
} search_repository_t;

void search_repository_create(search_repository_t *rep);
void search_repository_clean(search_repository_t *rep);
int search_repository_change_version(search_repository_t *rep, const char *newVersion);

int search_repository_setup_packages(search_repository_t *rep, const char *query);

const char* search_repository_pop_response(search_repository_t *rep);

int search_repository_perform(search_repository_t *rep);

#endif //NIX_OPERATOR_SEARCH_REPOSITORY_H
