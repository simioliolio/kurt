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
