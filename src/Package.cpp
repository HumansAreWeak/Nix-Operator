/**
 * Package implementation
 *
 * @author Maik Steiger
 * @date 17.07.23
 * @version 1.0
 */
#include "entities/Package.h"
#include <utility>

Package::Package(Type TYPE, const char *NAME, const char *VERSION, const char *ATTR_NAME, const char *ATTR_SET,
    std::vector<Platform> PLATFORMS, std::vector<License> LICENSES,
    std::vector<Maintainer> MAINTAINERS, const char *DESCRIPTION, const char *LONG_DESCRIPTION,
    std::vector<std::string> HOMEPAGES)
    : type(TYPE)
    , name(NAME)
    , version(VERSION)
    , attr_name(ATTR_NAME)
    , attr_set(ATTR_SET)
    , platforms(std::move(PLATFORMS))
    , licenses(std::move(LICENSES))
    , maintainers(std::move(MAINTAINERS))
    , description(DESCRIPTION)
    , long_description(LONG_DESCRIPTION)
    , homepages(std::move(HOMEPAGES))
{
}
