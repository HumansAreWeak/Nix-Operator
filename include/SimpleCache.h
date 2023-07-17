/**
 * This class is meant to act as an cache for the provided data types. It is
 * able to serialize and deserialize the given data for later usage.
 *
 * @author Maik Steiger
 * @date 17.07.23
 * @version 1.0
 */
#ifndef NIX_OPERATOR_SIMPLECACHE_H
#define NIX_OPERATOR_SIMPLECACHE_H
#include <string>
#include <unordered_map>

template <typename T>
class SimpleCache {
private:
    std::string mVersion;
    std::unordered_map<std::string, T> mMap;

private:
    std::string getKey(const std::string &key) const;

public:
    SimpleCache();
    bool has(const std::string &key) const;
    T get(const std::string &key) const;
    void put(const std::string &key, T val);

    void setVersion(const std::string &version);
};

#endif // NIX_OPERATOR_SIMPLECACHE_H
