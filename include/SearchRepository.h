//
// Created by ich on 16.07.23.
//

#ifndef NIX_OPERATOR_SEARCHREPOSITORY_H
#define NIX_OPERATOR_SEARCHREPOSITORY_H
#include "SearchDAO.h"
#include "SimpleCache.h"
#include "entities/Package.h"
#include <memory>
#include <string>
#include <vector>

class SearchRepository {
private:
    std::unique_ptr<SearchDAO> mSearchDAO;
    std::unique_ptr<SimpleCache<Package>> mPackageCache;
    std::unique_ptr<SimpleCache<const char *>> mOptionCache;
    std::unique_ptr<SimpleCache<const char *>> mFlakeCache;

public:
    SearchRepository(std::unique_ptr<SearchDAO> searchDAO,
        std::unique_ptr<SimpleCache<Package>> packageCache,
        std::unique_ptr<SimpleCache<const char *>> optionCache,
        std::unique_ptr<SimpleCache<const char *>> flakeCache);
    SearchRepository(std::unique_ptr<SearchDAO> searchDAO,
        std::unique_ptr<SimpleCache<Package>> packageCache,
        std::unique_ptr<SimpleCache<const char *>> optionCache,
        std::unique_ptr<SimpleCache<const char *>> flakeCache,
        const std::string &initialVersion);
    virtual ~SearchRepository() = default;

    std::future<std::vector<Package>> getPackagesFromQuery(const std::string &query);
    void setVersion(const std::string &version);
};

#endif // NIX_OPERATOR_SEARCHREPOSITORY_H
