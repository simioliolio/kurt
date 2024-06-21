add_library(kurt_macos
    include/core_audio_host.hpp
    src/core_audio_host.cpp
)
target_include_directories(kurt_macos
    PUBLIC
    include
)
target_link_libraries(kurt_macos
    PRIVATE
    "-framework AudioToolbox"
    kurt
)

enable_testing()

add_executable(kurt_macos_test
    test/core_audio_host_test.cpp
)
target_link_libraries(kurt_macos_test
    PRIVATE
    kurt
    kurt_macos
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(kurt_macos_test)
