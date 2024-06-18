add_library(kurt
    include/kurt.hpp
    include/pcm_reader.hpp
    src/pcm_reader.cpp
)

target_include_directories(kurt
    PUBLIC
    include
)

enable_testing()

add_executable(kurt_test
    test/pcm_reader_test.cpp
)
target_link_libraries(kurt_test
    PRIVATE
    kurt
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(kurt_test)

# Add test data
file(COPY test/resources/16bit_stereo.wav DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/kurt/test/resources/)
file(COPY test/resources/24bit_stereo.wav DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/kurt/test/resources/)
