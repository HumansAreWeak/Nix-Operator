/**
 * RepositoryFactory implementation
 *
 * @author Maik Steiger
 * @date 17.07.23
 * @version 1.0
 */
#include "RepositoryFactory.h"
#include "NixSearch_SearchDAO.h"
#include "SimpleCache.h"

std::shared_ptr<SearchRepository> RepositoryFactory::getSearchRepository()
{
    return std::make_unique<SearchRepository>(
        std::make_unique<NixSearch_SearchDAO>(),
        std::make_unique<SimpleCache>(),
        std::make_unique<SimpleCache>(),
        std::make_unique<SimpleCache>(),
        "23.05");
}
