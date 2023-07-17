//
// Created by ich on 16.07.23.
//

#ifndef NIX_OPERATOR_REPOSITORYFACTORY_H
#define NIX_OPERATOR_REPOSITORYFACTORY_H
#include <memory>
#include "SearchRepository.h"

class RepositoryFactory
{
public:
    std::shared_ptr<SearchRepository> getSearchRepository();
};


#endif //NIX_OPERATOR_REPOSITORYFACTORY_H
