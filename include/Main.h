//
// Created by ich on 16.07.23.
//

#ifndef NIX_OPERATOR_MAIN_H
#define NIX_OPERATOR_MAIN_H
#include <memory>
#include "RepositoryFactory.h"

class Main
{
private:
    RepositoryFactory mRepositoryFactory;
    std::shared_ptr<SearchRepository> mSearchRepository;

public:
    Main() = default;
    void init();
    void run();
};

#endif //NIX_OPERATOR_MAIN_H
