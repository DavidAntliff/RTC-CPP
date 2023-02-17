

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
