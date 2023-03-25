# The Ray Tracer Challenge - C++

## Conventions

This codebase uses the Left Hand Coordinate system.

## Building

### Conan

#### Create Profiles

Debug:
```
$ conan profile new gcc-11-debug --detect 
$ conan profile update settings.compiler.libcxx=libstdc++11 gcc-11-debug
$ conan profile update settings.compiler.cppstd=20 gcc-11-debug
$ conan profile update settings.build_type=Debug gcc-11-debug
```

Release:
```
$ conan profile new gcc-11-release --detect 
$ conan profile update settings.compiler.libcxx=libstdc++11 gcc-11-release
$ conan profile update settings.compiler.cppstd=20 gcc-11-release
$ conan profile update settings.build_type=Release gcc-11-release
```

#### Install Packages

Debug:

```
$ conan install -if cmake-build-debug . --build=missing -s build_type=Debug -pr:b=gcc-11-debug
```

Release:

```
$ conan install -if cmake-build-release . --build=missing -s build_type=Release -pr:b=gcc-11-release
```

### CMake

```
$ cmake .. \
    -DCMAKE_BUILD_TYPE=Debug
    -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake
    -DBUILD_TESTS=1
    -DUSE_STATIC_BOOST=1
```

## Development Notes

### Genericity

Started the project with a type `T` for most classes, to allow switching between `float` and `double`.
However it became apparent that `float` does not provide enough precision for quality images, therefore only the use of
`double` makes practical sense.

At some point, refactor the code to remove the `T` type parameters. This should simplify the code considerably.

### Polymorphism

For Chapter 9, introduced dynamic polymorphism via virtual member functions.
This increased `chapter8`'s execution time (Release build, for 2048x1536) from around 7.0 seconds to about 8.0 seconds.
There's +/-1 second variation between runs, so probably need a more complex image to get a more precise measurement.

Consider removing virtual member functions and replace the world collection with a tuple of vectors for each supported
shape:

`std::tuple<std::vector<Sphere>, std::vector<Plane>, ...>`

