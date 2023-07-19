/**
 * Main entry of the Nix-Operator program.
 *
 * @author Maik Steiger
 * @date 17.07.23
 * @version 1.0
 */
#include "Main.h"
#include "Config.h"
#include <curl/curl.h>
#include <iostream>
#include <ncurses.h>

void Main::init()
{
}

void Main::run()
{
    auto f = mHandler->fetchAllSources();

    if (!mHandler->canMapRepositories()) {
        // Wait for cache creation before accessing the cache
        f.wait();
    }

    mHandler->mapRepositories();

    // Initialize all the observers from the config values
    mConfig->notifyAll();

    /*
    auto future = mSearchRepository->getPackagesFromQuery("firefox");
    std::cout << "Received future, now waiting for it to finish." << std::endl;
    future.wait();
    auto packages = future.get();
    std::cout << "Future has finished processing. The result has a size of " << packages.size() << "." << std::endl;
    std::cout << "Printing all package names:" << std::endl;

    for (auto &package : packages) {
        std::cout << package.name << ":" << package.version << "\n";
    }
     */
    runSearchScreen();
}
void Main::runSearchScreen()
{
    // Init and clears screen
    initscr();

    int x, y, bx, by, mx, my;
    getyx(stdscr, y, x);
    getbegyx(stdscr, by, bx);
    getmaxyx(stdscr, my, mx);

    move(my - 2, mx + 2);

    // Prints a string to a window
    mvprintw(my - 2, mx - 1, "Search: ");

    // Refresh screen and match what is buffered
    refresh();

    // Waits for user input and returns it
    int c = getch();

    // Dealloc memory and ends ncurses
    endwin();
}
Main::Main(std::shared_ptr<Config> config, std::shared_ptr<SourceHandler> sourceHandler)
    : mConfig(std::move(config))
    , mHandler(std::move(sourceHandler))
{
}

int main()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);

    auto config = std::make_shared<Config>();
    auto sourceHandler = std::make_shared<SourceHandler>(config);

    Main main(config, sourceHandler);
    main.init();
    main.run();

    curl_global_cleanup();
    return 0;
}
