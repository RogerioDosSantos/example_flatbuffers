
# Flatbuffer Example documentation

# Introduction

[FlatBuffers](https://google.github.io/flatbuffers/index.html#flatbuffers_overview) is an efficient cross platform serialization library for C++, C#, C, Go, Java, JavaScript, PHP, and Python. It was originally created at Google for game development and other performance-critical applications.

## How to compile

### How to compile FlatBuffers

[FlatBuffers git repository](https://github.com/google/flatbuffers)

[FlatBuffers building documentation](https://google.github.io/flatbuffers/flatbuffers_guide_building.html)


``` shell

cd <flatbuffers_git_folder>
cmake -G "Unix Makefiles"
make

``````

### Linux

- Run */build/compile_linux_default.sh*

### Windows

- Open *The project on Visual Studio (2017 or later)* using the *Open CMAKE* option.
- Compile the desired projects

## Output location and details

  The compiled binaries will be located on the */stage* folder which follows the structure below:

      /stage
      ├── build
      │   └── cmake - CMake project files that can be used on to find the libraries by other projects
      │       ├── project-config.cmake
      │       └── project-configversion.cmake
      ├── include
      │       └── project.h
      └── Linux-4.4.0-43-Microsoft - It can change depending on the platform
          └── x86_64 - It can change depending on the platform
              ├── bin
              │   └── project - Executable or shared libraries
              ├── lib
              │   └── project.a - Static libraries
              └── cmake - CMake target for that specific platform
                  ├── project.cmake
                  └── project-noconfig.cmake

