/**
 * A small C++ wrapper to hide the nasty stuff about libcurl
 * @author Maik Steiger
 * @date 17.07.23
 */

#ifndef NIX_OPERATOR_CURLWRAPPER_H
#define NIX_OPERATOR_CURLWRAPPER_H
#include <curl/curl.h>
#include <memory>
#include <sstream>
#include <string>

struct MemoryStruct {
    char *memory;
    size_t size;
    const char *readptr;
    size_t sizeleft;

    MemoryStruct()
        : size(0)
        , memory(static_cast<char *>(malloc(1)))
        , readptr(nullptr)
        , sizeleft(0)
    {
    }

    ~MemoryStruct()
    {
        free(memory);
    }

    [[nodiscard]] inline std::string toString() const
    {
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
class CurlWrapper {
private:
private:
    CURL *mCurl;
    curl_slist *mHeaders;
    MemoryStruct mMem;
    CURLcode mLastResCode;

public:
    CurlWrapper();
    ~CurlWrapper();

    CurlWrapper *setBearer(const char *token);
    CurlWrapper *url(const char *url);
    CurlWrapper *url(const std::string &url);
    CurlWrapper *GET();
    CurlWrapper *request(const char *request);
    CurlWrapper *registerMulti(CURLM *curlm);
    CurlWrapper *removeMulti(CURLM *curlm);
    CURL *getCURL();

    /**
     * Appends the header to the headers section
     * @param content
     * @return
     */
    CurlWrapper *appendHeader(const char *content);
    CURLcode perform();

    [[nodiscard]] const char *fetchResponse() const;
    bool successful() const;
    [[nodiscard]] const char *getResult() const;

    /**
     * @return true if the configuration is valid and exists (i.e. url has status 200)
     */
    bool exists();
};

#endif // NIX_OPERATOR_CURLWRAPPER_H
