# Standalone CMake file
# Entry point for customers
# Does not change development environment

cmake_minimum_required(VERSION 3.24)
project(RayTracerChallenge-Exe)

set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)

set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

# If using Conan with boost:shared=False, set -DUSE_STATIC_BOOST when invoking cmake
if(USE_STATIC_BOOST)
    set(Boost_USE_STATIC_LIBS ON)
else()
    set(Boost_USE_STATIC_LIBS OFF)
    add_definitions(-DBOOST_LOG_DYN_LINK)  # Use dynamic linking for boost::log
endif()

find_package(Boost 1.81.0 COMPONENTS filesystem program_options log log_setup REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_subdirectory(lib)

include(GNUInstallDirs)

set(PROGRAMS_SRC
        ray_tracer_challenge.cpp
        chapter2.cpp
        chapter3.cpp
        chapter4.cpp
        chapter5.cpp
        chapter6.cpp
        chapter7.cpp
        chapter8.cpp
        chapter9.cpp
        chapter10_stripes.cpp
        chapter10_gradients.cpp
        chapter10_rings.cpp
)

foreach (FILE ${PROGRAMS_SRC})
    string(REPLACE ".cpp" "" TARGET_NAME "${FILE}")

    add_executable(${TARGET_NAME} ${FILE})
    target_link_libraries(${TARGET_NAME} PRIVATE Threads::Threads ${Boost_LIBRARIES} RayTracerChallenge-Lib)
    target_compile_features(${TARGET_NAME} INTERFACE cxx_std_20)
    target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(${TARGET_NAME} PRIVATE "$<$<CONFIG:Debug>:-O0>")
    install(TARGETS ${TARGET_NAME} EXPORT RayTracerChallengeTargets)
endforeach ()
