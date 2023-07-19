/**
 * Description of the header file to be included here.
 *
 * @author ich
 * @date 18.07.23
 * @version 1.0
 */
#include "SourceFetch.h"
#include "Common.h"

static bool FetchFromUrl(std::string url)
{
    return false;
}

SourceFetch::SourceFetch(const char *url)
    : mUrl(BASE_URL)
{
    mUrl.append(url);
}

std::future<bool> SourceFetch::fetchFromUrl()
{
    return std::async(std::launch::async, FetchFromUrl, mUrl);
}
