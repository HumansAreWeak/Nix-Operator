/**
 * A small C++ wrapper to hide the nasty stuff about libcurl
 * @author Maik Steiger
 * @date 17.07.23
 */

#ifndef NIX_OPERATOR_CURLWRAPPER_H
#define NIX_OPERATOR_CURLWRAPPER_H
#include <string>
#include <sstream>
#include <memory>
#include <curl/curl.h>

struct MemoryStruct
{
    char *memory;
    size_t size;
    const char* readptr;
    size_t sizeleft;

    MemoryStruct() : size(0), memory(static_cast<char*>(malloc(1))), readptr(nullptr), sizeleft(0) {}

    ~MemoryStruct() {
        free(memory);
    }

    [[nodiscard]] inline std::string toString() const {
        std::ostringstream ss;
        ss << "MemoryStruct(memory=";
        ss << memory;
        ss << ", size=";
        ss << size;
        ss << ", readptr=";
        ss << readptr;
        ss << ", sizeleft=";
        ss << sizeleft;
        ss << ")";
        return ss.str();
    }
};

/**
 * A teeny tiny wrapper, so I don't have to deal with cURL directly and all the manual memory allocation / read and
 * write functions myself.
 */
class CurlWrapper
{
private:
    CURL *mCurl;
    curl_slist *mHeaders;
    MemoryStruct mMem;
    CURLcode mLastResCode;

public:
    CurlWrapper();
    ~CurlWrapper();

    CurlWrapper* setBearer(const char* token);
    CurlWrapper* url(const char* url);
    CurlWrapper* request(const char* request);

    /**
     * Appends the header to the headers section
     * @param content
     * @return
     */
    CurlWrapper* appendHeader(const char* content);
    CURLcode perform();

    [[nodiscard]] const char* fetchResponse() const;
    bool successful() const;
    [[nodiscard]] const char* getResult() const;
};

#endif //NIX_OPERATOR_CURLWRAPPER_H
