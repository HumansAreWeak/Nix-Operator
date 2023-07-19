/**
 * Description of the header file to be included here.
 *
 * @author ich
 * @date 18.07.23
 * @version 1.0
 */

#ifndef NIX_OPERATOR_SOURCEHANDLER_H
#define NIX_OPERATOR_SOURCEHANDLER_H
#include "Config.h"
#include "entities/NixPackagePrefix.h"
#include <future>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class SourceHandler {
private:
    std::shared_ptr<Config> mConfig;
    unsigned int mObserverId;
    std::multimap<std::string, std::vector<NixPackagePrefix>> repositories;

public:
    explicit SourceHandler(std::shared_ptr<Config> &config);
    ~SourceHandler();
    std::future<std::optional<std::string>> fetchAllSources();
    bool canMapRepositories();
    void mapRepositories(const char *version);
    void mapRepositories();
};

#endif // NIX_OPERATOR_SOURCEHANDLER_H
