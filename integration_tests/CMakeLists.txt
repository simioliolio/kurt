add_executable(macos_integration_test
    macos/integration_test.cpp
)
target_link_libraries(macos_integration_test
    PRIVATE
    kurt
    kurt_macos
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(macos_integration_test)

# Add test data
file(COPY resources/440Hz2s.wav DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/)
