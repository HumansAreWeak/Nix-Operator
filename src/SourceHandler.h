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
#include "Cache.h"
#include "Config.h"
#include "entities/NixPackagePrefix.h"
#include "network/Common.h"
#include <future>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class SourceHandler {
public:
    inline static constexpr const std::string_view NIXOP_REGISTRY_FILE = "nixop_registries";
    inline static constexpr const std::string_view NIXOP_REGISTRY_TMP_FILE = "nixop_registries_tmp";

    inline static constexpr const std::string_view NIXOP_REGISTRY = JoinExpr<Cache::NIXOP_CACHE_DIR, NIXOP_REGISTRY_FILE>::value;
    inline static constexpr const std::string_view NIXOP_REGISTRY_TMP = JoinExpr<Cache::NIXOP_CACHE_DIR, NIXOP_REGISTRY_TMP_FILE>::value;

    inline static constexpr const std::string_view MARKER_SUB = "/?delimiter=/&prefix=nixpkgs/";
    inline static constexpr const std::string_view START_MARKER_URL = JoinExpr<Common::S3_BASE_URL, MARKER_SUB>::value;

private:
    std::shared_ptr<Config> mConfig;
    unsigned int mObserverId;
    std::map<std::string, std::vector<NixPackagePrefix>> mRepositories;
    NixSelection mSelection;

private:
    /**
     * Stores the map of packages to the given path
     * @param packages
     * @param path
     */
    void storeRepositoriesToStorage(const std::multimap<std::string, NixPackagePrefix> &packages, const char *path);

    /**
     * Determines the path of where to store the provided packages
     * @param packages
     */
    void storeRepositories(const std::multimap<std::string, NixPackagePrefix> &packages);

    /**
     * Fetches the packages of the provided selection
     * @param selection Copy of the current selection, that will be inspected, downloaded and cached
     */
    void fetchPackages(NixSelection selection);

    /**
     * Checks if the provided version parameter exists in the cached repositories
     * @param version
     * @return
     */
    bool repositoryExists(const std::string &version);

    /**
     * Returns a copy of the cached repository given by its offset. In case of any error,
     * the NixPackagePrefix::zero() object is being returned
     * @param version
     * @param offset
     * @return
     */
    NixPackagePrefix getRepositoryByOffset(const std::string &version, unsigned long offset);

    /**
     * Updates all repository sources, by downloading and caching them
     * @return
     */
    std::optional<std::string> updateAllSources();

    /**
     * Returns the url to the given repository
     * @param package
     * @return
     */
    std::string getRepoUrl(const NixPackagePrefix &package);

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
