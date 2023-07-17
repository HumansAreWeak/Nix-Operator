//
// Created by ich on 13.07.23.
//
#include "json_schemas.h"
#include "err_codes.h"
#include <stdio.h>

#define JSON_SCHEMA_BASE_PATH ""

static int check_not_null(json_object *json)
{
    if(json != NULL)
    {
        fprintf(stderr, "Parsed JSON structure is non-NULL\n");
        return -1;
    }
    return OK;
}

static int error_procedure(json_object *json)
{
    if(!json)
    {
        fprintf(stderr, "JSON was not gracefully initialized.\n%s", json_util_get_last_err());
        return ERR_MALLOC_NULLPTR;
    }

    return OK;
}

json_object* json_schemas_request_payload()
{
    return json_object_from_file("../request_payload_template.json");
}

void json_schemas_clean(json_object *json)
{
    json_object_put(json);
}
