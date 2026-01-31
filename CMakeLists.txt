cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 11)

project(ce2 C)

include_directories(${CMAKE_SOURCE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${CMAKE_SOURCE_DIR}/bin")

file(GLOB_RECURSE ENGINE_CORE       "engine/core/*.c")
file(GLOB_RECURSE ENGINE_GRAPHICS   "engine/graphics/*.c")
file(GLOB_RECURSE ENGINE_INPUT      "engine/input/*.c")
file(GLOB_RECURSE ENGINE_OBJECTS    "engine/objects/*.c")
file(GLOB_RECURSE ENGINE_PHYSICS    "engine/physics/*.c")

if (UNIX)
    file(GLOB ENGINE_PLATFORM "engine/platform/unix/*.c")
elseif (WIN32)
    file(GLOB ENGINE_PLATFORM "engine/platform/win/*.c")
else()
    message(FATAL_ERROR "Неподдерживаемая платформа")
endif()

set(ENGINE_SOURCES
    ${ENGINE_CORE}
    ${ENGINE_GRAPHICS}
    ${ENGINE_INPUT}
    ${ENGINE_OBJECTS}
    ${ENGINE_PHYSICS}
    ${ENGINE_PLATFORM}
)

file(GLOB_RECURSE GAME_SOURCES
    "game/*.c"
)

add_executable(game ${ENGINE_SOURCES} ${GAME_SOURCES})