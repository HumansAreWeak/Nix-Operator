/**
 * Description of the header file to be included here.
 *
 * @author Maik Steiger
 * @date 19.07.23
 * @version 1.0
 */

#ifndef NIX_OPERATOR_CONFIG_H
#define NIX_OPERATOR_CONFIG_H
#include "Cache.h"
#include "Utils.h"
#include <fstream>
#include <functional>
#include <string>
#include <unordered_map>

class Config {
private:
    inline static unsigned int CONFIG_ID = 0;
    using Callback = std::function<void(const char *name, const char *value)>;

    // TODO: Need some rework
    inline static constexpr const std::string_view CONFIG_FILE_NAME = "nixop_rc";
    inline static constexpr const std::string_view CONFIG_TMP_FILE_NAME = "nixop_rc_tmp";

    inline static constexpr const std::string_view CONFIG_FILE = JoinExpr<Cache::NIXOP_CACHE_DIR, CONFIG_FILE_NAME>::value;
    inline static constexpr const std::string_view CONFIG_TMP_FILE = JoinExpr<Cache::NIXOP_CACHE_DIR, CONFIG_TMP_FILE_NAME>::value;

private:
    std::unordered_map<unsigned int, Callback> mObservers;
    std::fstream mFStream;

private:
    /**
     * Returns an output filestream, pointing to the temporary configuration file
     * @return Stream to temporary config file
     */
    std::ofstream getTempConfigStream();

    /**
     * Checks if the stream is open (file exists)
     * @return Is the stream open
     */
    bool isStreamOpen();

    /**
     * WARNING: The argument [res] gets mutated!
     * Stores the value of the line starting with [startingWith] into the argument [res].
     *
     * @param startingWith Search for line starting with this parameter
     * @param res The mutable string that will get its value after value has been found
     * @return true if a line was found, otherwise false
     */
    bool getOptionWhereLineStartsWith(const std::string &startingWith, std::string &res);

    /**
     * Writes the config with option and value to the configuration file. It includes testing,
     * like if the file exists. If the file already contains the entry, it'll be overridden and
     * notifies its observers.
     * @param optionName
     * @param value
     */
    void writeToConfig(const char *optionName, const char *value);

    /**
     * Notifies all the registered observers with the provided name and value attributes
     * @param name Name of the option that observers should know about
     * @param value Value of the option that observers should know about
     */
    void notifyAll(const char *name, const char *value);

    /**
     * Applies the preconfigured temporary configuration file by renaming it to the original file,
     * effectively applying the changes that have been made.
     */
    void applyTempConfig();

    /**
     * Checks if the configuration file already exists and creates a new one with optionName and value
     * as first entry
     * @param optionName
     * @param value
     * @return If the file already existed or not
     */
    bool createFileIfNotExist(const char *optionName, const char *value);

    /**
     * Purely writes option with value to the config file, without any checking before hand
     * @param optionName
     * @param value
     */
    void writeRawConfig(const char *optionName, const char *value);

public:
    Config();
    std::string getString(const char *optionName, const char *def);
    void setString(const char *optionName, const char *value);

public:
    void notifyAll();
    unsigned int registerObserver(Callback observer);
    bool removeObserver(unsigned int id);
};

#endif // NIX_OPERATOR_CONFIG_H
