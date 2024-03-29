cmake_minimum_required(VERSION 3.20)

# Boost headers only
find_package(Boost 1.81.0 REQUIRED)

# Need at least one .cpp file to build the lib:
set(SRCS
        tuple.cpp
        materials.cpp
        patterns.cpp
        )

set(HDRS
        include/ray_tracer_challenge/math.h
        include/ray_tracer_challenge/tuples.h
        include/ray_tracer_challenge/color.h
        include/ray_tracer_challenge/canvas.h
        include/ray_tracer_challenge/matrices.h
        include/ray_tracer_challenge/transformations.h
        include/ray_tracer_challenge/rays.h
        include/ray_tracer_challenge/lights.h
        include/ray_tracer_challenge/materials.h
        include/ray_tracer_challenge/world.h
        include/ray_tracer_challenge/camera.h
        include/ray_tracer_challenge/shapes.h
        include/ray_tracer_challenge/planes.h
        include/ray_tracer_challenge/patterns.h
        include/ray_tracer_challenge/perlin_noise.h
        )

add_library(RayTracerChallenge-Lib ${SRCS} ${HDRS})

target_compile_features(RayTracerChallenge-Lib PUBLIC cxx_std_20)
target_compile_options(RayTracerChallenge-Lib PRIVATE -Wall -Wextra -Wpedantic)
target_compile_options(RayTracerChallenge-Lib PRIVATE "$<$<CONFIG:Debug>:-O0>")
target_include_directories(RayTracerChallenge-Lib PUBLIC include)
target_link_libraries(RayTracerChallenge-Lib PRIVATE Boost::boost)

# Modern CMake recommends use of an ALIAS, for better error handling:
add_library(RayTracerChallenge::Lib ALIAS RayTracerChallenge-Lib)
