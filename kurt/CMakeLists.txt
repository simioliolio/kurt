add_library(kurt
    include/kurt.hpp
    src/kurt.cpp
    include/pcm_parser.hpp
    src/pcm_parser.cpp
)

target_include_directories(kurt
    PUBLIC
    include
)

enable_testing()

include(GoogleTest)

add_executable(kurt_test
    test/kurt_test.cpp
    test/pcm_parser_test.cpp
)
target_link_libraries(kurt_test
    PRIVATE
    kurt
    GTest::gtest_main
)
gtest_discover_tests(kurt_test)

# Add test data
file(COPY test/resources/16bit_stereo.wav DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/kurt/test/resources/)
file(COPY test/resources/24bit_stereo.wav DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/kurt/test/resources/)
