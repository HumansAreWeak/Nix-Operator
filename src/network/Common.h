/**
 * Here are common networking utilities, that are shared between instances.
 * All utilities are static and should have no side-effects.
 *
 * @author Maik Steiger
 * @date 18.07.23
 * @version 1.0
 */

#ifndef NIX_OPERATOR_COMMON_H
#define NIX_OPERATOR_COMMON_H
#include <string>

struct NixSelection {
    std::string version;
    std::string hash;
    unsigned long offset;
    NixPackagePrefix pkg;

    inline NixSelection(const std::string &VERSION, const std::string &HASH, unsigned long OFFSET)
        : version(VERSION.c_str())
        , hash(HASH.c_str())
        , offset(OFFSET)
        , pkg(NixPackagePrefix::zero())
    {
    }

    inline NixSelection()
        : version("")
        , hash("")
        , offset(0)
        , pkg(NixPackagePrefix::zero())
    {
    }

    [[nodiscard]] inline std::string toString() const
    {
        std::ostringstream ss;
        ss << "NixSelection(version=" << version << ", hash=" << hash << ", offset=" << offset << ", pkg=" << pkg.toString() << ")";
        return ss.str();
    }
};

class Common {
public:
    inline static constexpr const std::string_view S3_BASE_URL = "https://nix-releases.s3.amazonaws.com";
    inline static constexpr const std::string_view RELEASE_BASE_URL = "https://releases.nixos.org";
};

#endif // NIX_OPERATOR_COMMON_H
