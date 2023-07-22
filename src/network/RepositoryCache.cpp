/**
 * Description of the header file to be included here.
 *
 * @author ich
 * @date 22.07.23
 * @version 1.0
 */

#include "RepositoryCache.h"
#include "../Cache.h"
#include <cstring>
#include <fstream>
#include <loguru.hpp>
#include <utility>

RepositoryCache::RepositoryCache(NixSelection selectionState)
    : mSelection(std::move(selectionState))
{
    auto &package = mSelection.pkg;
    std::ostringstream ss;
    // ss << PACKAGE_URL_PREFIX.data() << "nixpkgs-" << package.version << "pre" << package.id << "." << package.hash;
    ss << package.version << "pre" << package.id << "." << package.hash;
    mFilename = ss.str();
    mFilenameTmp = mFilename + "_tmp";
    mBaseUrl = PACKAGE_URL_PREFIX.data();
    mBaseUrl += "nixpkgs-";
    mBaseUrl += mFilename;
    mPackageUrl = mBaseUrl + "/packages.json.br";
}

void RepositoryCache::run()
{
    if (packageUrlValid())
        cachePackages(Cache::fileExists(mFilename) ? mFilenameTmp : mFilename);
}

void RepositoryCache::cachePackages(const std::string &filename)
{
    LOG_F(INFO, "Fetching packages of %s", mPackageUrl.c_str());
    if (filename != mFilename) {
        LOG_F(INFO, "%s already exists. Saving as %s", mFilename.c_str(), filename.c_str());
    }

    std::ofstream outfile(filename, std::ios::binary | std::ios::trunc);
    CurlWrapper curl;
    curl.url(mPackageUrl)->GET()->perform();
    const char *res = curl.fetchResponse();
    outfile.write(res, strlen(res));

    LOG_F(INFO, "Content: %s", curl.fetchResponse());
}

bool RepositoryCache::packageUrlValid()
{
    CurlWrapper curl;
    return curl.url(mPackageUrl)->exists();
}
