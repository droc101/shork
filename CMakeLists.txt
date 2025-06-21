cmake_minimum_required(VERSION 3.30)
project(shork C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "-Wall")

set(CMAKE_C_FLAGS_DEBUG "-g -O0 -DBUILDSTYLE_DEBUG")
set(CMAKE_C_FLAGS_RELEASE "-g0 -O3 -s -DNDEBUG -DBUILDSTYLE_RELEASE")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g -O3 -DBUILDSTYLE_DEBUG")

add_executable(shork main.c
        renderer.c
        renderer.h
        platform/console/tty.c
        platform/console/tty.h
        io.c
        io.h
        model_loader.c
        model_loader.h
        platform/console/ansi.h
        args.c
        args.h
        platform/console/egl.c
        platform/console/egl.h
        platform/console/platform_console.c
        platform/console/platform_console.h
        platform/platform.h
        platform/sdl/platform_sdl.c
        platform/sdl/platform_sdl.h
)

find_package(SDL3 REQUIRED)
include_directories(${SDL3_INCLUDE_DIRS})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

target_link_libraries(shork -lEGL -lGLESv2 m SDL3 ${SDL3_LIBRARY})

add_custom_target(
        copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
)
add_dependencies(shork copy_assets)
