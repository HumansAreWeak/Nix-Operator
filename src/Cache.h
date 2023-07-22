/**
 * Description of the header file to be included here.
 *
 * @author Maik Steiger
 * @date 19.07.23
 * @version 1.0
 */

#ifndef NIX_OPERATOR_CACHE_H
#define NIX_OPERATOR_CACHE_H
#include <string>

class Cache {
private:
    static std::string getAbsFilepath(const std::string &filename);

public:
    inline static constexpr const std::string_view NIXOP_CACHE_DIR = "/tmp/nixop/";
    static void ensureCacheFolderExists();
    static bool fileExists(const std::string &filename);
};

#endif // NIX_OPERATOR_CACHE_H
