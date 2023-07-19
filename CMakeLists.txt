cmake_minimum_required(VERSION 3.0)
project(nix_operator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)

add_subdirectory(vendors)

file(GLOB SRC_FILES src/*.cpp src/*.h)

add_executable(nix_operator
        src/Main.cpp
        src/Main.h
        src/Utils.cpp
        src/Utils.h
        src/SourceHandler.cpp
        src/SourceHandler.h
        src/network/CurlWrapper.cpp
        src/network/CurlWrapper.h
        src/network/SourceFetch.cpp
        src/network/SourceFetch.h
        src/network/Common.h
        src/entities/NixPackagePrefix.h src/Config.cpp src/Config.h src/Cache.cpp src/Cache.h)

target_link_libraries(nix_operator PUBLIC jsoncpp pugixml ${CURL_LIBRARIES})

target_link_options(nix_operator PUBLIC -lncursesw)

