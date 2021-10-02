project(HelloCoroutines)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-fcoroutines")


add_executable(HelloCoop hello_coop.cpp)
target_link_libraries(HelloCoop coop::coop Threads::Threads)


add_executable(HelloCoroutines hello_coroutines.cpp)
target_link_libraries(HelloCoroutines -fcoroutines)


find_package(Boost COMPONENTS REQUIRED)
if(Boost_FOUND)
    message(STATUS "Boost libary is found")
endif()

add_executable(HelloCoroutinesBoost hello_coroutines_boost.cpp)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})
if(WIN32)
    target_link_libraries(HelloCoroutinesBoost ${Boost_LIBRARIES})
else()
    target_link_libraries(HelloCoroutinesBoost PRIVATE -lboost_coroutine -lboost_context -lboost_system)
endif()



