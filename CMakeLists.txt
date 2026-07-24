cmake_minimum_required(VERSION 4.2)
project(Clash)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/externals/sanitizers-cmake/cmake" ${CMAKE_MODULE_PATH})

set(SANITIZE_ADDRESS ON CACHE BOOL "Enable AddressSanitizer" FORCE)
set(SANITIZE_UNDEFINED ON CACHE BOOL "Enable UndefinedBehaviorSanitizer" FORCE)

find_package(Sanitizers REQUIRED)

add_executable(Clash src/main.cpp backward.cpp backward.hpp
        src/util.h
        src/util.cpp)

file(
        DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.43.1/CPM.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
        EXPECTED_HASH SHA256=1c40fc102ce9625d7de7eb14f541cab30cc3138dca627f0b0ec40293ce6c2934
)

include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

# Downloads this wrapper library and tree-sitter.
# Makes them available via the `cpp-tree-sitter` CMake library target.
CPMAddPackage(
        NAME cpp-tree-sitter
        GIT_REPOSITORY https://github.com/nsumner/cpp-tree-sitter.git
        GIT_TAG v0.0.3
)

# Downloads a tree-sitter grammar from github and makes it available as a
# cmake library target.
add_grammar_from_repo(tree-sitter-bash            # Defines the library name for a grammar
        https://github.com/tree-sitter/tree-sitter-bash.git  # Repository URL of a tree-sitter grammar
        0.23.3                                               # Version tag for the grammar
)

add_grammar_from_repo(tree-sitter-json
        https://github.com/tree-sitter/tree-sitter-json.git
        0.19.0
)



# Use the library in a demo program.

target_link_libraries(Clash
        tree-sitter-bash
        tree-sitter-json
        cpp-tree-sitter
)


add_sanitizers(Clash)

if(TARGET tree-sitter)
    add_sanitizers(tree-sitter)
endif()

if(TARGET cpp-tree-sitter)
    add_sanitizers(cpp-tree-sitter)
endif()

if(TARGET tree-sitter-bash)
    add_sanitizers(tree-sitter-bash)
endif()

if(TARGET tree-sitter-json)
    add_sanitizers(tree-sitter-json)
endif()