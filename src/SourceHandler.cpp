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
#include <array>
#include <chrono>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <pugixml.hpp>
#include <regex>
#include <sys/stat.h>

#define START_MARKER_URL (BASE_URL "/?delimiter=/&prefix=nixpkgs/")

#define NIXOP_REGISTRY_FILE (NIXOP_CACHE_DIR "/nixop_registries")
#define NIXOP_REGISTRY_FILE_TMP (NIXOP_CACHE_DIR "/nixop_registries_tmp")

static void storePackagesToStorage(const std::multimap<std::string, NixPackagePrefix> &packages, const char *path)
{
    std::cout << "Storing file in path " << path << std::endl;
    // Explicitly stating that we are truncating
    std::ofstream f(path, std::ios::trunc);
    std::string current;

    for (auto &pkg : packages) {
        if (pkg.first != current) {
            if (!current.empty()) {
                f << ")\n";
            }
            current = pkg.first;
            f << current << "(";
        }
        else {
            f << ")(";
        }

        f << pkg.second.hash << ";" << pkg.second.lastModified << ";" << pkg.second.size << ";" << pkg.second.id;
    }
    f << ")";
}

static void storePackages(const std::multimap<std::string, NixPackagePrefix> &packages)
{
    // Checking with Posix functions if main cache file already exists
    struct stat buffer { };
    if (stat(NIXOP_REGISTRY_FILE, &buffer) == 0) {
        storePackagesToStorage(packages, NIXOP_REGISTRY_FILE_TMP);
    }
    else {
        storePackagesToStorage(packages, NIXOP_REGISTRY_FILE);
    }
}

std::optional<std::string> updateAllSources()
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
    return std::async(std::launch::async, updateAllSources);
}
bool SourceHandler::canMapRepositories()
{
    struct stat buffer { };
    return stat(NIXOP_REGISTRY_FILE, &buffer) == 0;
}

SourceHandler::SourceHandler(std::shared_ptr<Config> &config)
    : mConfig(config)
{
    mObserverId = mConfig->registerObserver([&](const char *name, const char *value) {
        if (strcmp(name, "version"))
            mapRepositories(value);
        std::cout << "Observer called with name=" << name << ", value=" << value << std::endl;
    });
}

SourceHandler::~SourceHandler()
{
    mConfig->removeObserver(mObserverId);
}

void SourceHandler::mapRepositories(const char *version)
{
    std::ifstream f(NIXOP_REGISTRY_FILE);
    std::string line;
    // Match everything from start up to first occurrence of '(' (opening bracket)
    std::regex expr(R"(^[^\(]*)");
    std::smatch matches;

    // What a mess ?!
    while (std::getline(f, line)) {
        // Skip if no valid version number is found
        if (!std::regex_search(line, matches, expr) || matches.empty())
            continue;

        std::string hit = matches[0].str();
        unsigned long ptr = hit.size();
        unsigned long offset = 0;

        while (ptr < line.size()) {
            unsigned long start = line.find('(') + offset;
            unsigned long end = line.find(')') + offset;
            unsigned long num = end - start - 1;
            std::string current = line.substr(start + 1, num);
            ptr += end + 1;
            offset += 2 + num;

            std::cout << current << std::endl;

            int index = 0;
            int lastHit = 0;
            std::array<std::string, 4> found;

            /*

            for (int i = 0; i < current.size(); i++) {
                if (current[i] == ';') {
                    int numEl = i - lastHit;
                    found[index++] = current.substr(lastHit, numEl);
                    lastHit = i + 1;

                    if (index == 3) {
                        found[index] = current.substr(lastHit);
                        break;
                    }
                }
            }

            std::cout << "Found: \n";

            for (auto &d : found) {
                std::cout << "        " << d << "\n";
            }
             */
        }
    }
}

void SourceHandler::mapRepositories()
{
    mapRepositories(mConfig->getString("version", "23.05_latest").c_str());
}
