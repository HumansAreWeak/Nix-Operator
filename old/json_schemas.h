//
// Created by ich on 13.07.23.
//

#ifndef NIX_OPERATOR_JSON_SCHEMAS_H
#define NIX_OPERATOR_JSON_SCHEMAS_H
#include <json-c/json.h>

json_object* json_schemas_request_payload();

void json_schemas_clean(json_object *json);

#endif //NIX_OPERATOR_JSON_SCHEMAS_H
