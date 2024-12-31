# Set minimum CMake version
cmake_minimum_required(VERSION 3.29)

# Set project name
project(kurt
    VERSION 0.1.0
    LANGUAGES CXX
)

if (CMAKE_GENERATOR STREQUAL "Xcode")
    enable_language(Swift)
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# For googletest
cmake_policy(SET CMP0135 NEW)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_subdirectory(kurt)
add_subdirectory(kurt_macos)
add_subdirectory(kurt_macos_app)
add_subdirectory(integration_tests)
