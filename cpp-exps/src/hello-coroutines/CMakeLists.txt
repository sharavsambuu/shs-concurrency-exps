project(HelloCoroutines)

#FIND_PACKAGE(Boost COMPONENTS REQUIRED)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "")

#include_directories(${Boost_INCLUDE_DIR})
#link_directories(${Boost_LIBRARY_DIR})

add_executable(HelloCoroutines hello_coroutines.cpp)
#target_link_libraries(HelloCoroutines ${Boost_LIBRARIES})


