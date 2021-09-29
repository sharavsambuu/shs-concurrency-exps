project(HelloThreads)

#FIND_PACKAGE(Boost COMPONENTS REQUIRED)
FIND_PACKAGE(Threads COMPONENTS REQUIRED)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "")

#include_directories(${Boost_INCLUDE_DIR})
#link_directories(${Boost_LIBRARY_DIR})

add_executable(HelloThreads hello_threads.cpp)
target_link_libraries(HelloThreads Threads::Threads)


