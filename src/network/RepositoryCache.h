/**
 * Description of the header file to be included here.
 *
 * @author ich
 * @date 22.07.23
 * @version 1.0
 */

#ifndef NIX_OPERATOR_REPOSITORYCACHE_H
#define NIX_OPERATOR_REPOSITORYCACHE_H
#include "../Utils.h"
#include "../entities/NixPackagePrefix.h"
#include "Common.h"
#include "CurlWrapper.h"
#include <string>

class RepositoryCache {
public:
    inline static constexpr const std::string_view PACKAGE_PREFIX = "/nixpkgs/";
    inline static constexpr const std::string_view PACKAGE_URL_PREFIX = JoinExpr<Common::RELEASE_BASE_URL, PACKAGE_PREFIX>::value;

private:
    const NixSelection mSelection;
    std::string mFilename;
    std::string mFilenameTmp;
    std::string mBaseUrl;
    std::string mPackageUrl;

private:
    bool packageUrlValid();
    void cachePackages(const std::string &filename);

public:
    explicit RepositoryCache(NixSelection selectionState);
    void run();
};

#endif // NIX_OPERATOR_REPOSITORYCACHE_H
