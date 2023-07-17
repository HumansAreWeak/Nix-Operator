/**
 * Main entry of the Nix-Operator program.
 *
 * @author Maik Steiger
 * @date 17.07.23
 * @version 1.0
 */
#include "Main.h"
#include <curl/curl.h>
#include <iostream>

void Main::init()
{
    mSearchRepository = mRepositoryFactory.getSearchRepository();
}

void Main::run()
{
    auto future = mSearchRepository->getPackagesFromQuery("firefox");
    std::cout << "Received future, now waiting for it to finish." << std::endl;
    future.wait();
    auto packages = future.get();
    std::cout << "Future has finished processing. The result has a size of " << packages.size() << "." << std::endl;

    std::cout << "Printing all package names:" << std::endl;

    for (auto &package : packages) {
        std::cout << package.name << ":" << package.version << "\n";
    }
}

int main()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);

    Main main;
    main.init();
    main.run();

    curl_global_cleanup();
    return 0;
}
