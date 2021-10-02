cmake_minimum_required(VERSION 3.18)

project("cpp20-experiments")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-fcoroutines")

FIND_PACKAGE(Threads COMPONENTS REQUIRED)


if(WIN32)
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/windows-cmakes/")
endif()


include(FetchContent)

# Coop, C++20 aware cooperative multi tasking library
FetchContent_Declare(
    coop
    GIT_REPOSITORY https://github.com/jeremyong/coop.git
    GIT_TAG main
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(coop)


add_subdirectory("src/hello-threads")
add_subdirectory("src/hello-coroutines")
add_subdirectory("src/hello-fibers")

