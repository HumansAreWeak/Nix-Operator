//
// Created by ich on 16.07.23.
//

#ifndef NIX_OPERATOR_MAIN_H
#define NIX_OPERATOR_MAIN_H
#include "Config.h"
#include "SourceHandler.h"
#include <memory>

class Main {
private:
    std::shared_ptr<Config> mConfig;
    std::shared_ptr<SourceHandler> mHandler;

public:
    Main(std::shared_ptr<Config> config, std::shared_ptr<SourceHandler> sourceHandler);
    void init();
    void runSearchScreen();
    void run();
};

#endif // NIX_OPERATOR_MAIN_H
