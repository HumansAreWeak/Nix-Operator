/**
 * Main handling implementation for repository selection control
 * It manages downloading, caching, as well as parsing and selecting.
 *
 * @author Maik Steiger
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

struct NixSelection {
    std::string version;
    std::string hash;
    long offset;
    NixPackagePrefix *pkg;

    inline NixSelection(const std::string &VERSION, const std::string &HASH, long OFFSET)
        : version(VERSION.c_str())
        , hash(HASH.c_str())
        , offset(OFFSET)
        , pkg(nullptr)
    {
    }

    inline NixSelection()
        : version("")
        , hash("")
        , offset(0)
        , pkg(nullptr)
    {
    }
};

class SourceHandler {
private:
    std::shared_ptr<Config> mConfig;
    unsigned int mObserverId;
    std::map<std::string, std::vector<NixPackagePrefix>> mRepositories;
    NixSelection mSelection;

private:
    void storePackagesToStorage(const std::multimap<std::string, NixPackagePrefix> &packages, const char *path);
    void storePackages(const std::multimap<std::string, NixPackagePrefix> &packages);
    std::optional<std::string> updateAllSources();

public:
    explicit SourceHandler(std::shared_ptr<Config> &config);
    ~SourceHandler();
    std::future<std::optional<std::string>> fetchAllSources();
    bool canMapRepositories();
    void mapRepositories();
    void selectVersion(const std::string &version);
    NixSelection &getSelection();
};

#endif // NIX_OPERATOR_SOURCEHANDLER_H
