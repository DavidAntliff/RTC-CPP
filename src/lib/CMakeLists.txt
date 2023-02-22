cmake_minimum_required(VERSION 3.24)

set(SRCS
        tuple.cpp
)

set(HDRS
        include/ray_tracer_challenge/math.h
        include/ray_tracer_challenge/tuple.h
        include/ray_tracer_challenge/color.h
        include/ray_tracer_challenge/canvas.h
        include/ray_tracer_challenge/matrix.h
        include/ray_tracer_challenge/transformations.h
        include/ray_tracer_challenge/ray.h
        )

add_library(RayTracerChallenge-Lib ${SRCS} ${HDRS})

target_compile_features(RayTracerChallenge-Lib PUBLIC cxx_std_20)
target_compile_options(RayTracerChallenge-Lib PRIVATE -Wall -Wextra -Wpedantic)
target_compile_options(RayTracerChallenge-Lib PRIVATE "$<$<CONFIG:Debug>:-O0>")
target_include_directories(RayTracerChallenge-Lib PUBLIC include)

# Modern CMake recommends use of an ALIAS, for better error handling:
add_library(RayTracerChallenge::Lib ALIAS RayTracerChallenge-Lib)
