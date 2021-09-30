project(HelloCoroutines)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-fcoroutines")

add_executable(HelloCoroutines hello_coroutines.cpp)


