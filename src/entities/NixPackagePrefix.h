/**
 * Description of the header file to be included here.
 *
 * @author ich
 * @date 18.07.23
 * @version 1.0
 */

#ifndef NIX_OPERATOR_NIXPACKAGEPREFIX_H
#define NIX_OPERATOR_NIXPACKAGEPREFIX_H
#include <sstream>
#include <string>

struct NixPackagePrefix {
    std::string version;
    std::string hash;
    unsigned long int lastModified;
    unsigned int size;
    unsigned int id;

    inline NixPackagePrefix(const char *VERSION, const char *HASH, unsigned long int LAST_MODIFIED, unsigned int SIZE, unsigned int ID)
        : version(VERSION)
        , hash(HASH)
        , lastModified(LAST_MODIFIED)
        , size(SIZE)
        , id(ID)
    {
    }

    inline NixPackagePrefix(std::string &VERSION, std::string &HASH, unsigned long int LAST_MODIFIED, unsigned int SIZE, unsigned int ID)
        : version(VERSION.c_str())
        , hash(HASH.c_str())
        , lastModified(LAST_MODIFIED)
        , size(SIZE)
        , id(ID)
    {
    }

    [[nodiscard]] inline std::string toString() const
    {
        std::ostringstream ss;
        ss << "NixPackagePrefix(version=" << version << ", hash=" << hash << ", last_modified=" << lastModified << ", size=" << size << ", id=" << id << ")";
        return ss.str();
    }
};

#endif // NIX_OPERATOR_NIXPACKAGEPREFIX_H
