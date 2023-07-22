/**
 * Description of the header file to be included here.
 *
 * @author ich
 * @date 19.07.23
 * @version 1.0
 */
#include "Cache.h"
#include <filesystem>
#include <loguru.hpp>
#include <sys/stat.h>

void Cache::ensureCacheFolderExists()
{
    using namespace std::filesystem;
    if (!is_directory(NIXOP_CACHE_DIR.data()) || !exists(NIXOP_CACHE_DIR.data())) {
        LOG_F(INFO, "%s cache directory does not exist. It will be created by the setup.", NIXOP_CACHE_DIR.data());
        create_directories(NIXOP_CACHE_DIR.data());
    }
}

bool Cache::fileExists(const std::string &filename)
{
    struct stat bf { };
    return (stat(getAbsFilepath(filename).c_str(), &bf) == 0);
}

std::string Cache::getAbsFilepath(const std::string &filename)
{
    std::ostringstream ss;
    ss << NIXOP_CACHE_DIR.data() << filename;
    return ss.str();
}
