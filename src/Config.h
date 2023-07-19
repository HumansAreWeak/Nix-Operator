/**
 * Description of the header file to be included here.
 *
 * @author ich
 * @date 19.07.23
 * @version 1.0
 */

#ifndef NIX_OPERATOR_CONFIG_H
#define NIX_OPERATOR_CONFIG_H
#include "Cache.h"
#include <fstream>
#include <functional>
#include <string>
#include <unordered_map>

class Config {
private:
    inline static unsigned int CONFIG_ID = 0;
    using Callback = std::function<void(const char *name, const char *value)>;

    inline static constexpr const char *CONFIG_FILE = NIXOP_CACHE_DIR "/nixop_rc";
    inline static constexpr const char *CONFIG_FILE_TMP = NIXOP_CACHE_DIR "/nixop_rc_tmp";

private:
    std::unordered_map<unsigned int, Callback> mObservers;

private:
    bool getOptionWhereLineStartsWith(const char *startingWith, std::string &res);
    bool writeToConfig(const char *optionName, const char *value);
    void notifyAll(const char *name, const char *value);

public:
    Config();
    std::string getString(const char *optionName, const char *def);
    bool setString(const char *optionName, const char *value);

public:
    void notifyAll();
    unsigned int registerObserver(Callback observer);
    bool removeObserver(unsigned int id);
};

#endif // NIX_OPERATOR_CONFIG_H
