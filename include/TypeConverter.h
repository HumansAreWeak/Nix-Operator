//
// Created by ich on 17.07.23.
//

#ifndef NIX_OPERATOR_TYPECONVERTER_H
#define NIX_OPERATOR_TYPECONVERTER_H
#include "entities/CommonEntity.h"
#include <vector>
#include <json/json.h>

namespace TypeConverter
{
    Type nixSearchConvertIntoType(const char* type);
    std::vector<Platform> nixSearchConvertIntoPlatforms(Json::Value &json);
    std::vector<License> nixSearchConvertIntoLicenses(Json::Value &json);
    std::vector<Maintainer> nixSearchConvertIntoMaintainers(Json::Value &json);
    std::vector<std::string> nixSearchConvertIntoHomepages(Json::Value &json);
}


#endif //NIX_OPERATOR_TYPECONVERTER_H
