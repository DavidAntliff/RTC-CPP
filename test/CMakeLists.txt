cmake_minimum_required(VERSION 3.20)
project(RayTracerChallengeTests)

if (NOT TARGET RayTracerChallenge::Lib)
    find_package(RayTracerChallenge::Lib CONFIG REQUIRED)
endif()

find_package(Boost 1.81.0 REQUIRED)
find_package(GTest 1.12.1 REQUIRED)

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
        test_perlin_noise.cpp
        )

add_executable(RayTracerChallengeTests ${SRCS} ${HDRS})

target_link_libraries(RayTracerChallengeTests
        PRIVATE
            #Threads::Threads
            RayTracerChallenge::Lib
            GTest::gtest_main
            Boost::boost
            )

include(GoogleTest)
gtest_discover_tests(RayTracerChallengeTests)
