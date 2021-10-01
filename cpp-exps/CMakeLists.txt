cmake_minimum_required(VERSION 3.18)

project("cpp20-experiments")

set(CMAKE_CXX_STANDARD 20)

if(WIN32)
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/windows-cmakes/")
endif()

add_subdirectory("src/hello-threads")
add_subdirectory("src/hello-coroutines")
add_subdirectory("src/hello-fibers")

