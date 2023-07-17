/**
 * Description of the header file to be included here.
 *
 * @author ich
 * @date 17.07.23
 * @version 1.0
 */

#include "Utils.h"

const char *Utils::asCString(const Json::Value &val)
{
    return val.isNull() ? "" : val.asCString();
}

std::string Utils::asString(const Json::Value &val)
{
    return val.isNull() ? "" : val.asString();
}
