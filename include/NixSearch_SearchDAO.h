//
// Created by ich on 16.07.23.
//
#ifndef NIX_OPERATOR_NIXSEARCH_SEARCHDAO_H
#define NIX_OPERATOR_NIXSEARCH_SEARCHDAO_H
#include "SearchDAO.h"
#include <fstream>


class NixSearch_SearchDAO : public SearchDAO
{
private:
    std::string mVersion;

public:
    ~NixSearch_SearchDAO() override = default;
    std::future<std::vector<Package>> getPackagesByQuery(const std::string &query) override;
    void setVersion(const std::string &version) override;
};


#endif //NIX_OPERATOR_NIXSEARCH_SEARCHDAO_H
