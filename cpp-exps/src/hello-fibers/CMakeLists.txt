project(HelloFibers)


set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "-fcoroutines")


find_package(Boost COMPONENTS REQUIRED)

#set(Boost_USE_STATIC_LIBS ON)

# general fiber experiments
add_executable(HelloFibersBoost hello_fibers_boost.cpp)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})
target_link_libraries(HelloFibersBoost 
    -lboost_fiber
    -lboost_thread
    -lboost_context 
    -lboost_system 
    )


# running fabers on various threads experiment
add_executable(FiberSharingOnThreads fiber_sharing_on_threads.cpp)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})
target_link_libraries(FiberSharingOnThreads
    -lboost_fiber
    -lboost_thread
    -lboost_context 
    -lboost_system 
    -lpthread
    )
