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
        console.c
        console.h
        io.c
        io.h
        model_loader.c
        model_loader.h
        ansi.h
)

target_link_libraries(shork -lEGL -lGLESv2 m)

add_custom_target(
        copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
)
add_dependencies(shork copy_assets)
