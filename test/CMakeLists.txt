cmake_minimum_required(VERSION 3.24)
project(RayTracerChallengeTests)

find_package(GTest REQUIRED)

if (NOT TARGET RayTracerChallenge::Lib)
    find_package(RayTracerChallenge::Lib CONFIG REQUIRED)
endif()

find_package(Boost COMPONENTS filesystem REQUIRED)

#set(THREADS_PREFER_PTHREAD_FLAG ON)
#find_package(Threads REQUIRED)

set(SRCS
        test_tuple.cpp
        test_color.cpp
        test_canvas.cpp
        test_matrix.cpp
        test_transformations.cpp
        test_ray.cpp
        )

add_executable(RayTracerChallengeTests ${SRCS})

target_link_libraries(RayTracerChallengeTests
        PRIVATE
            #Threads::Threads
            RayTracerChallenge::Lib
            GTest::gtest
            GTest::gtest_main
            ${Boost_LIBRARIES})

include(GoogleTest)
gtest_discover_tests(RayTracerChallengeTests)
