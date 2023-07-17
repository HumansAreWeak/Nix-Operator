/**
 * Implementation of the SearchDAO for the NixSearch as source
 *
 * @author Maik Steiger
 * @date 17.07.23
 * @version 1.0
 */
#include "NixSearch_SearchDAO.h"
#include "CurlWrapper.h"
#include "TypeConverter.h"
#include "Utils.h"
#include <iostream>
#include <regex>

static constexpr const char *BEARER_TOKEN = "Basic YVdWU0FMWHBadjpYOGdQSG56TDUyd0ZFZWt1eHNmUTljU2g=";

static std::string generateBaseUrl(const std::string &version)
{
    return std::string("https://search.nixos.org/backend/latest-42-nixos-") + version + "/_search";
}

std::string readFile(const char *filepath)
{
    std::ifstream f(filepath, std::ifstream::binary);
    return std::string((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
}

inline std::string getRequestPayloadTemplate()
{
    return readFile("../assets/request_payload_template.json");
}

static std::vector<Package> getPackages(const std::string query, const std::string version)
{
    std::string requestPayload = getRequestPayloadTemplate();
    std::string search = std::regex_replace(requestPayload, std::regex(R"(\[SEARCH\_QUERY\])"), query);

    CurlWrapper curl;
    curl.setBearer(BEARER_TOKEN)
        ->url(generateBaseUrl(version).c_str())
        ->request(search.c_str())
        ->appendHeader("Content-Type: application/json")
        ->appendHeader("charset=UTF-8")
        ->perform();

    if (!curl.successful()) {
        std::cerr << "curl transaction failed: " << curl.getResult() << std::endl;
        return {};
    }

    /*
    CURL* curl = curl_easy_init();

    curl_slist *sList = curl_slist_append(nullptr, "Content-Type: application/json");
    curl_slist_append(sList, "charset=UTF-8");

    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BEARER);
    curl_easy_setopt(curl, CURLOPT_XOAUTH2_BEARER, BEARER_TOKEN);
    curl_easy_setopt(curl, CURLOPT_URL, generateBaseUrl(version).c_str());
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

    std::shared_ptr<MemoryStruct> mem = curl_get_memory_buffer();
    curl_set_request(curl, mem, search);
    curl_set_response(curl, mem);

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, sList);

    CURLcode res = curl_easy_perform(curl);

    if(res != CURLE_OK)
    {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    }

    curl_slist_free_all(sList);
    curl_easy_cleanup(curl);

    if(res != CURLE_OK) return {};
     */

    Json::Value root;
    Json::Reader reader;

    if (!reader.parse(curl.fetchResponse(), root)) {
        std::cerr << "Error parsing the string to JSON" << std::endl;
        return {};
    }

    const Json::Value &hits = root["hits"]["hits"];
    std::vector<Package> res = {};
    std::transform(hits.begin(), hits.end(), std::back_inserter(res), [](const Json::Value &e) {
        const Json::Value &pkg = e["_source"];
        return Package(
            TypeConverter::nixSearchConvertIntoType(Utils::asCString(pkg["type"])),
            Utils::asCString(pkg["package_pname"]),
            Utils::asCString(pkg["package_pversion"]),
            Utils::asCString(pkg["package_attr_name"]),
            Utils::asCString(pkg["package_attr_set"]),
            TypeConverter::nixSearchConvertIntoPlatforms(const_cast<Json::Value &>(pkg["package_platforms"])),
            TypeConverter::nixSearchConvertIntoLicenses(const_cast<Json::Value &>(pkg["package_license"])),
            TypeConverter::nixSearchConvertIntoMaintainers(const_cast<Json::Value &>(pkg["package_maintainers"])),
            Utils::asCString(pkg["package_description"]),
            Utils::asCString(pkg["package_longDescription"]),
            TypeConverter::nixSearchConvertIntoHomepages(const_cast<Json::Value &>(pkg["package_homepage"])));
    });

    return res;
}

std::future<std::vector<Package>> NixSearch_SearchDAO::getPackagesByQuery(const std::string &query)
{
    return std::async(std::launch::async, getPackages, query, mVersion);
}

void NixSearch_SearchDAO::setVersion(const std::string &version)
{
    mVersion = version;
}
