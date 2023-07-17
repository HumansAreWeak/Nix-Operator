//
// Created by ich on 17.07.23.
//

#include "TypeConverter.h"
#include <vector>
#include <algorithm>
#include <cstring>
#include "Utils.h"

namespace TypeConverter
{
    Type nixSearchConvertIntoType(const char* type)
    {
        std::vector<Type> res;
        std::copy_if(_Type::ALL.begin(), _Type::ALL.end(), std::back_inserter(res), [&type](const Type &e) { return strcmp(e.name, type) == 0; });
        if(!res.empty()) return res[0];
        return _Type::UNKNOWN;
    }

    std::vector<Platform> nixSearchConvertIntoPlatforms(Json::Value &json)
    {
        std::vector<Platform> res;
        for(auto &pkg : json)
        {
            for(auto &platform : _Platform::ALL)
            {
                if(strcmp(Utils::asCString(pkg), platform.name) == 0)
                {
                    res.push_back(platform);
                    break;
                }
            }
        }

        if(!res.empty()) return res;
        res.push_back(_Platform::UNKNOWN);
        return res;
    }

    std::vector<License> nixSearchConvertIntoLicenses(Json::Value &json)
    {
        std::vector<License> res;
        std::transform(json.begin(), json.end(), std::back_inserter(res), [](const Json::Value &e) {
            return License(Utils::asCString(e["url"]), Utils::asCString(e["fullName"]));
        });
        return res;
    }

    std::vector<Maintainer> nixSearchConvertIntoMaintainers(Json::Value &json)
    {
        std::vector<Maintainer> res;
        std::transform(json.begin(), json.end(), std::back_inserter(res), [](const Json::Value &e) {
            return Maintainer(Utils::asCString(e["name"]),Utils::asCString(e["github"]),Utils::asCString(e["email"]));
        });
        return res;
    }

    std::vector<std::string> nixSearchConvertIntoHomepages(Json::Value &json)
    {
        std::vector<std::string> res;
        std::transform(json.begin(), json.end(), std::back_inserter(res), [](const Json::Value &e) { return Utils::asString(e); });
        return res;
    }
}
