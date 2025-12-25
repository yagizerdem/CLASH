
# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 2.24:
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        processTest
        process_test.cc
)

add_executable(
        expandTest
        expand_test.cc
)

target_link_libraries(processTest PUBLIC processLib)
target_link_libraries(processTest PRIVATE GTest::gtest GTest::gtest_main)

target_link_libraries(expandTest PUBLIC expandLib)
target_link_libraries(expandTest PRIVATE GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(processTest)
gtest_discover_tests(expandTest)