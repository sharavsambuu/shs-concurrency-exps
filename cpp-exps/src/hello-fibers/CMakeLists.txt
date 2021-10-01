project(HelloFibers)


set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "-fcoroutines")


find_package(Boost COMPONENTS REQUIRED)
if(Boost_FOUND)
    message(STATUS "Boost libary is found")
endif()

add_executable(HelloFibersBoost hello_fibers_boost.cpp)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})
target_link_libraries(HelloFibersBoost PRIVATE -lboost_fiber -lboost_context -lboost_system)



