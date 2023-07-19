/**
 * Description of the header file to be included here.
 *
 * @author ich
 * @date 19.07.23
 * @version 1.0
 */
#include "Cache.h"
#include <filesystem>

void Cache::ensureCacheFolderExists()
{
    using namespace std::filesystem;
    if (!is_directory(NIXOP_CACHE_DIR) || !exists(NIXOP_CACHE_DIR)) {
        create_directories(NIXOP_CACHE_DIR);
    }
}
