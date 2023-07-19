/**
 * Description of the header file to be included here.
 *
 * @author ich
 * @date 19.07.23
 * @version 1.0
 */
#include "Config.h"
#include <cstring>
#include <filesystem>
#include <iostream>

Config::Config()
{
    Cache::ensureCacheFolderExists();
};

bool Config::getOptionWhereLineStartsWith(const char *startingWith, std::string &line)
{
    std::ifstream file(CONFIG_FILE);
    if (!file.is_open())
        return false;

    for (; std::getline(file, line);) {
        if (line.starts_with(startingWith)) {
            line = line.substr(strlen(startingWith) + 1);
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

bool Config::writeToConfig(const char *optionName, const char *value)
{
    std::ifstream f(CONFIG_FILE);

    if (!f.is_open()) {
        std::ofstream outfile(CONFIG_FILE);
        outfile << optionName << "=" << value;
        return true;
    }

    std::string line;
    std::ofstream outfile(CONFIG_FILE_TMP, std::ios::ate);

    bool found = false;
    while (std::getline(f, line)) {
        if (line.starts_with(optionName)) {
            outfile << optionName << "=" << value << "\n";
            found = true;
        }
        else {
            outfile << line;
        }
    }

    if (!found) {
        outfile << optionName << "=" << value << "\n";
    }

    rename(CONFIG_FILE_TMP, CONFIG_FILE);
    notifyAll(optionName, value);
    return found;
}

bool Config::setString(const char *optionName, const char *value)
{
    return writeToConfig(optionName, value);
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
    std::ifstream f(CONFIG_FILE);
    if (!f.is_open())
        return;

    std::string line;
    while (std::getline(f, line)) {
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
