project(HelloThreads)

#FIND_PACKAGE(Boost COMPONENTS REQUIRED)
FIND_PACKAGE(Threads COMPONENTS REQUIRED)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "")

#include_directories(${Boost_INCLUDE_DIR})
#link_directories(${Boost_LIBRARY_DIR})

add_executable(HelloThreads hello_threads.cpp)
target_link_libraries(HelloThreads Threads::Threads)

add_executable(HelloFutures hello_futures.cpp)
target_link_libraries(HelloFutures Threads::Threads)

# Problems in threading
add_executable(ProblemoThreads problemo_threads.cpp)
target_link_libraries(ProblemoThreads Threads::Threads)
