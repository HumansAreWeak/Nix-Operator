cmake_minimum_required(VERSION 3.0)
project(nix_operator)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

find_package(CURL REQUIRED)

add_subdirectory(vendors)

file(GLOB SRC_FILES src/*.cpp include/*.h)

add_executable(nix_operator ${SRC_FILES} include/entities/Package.h include/entities/CommonEntity.h src/Package.cpp)

target_link_libraries(nix_operator PUBLIC jsoncpp ${CURL_LIBRARIES})

target_include_directories(nix_operator PUBLIC ./include ${CURL_INCLUDE_DIR})

target_link_options(nix_operator PUBLIC -lncursesw)

