//
// Created by ich on 16.07.23.
//

#ifndef NIX_OPERATOR_SEARCHDAO_H
#define NIX_OPERATOR_SEARCHDAO_H
#include <string>
#include <future>
#include <vector>
#include "entities/Package.h"

class SearchDAO
{
public:
    virtual ~SearchDAO() = default;
    virtual std::future<std::vector<Package>> getPackagesByQuery(const std::string &query) = 0;
    virtual void setVersion(const std::string &version) = 0;
};

#endif //NIX_OPERATOR_SEARCHDAO_H
