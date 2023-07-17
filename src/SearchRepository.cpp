/**
 * Search Repository implementation
 *
 * @author Maik Steiger
 * @date 17.07.23
 * @version 1.0
 */
#include "SearchRepository.h"

SearchRepository::SearchRepository(std::unique_ptr<SearchDAO> searchDAO,
    std::unique_ptr<SimpleCache> packageCache,
    std::unique_ptr<SimpleCache> optionCache,
    std::unique_ptr<SimpleCache> flakeCache)
    : mSearchDAO(std::move(searchDAO))
    , mPackageCache(std::move(packageCache))
    , mOptionCache(std::move(optionCache))
    , mFlakeCache(std::move(flakeCache))
{
}

SearchRepository::SearchRepository(std::unique_ptr<SearchDAO> searchDAO, std::unique_ptr<SimpleCache> packageCache,
    std::unique_ptr<SimpleCache> optionCache, std::unique_ptr<SimpleCache> flakeCache,
    const std::string &initialVersion)
    : mSearchDAO(std::move(searchDAO))
    , mPackageCache(std::move(packageCache))
    , mOptionCache(std::move(optionCache))
    , mFlakeCache(std::move(flakeCache))
{
    setVersion(initialVersion);
}

void SearchRepository::setVersion(const std::string &version)
{
    mPackageCache->setVersion(version);
    mOptionCache->setVersion(version);
    mFlakeCache->setVersion(version);
    mSearchDAO->setVersion(version);
}

std::future<std::vector<Package>> SearchRepository::getPackagesFromQuery(const std::string &query)
{
    if (mPackageCache->has(query))
        mPackageCache->get(query); // TODO: What to do with the raw data?
    return mSearchDAO->getPackagesByQuery(query);
}
