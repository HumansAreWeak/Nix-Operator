/**
 * Description of the header file to be included here.
 *
 * @authors Maik Steiger
 * @date 17.07.23
 * @version 1.0
 */

#ifndef NIX_OPERATOR_PACKAGE_H
#define NIX_OPERATOR_PACKAGE_H
#include <string>
#include <vector>
#include "CommonEntity.h"

struct Package
{
    const Type type;
    std::string name;
    std::string version;
    std::string attr_name;
    std::string attr_set;
    const std::vector<Platform> platforms;
    const std::vector<License> licenses;
    const std::vector<Maintainer> maintainers;
    std::string description;
    std::string long_description;
    const std::vector<std::string> homepages;

    Package(
            Type TYPE,
            const char* NAME,
            const char* VERSION,
            const char* ATTR_NAME,
            const char* ATTR_SET,
            std::vector<Platform> PLATFORMS,
            std::vector<License> LICENSES,
            std::vector<Maintainer> MAINTAINERS,
            const char* DESCRIPTION,
            const char* LONG_DESCRIPTION,
            std::vector<std::string> HOMEPAGES
    );
};

#endif //NIX_OPERATOR_PACKAGE_H
