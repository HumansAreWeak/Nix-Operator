/**
 * Description of the header file to be included here.
 *
 * @author ich
 * @date 19.07.23
 * @version 1.0
 */
#include "Config.h"
#include <filesystem>
#include <iostream>
#include <loguru.hpp>

Config::Config()
    : mFStream(CONFIG_FILE.data(), std::ios::in | std::ios::out | std::ios::app)
{
}

bool Config::getOptionWhereLineStartsWith(const std::string &startingWith, std::string &line)
{
    if (!isStreamOpen())
        return false;

    for (; std::getline(mFStream, line);) {
        if (line.starts_with(startingWith)) {
            line = line.substr(startingWith.size() + 1);
            return true;
        }
    }
    return false;
}

std::string Config::getString(const char *optionName, const char *def)
{
    std::string res;
    if (getOptionWhereLineStartsWith(optionName, res))
        return res;

    writeToConfig(optionName, def);
    return def;
}

void Config::writeToConfig(const char *optionName, const char *value)
{
    createFileIfNotExist();
    writeRawConfig(optionName, value);
    applyTempConfig();
    notifyAll(optionName, value);
}

void Config::setString(const char *optionName, const char *value)
{
    writeToConfig(optionName, value);
}

unsigned int Config::registerObserver(Config::Callback observer)
{
    mObservers.emplace(++CONFIG_ID, observer);
    return CONFIG_ID;
}

bool Config::removeObserver(unsigned int key)
{
    const auto it = mObservers.find(key);
    if (it == mObservers.end())
        return false;
    mObservers.erase(it);
    return true;
}

void Config::notifyAll()
{
    if (!isStreamOpen())
        return;

    for (std::string line; std::getline(mFStream, line);) {
        auto pos = line.find('=');
        notifyAll(line.substr(0, pos).c_str(), line.substr(pos + 1).c_str());
    }
}

void Config::notifyAll(const char *name, const char *value)
{
    for (auto &observer : mObservers) {
        observer.second(name, value);
    }
}

bool Config::isStreamOpen()
{
    return mFStream.is_open();
}

void Config::applyTempConfig()
{
    rename(CONFIG_TMP_FILE.data(), CONFIG_FILE.data());
}

std::ofstream Config::getTempConfigStream()
{
    return std::ofstream(CONFIG_TMP_FILE.data(), std::ios::ate);
}

bool Config::createFileIfNotExist()
{
    Cache::ensureCacheFolderExists();

    // The file does not exist at this point
    if (!isStreamOpen()) {
        LOG_F(INFO, "It seems that the %s file does not exist. Creating...", CONFIG_FILE_NAME.data());
        mFStream << "";
        return true;
    }
    return false;
}

void Config::writeRawConfig(const char *optionName, const char *value)
{
    auto outfile = getTempConfigStream();
    bool match = false;

    for (std::string line; std::getline(mFStream, line);) {
        if (line.starts_with(optionName)) {
            outfile << optionName << "=" << value << "\n";
            match = true;
        }
        else {
            outfile << line;
        }
    }

    // Config was not found, so append it to the temporary stream
    if (!match) {
        outfile << optionName << "=" << value << "\n";
    }
}
