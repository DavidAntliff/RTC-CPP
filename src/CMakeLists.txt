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

add_executable(ray_tracer_challenge ray_tracer_challenge.cpp)
target_link_libraries(ray_tracer_challenge PRIVATE Threads::Threads ${Boost_LIBRARIES} RayTracerChallenge-Lib)
target_compile_features(ray_tracer_challenge INTERFACE cxx_std_20)
target_compile_options(ray_tracer_challenge PRIVATE -Wall -Wextra -Wpedantic)
target_compile_options(ray_tracer_challenge PRIVATE "$<$<CONFIG:Debug>:-O0>")
install(TARGETS ray_tracer_challenge EXPORT RayTracerChallengeTargets)

add_executable(chapter2 chapter2.cpp)
target_link_libraries(chapter2 PRIVATE Threads::Threads ${Boost_LIBRARIES} RayTracerChallenge-Lib)
target_compile_features(chapter2 INTERFACE cxx_std_20)
target_compile_options(chapter2 PRIVATE -Wall -Wextra -Wpedantic)
target_compile_options(chapter2 PRIVATE "$<$<CONFIG:Debug>:-O0>")
install(TARGETS chapter2 EXPORT RayTracerChallengeTargets)

add_executable(chapter3 chapter3.cpp)
target_link_libraries(chapter3 PRIVATE Threads::Threads ${Boost_LIBRARIES} RayTracerChallenge-Lib)
target_compile_features(chapter3 INTERFACE cxx_std_20)
target_compile_options(chapter3 PRIVATE -Wall -Wextra -Wpedantic)
target_compile_options(chapter3 PRIVATE "$<$<CONFIG:Debug>:-O0>")
install(TARGETS chapter3 EXPORT RayTracerChallengeTargets)
