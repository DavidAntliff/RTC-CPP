cmake_minimum_required(VERSION 3.24)
project(RayTracerChallengeTests)

find_package(GTest REQUIRED)

if (NOT TARGET RayTracerChallenge::Lib)
    find_package(RayTracerChallenge::Lib CONFIG REQUIRED)
endif()

find_package(Boost COMPONENTS filesystem REQUIRED)

#set(THREADS_PREFER_PTHREAD_FLAG ON)
#find_package(Threads REQUIRED)

set(HDRS
        support/support.h
        )

set(SRCS
        test_tuples.cpp
        test_color.cpp
        test_canvas.cpp
        test_matrices.cpp
        test_transformations.cpp
        test_rays.cpp
        test_spheres.cpp
        test_intersections.cpp
        test_lights.cpp
        test_materials.cpp
        test_world.cpp
        test_camera.cpp
        test_shapes.cpp
        test_planes.cpp
        test_patterns.cpp
        )

add_executable(RayTracerChallengeTests ${SRCS} ${HDRS})

target_link_libraries(RayTracerChallengeTests
        PRIVATE
            #Threads::Threads
            RayTracerChallenge::Lib
            GTest::gtest
            GTest::gtest_main
            ${Boost_LIBRARIES})

include(GoogleTest)
gtest_discover_tests(RayTracerChallengeTests)
