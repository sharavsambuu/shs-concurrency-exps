set(SDL2_DIR $ENV{SDL2_PATH})

message(STATUS "setting up Boost Path: $ENV{BOOST_PATH}")


set(Boost_INCLUDE_DIRS "$ENV{BOOST_PATH}")
set(Boost_INCLUDE_DIR  "$ENV{BOOST_PATH}")

if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    set(Boost_LIBRARY_DIRS "$ENV{BOOST_PATH}/lib64-msvc-14.2")
    set(Boost_LIBRARY_DIR  "$ENV{BOOST_PATH}/lib64-msvc-14.2")
else()
    set(Boost_LIBRARY_DIRS "$ENV{BOOST_PATH}/lib32-msvc-14.2")
    set(Boost_LIBRARY_DIR  "$ENV{BOOST_PATH}/lib32-msvc-14.2")
endif()
