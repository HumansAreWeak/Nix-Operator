/**
 * Description of the header file to be included here.
 *
 * @author ich
 * @date 18.07.23
 * @version 1.0
 */
#include "SourceHandler.h"
#include "Config.h"
#include "entities/NixPackagePrefix.h"
#include "network/Common.h"
#include "network/CurlWrapper.h"
#include "network/SourceFetch.h"
#include <algorithm>
#include <array>
#include <chrono>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <pugixml.hpp>
#include <regex>
#include <sstream>
#include <sys/stat.h>

#define START_MARKER_URL (BASE_URL "/?delimiter=/&prefix=nixpkgs/")

void SourceHandler::storePackagesToStorage(const std::multimap<std::string, NixPackagePrefix> &packages, const char *path)
{
    std::cout << "Storing file in path " << path << std::endl;
    // Explicitly stating that we are truncating
    std::ofstream f(path, std::ios::trunc);
    std::string current;

    for (auto &pkg : packages) {
        if (pkg.first != current) {
            if (!current.empty()) {
                f << '\n';
            }
            current = pkg.first;
            f << current << '?';
        }
        else {
            f << '?';
        }

        f << pkg.second.hash << '$' << pkg.second.lastModified << '$' << pkg.second.size << '$' << pkg.second.id;
    }
}

void SourceHandler::storePackages(const std::multimap<std::string, NixPackagePrefix> &packages)
{
    // Checking with Posix functions if main cache file already exists
    struct stat buffer { };
    if (stat(NIXOP_REGISTRY.data(), &buffer) == 0) {
        storePackagesToStorage(packages, NIXOP_REGISTRY_TMP.data());
    }
    else {
        storePackagesToStorage(packages, NIXOP_REGISTRY.data());
    }
}

std::optional<std::string> SourceHandler::updateAllSources()
{
    using namespace std::chrono_literals;

    std::multimap<std::string, NixPackagePrefix> packages;

    std::string prefix = START_MARKER_URL;
    prefix += "&marker=";
    std::string url = START_MARKER_URL;
    bool needToRequestMore = true;

    // Some Regex Wizardy
    std::regex expr(R"(\b-([0-9.]*)?pre.*?([0-9]*)?\.([0-9a-z]*)?)");
    std::smatch matches;

    // TODO: Refactor the WHOLE DAMN THING
    while (needToRequestMore) {
        CurlWrapper curl;
        curl.url(url.c_str())->perform();
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_string(curl.fetchResponse());

        if (!result) {
            std::cerr << "Could not parse received XML file" << std::endl;
            return "Could not parse received XML file";
        }

        pugi::xml_node listBucketResults = doc.child("ListBucketResult");
        std::string nextMarker = listBucketResults.child_value("NextMarker");

        for (pugi::xml_node entry : listBucketResults.children("Contents")) {
            std::string fetchedKey = entry.child_value("Key");
            std::string fetchedLastModified = entry.child_value("LastModified");
            std::string version;
            std::string sId;
            std::string hash;
            std::string fetchedSize = entry.child_value("Size");

            if (std::regex_search(fetchedKey, matches, expr)) {
                if (matches.size() >= 4) {
                    version = matches[1].str();
                    sId = matches[2].str();
                    hash = matches[3].str();
                }
                else {
                    std::cerr << "The fetched key (key=" << fetchedKey << ") did not evaluated correctly with the provided regex pattern\n";
                }
            }
            else {
                std::cerr << "The fetched key (key=" << fetchedKey << ") has not found a single match with the provided regex pattern\n";
            }

            std::tm tm {};
            strptime(fetchedLastModified.c_str(), "%FT%T", &tm);
            std::time_t t = mktime(&tm);
            auto time = static_cast<unsigned long int>(t);
            unsigned int id = std::stoi(sId);
            unsigned int size = std::stoi(fetchedSize);

            packages.insert(std::make_pair(version, NixPackagePrefix(version.c_str(), hash.c_str(), time, size, id)));
        }

        if (!nextMarker.empty()) {
            url = prefix + nextMarker;
        }
        else {
            needToRequestMore = false;
        }
    }

    storePackages(packages);

    return {};
}

std::future<std::optional<std::string>> SourceHandler::fetchAllSources()
{
    return std::async(std::launch::async, [&]() { return updateAllSources(); });
}

bool SourceHandler::canMapRepositories()
{
    struct stat buffer { };
    return stat(NIXOP_REGISTRY.data(), &buffer) == 0;
}

SourceHandler::SourceHandler(std::shared_ptr<Config> &config)
    : mConfig(config)
{
    mObserverId = mConfig->registerObserver([&](const char *name, const char *value) {
        if (strcmp(name, "version") == 0)
            selectVersion(value);
        std::cout << "Observer called with name=" << name << ", value=" << value << std::endl;
    });
}

SourceHandler::~SourceHandler()
{
    mConfig->removeObserver(mObserverId);
}

void SourceHandler::mapRepositories()
{
    std::ifstream f(NIXOP_REGISTRY.data());
    std::string line;
    mRepositories.clear();
    std::vector<NixPackagePrefix> *packages;

    // What a mess ?!
    while (std::getline(f, line)) {
        std::istringstream ss(line);
        // std::cout << "Read line: " << line << std::endl;
        std::string version;
        bool readVersion = false;

        for (std::string group; std::getline(ss, group, '?');) {
            if (!readVersion) {
                version = group;
                mRepositories.insert(std::pair(version, std::vector<NixPackagePrefix>()));
                packages = &mRepositories[version];
                // 1000 elements per group reserved, so it won't copy too much under the hood
                packages->reserve(1000);
                readVersion = true;
            }
            else {
                std::istringstream iss(group);
                std::string sHash;
                std::string sLastModified;
                std::string sSize;
                std::string sId;
                int i = 0;

                for (std::string specifier; std::getline(iss, specifier, '$');) {
                    switch (i++) {
                    case 0:
                        sHash = specifier;
                        break;
                    case 1:
                        sLastModified = specifier;
                        break;
                    case 2:
                        sSize = specifier;
                        break;
                    case 3:
                        sId = specifier;
                        break;
                    default:
                        break;
                    }
                }

                packages->emplace_back(
                    version,
                    sHash,
                    std::stoi(sLastModified),
                    std::stoi(sSize),
                    std::stoi(sId));
            }
        }
    }

    for (auto &rep : mRepositories) {
        auto &v = rep.second;
        std::sort(v.begin(), v.end(), [](const NixPackagePrefix &p1, const NixPackagePrefix &p2) { return p1.lastModified > p2.lastModified; });
    }
}

void SourceHandler::selectVersion(const std::string &version)
{
    unsigned long pos = version.find('_');
    std::string nextVersion = version.substr(0, pos);
    std::string hash = version.substr(pos + 1);

    auto search = mRepositories.find(nextVersion);
    if (search == mRepositories.end())
        return selectVersion("23.05_latest");

    long offset = 0;

    if (hash != "latest") {
        auto it = std::find_if(search->second.begin(), search->second.end(), [&](const NixPackagePrefix &p) { return p.hash == hash; });
        offset = std::distance(search->second.begin(), it);
    }

    mSelection = NixSelection(
        nextVersion,
        hash,
        offset);
}

NixSelection &SourceHandler::getSelection()
{
    auto s = mRepositories.find(mSelection.version);
    mSelection.pkg = &s->second[mSelection.offset];
    return mSelection;
}
