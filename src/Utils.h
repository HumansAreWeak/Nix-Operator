/**
 * Description of the header file to be included here.
 *
 * @author ich
 * @date 17.07.23
 * @version 1.0
 */

#ifndef NIX_OPERATOR_UTILS_H
#define NIX_OPERATOR_UTILS_H
#include <json/json.h>

enum class SourceIdentifier {
    UNKNOWN = (0 << 0),
    NIXOS_SEARCH = (1 << 0),
};

class Utils {
public:
    static const char *asCString(const Json::Value &val);
    static std::string asString(const Json::Value &val);
};

#endif // NIX_OPERATOR_UTILS_H
