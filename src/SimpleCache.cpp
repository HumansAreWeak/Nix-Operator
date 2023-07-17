/**
 * SimpleCache implementation
 *
 * @author Maik Steiger
 * @date 17.07.23
 * @version 1.0
 */
#include "SimpleCache.h"

template <typename T>
SimpleCache::SimpleCache() = default;

template <typename T>
void SimpleCache::setVersion(const std::string &version)
{
    mVersion = version;
}

bool SimpleCache::has(const std::string &key) const
{
    return mMap.contains(getKey(key));
}

std::string SimpleCache::getKey(const std::string &key) const
{
    return key + " " + mVersion;
}

const char *SimpleCache::get(const std::string &key) const
{
    return mMap.at(key);
}

void SimpleCache::put(const std::string &key, const char *val)
{
    mMap[key] = val;
}
