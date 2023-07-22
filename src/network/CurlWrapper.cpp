/**
 * Implementation file of the CurlWrapper class definition
 *
 * @author Maik Steiger
 * @date 17.07.23
 * @version 1.0
 */
#include "CurlWrapper.h"
#include <cstring>
#include <iostream>

static size_t WriteMemoryCallback(void *data, size_t size, size_t nmemb, void *clientp)
{
    size_t realsize = size * nmemb;
    auto *mem = static_cast<MemoryStruct *>(clientp);

    char *ptr = static_cast<char *>(realloc(mem->memory, mem->size + realsize + 1));
    if (ptr == nullptr)
        return static_cast<size_t>(0); /* Out of memory! */

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), data, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}

static size_t ReadCallback(void *data, size_t size, size_t nmemb, void *clientp)
{
    auto *wt = static_cast<MemoryStruct *>(clientp);
    size_t buffer_size = size * nmemb;

    if (wt->sizeleft) {
        /* copy as much as possible from the source to the destination */
        size_t copy_this_much = wt->sizeleft;
        if (copy_this_much > buffer_size)
            copy_this_much = buffer_size;
        memcpy(data, wt->readptr, copy_this_much);

        wt->readptr += copy_this_much;
        wt->sizeleft -= copy_this_much;
        return copy_this_much; /* we copied this many bytes */
    }

    return 0; /* no more data left to deliver */
}

CurlWrapper::CurlWrapper()
    : mCurl(curl_easy_init())
    , mHeaders(nullptr)
{
}

CurlWrapper::~CurlWrapper()
{
    if (mHeaders != nullptr)
        curl_slist_free_all(mHeaders);
    curl_easy_cleanup(mCurl);
}

CURL *CurlWrapper::getCURL()
{
    return mCurl;
}

CurlWrapper *CurlWrapper::setBearer(const char *token)
{
    curl_easy_setopt(mCurl, CURLOPT_HTTPAUTH, CURLAUTH_BEARER);
    curl_easy_setopt(mCurl, CURLOPT_XOAUTH2_BEARER, token);
    return this;
}

CurlWrapper *CurlWrapper::url(const char *url)
{
    curl_easy_setopt(mCurl, CURLOPT_URL, url);
    curl_easy_setopt(mCurl, CURLOPT_WRITEDATA, &mMem);
    curl_easy_setopt(mCurl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    return this;
}

CurlWrapper *CurlWrapper::request(const char *request)
{
    mMem.readptr = request;
    mMem.sizeleft = strlen(mMem.readptr);

    curl_easy_setopt(mCurl, CURLOPT_READDATA, &mMem);
    curl_easy_setopt(mCurl, CURLOPT_READFUNCTION, ReadCallback);
    curl_easy_setopt(mCurl, CURLOPT_POST, 1L);
    curl_easy_setopt(mCurl, CURLOPT_POSTFIELDSIZE, mMem.sizeleft);
    curl_easy_setopt(mCurl, CURLOPT_POSTFIELDS, mMem.readptr);
    return this;
}

CurlWrapper *CurlWrapper::appendHeader(const char *content)
{
    if (mHeaders == nullptr) {
        mHeaders = curl_slist_append(mHeaders, content);
        curl_easy_setopt(mCurl, CURLOPT_HTTPHEADER, mHeaders);
    }
    else
        curl_slist_append(mHeaders, content);
    return this;
}

CURLcode CurlWrapper::perform()
{
    mLastResCode = curl_easy_perform(mCurl);
    return mLastResCode;
}

const char *CurlWrapper::fetchResponse() const
{
    return mMem.memory;
}

const char *CurlWrapper::getResult() const
{
    return curl_easy_strerror(mLastResCode);
}

bool CurlWrapper::successful() const
{
    return mLastResCode == CURLE_OK;
}
CurlWrapper *CurlWrapper::GET()
{
    curl_easy_setopt(mCurl, CURLOPT_HTTPGET, 1L);
    return this;
}

CurlWrapper *CurlWrapper::registerMulti(CURLM *curlm)
{
    curl_multi_add_handle(curlm, mCurl);
    return this;
}

CurlWrapper *CurlWrapper::removeMulti(CURLM *curlm)
{
    curl_multi_remove_handle(curlm, mCurl);
    return this;
}

CurlWrapper *CurlWrapper::url(const std::string &url)
{
    return this->url(url.c_str());
}

bool CurlWrapper::exists()
{
    curl_easy_setopt(mCurl, CURLOPT_NOBODY, 1);
    perform();
    return successful();
}
