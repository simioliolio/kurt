add_executable(kurtmacosapp
    ContentView.swift
    KurtApp.swift
)

file(STRINGS "${CMAKE_SOURCE_DIR}/.swift-version" SWIFT_VERSION)
set_target_properties(kurtmacosapp PROPERTIES
    XCODE_ATTRIBUTES_SWIFT_VERSION ${SWIFT_VERSION}
)

set_target_properties(kurtmacosapp PROPERTIES
    XCODE_ATTRIBUTE_SWIFT_OPTIMIZATION_LEVEL[variant=Debug] "-Onone"
    XCODE_ATTRIBUTE_SWIFT_OBJC_INTEROP_MODE "objcxx"
)

set_target_properties(kurtmacosapp PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in
)

set_target_properties(kurtmacosapp PROPERTIES
    XCODE_ATTRIBUTE_SWIFT_OBJC_BRIDGING_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/kurtmacosapp-Bridging-Header.h
)

# Ensure C++23 is used during Swift module emission
target_compile_options(kurtmacosapp PRIVATE
    $<$<COMPILE_LANGUAGE:Swift>:-Xcc -std=c++2b>
)

target_link_libraries(kurtmacosapp PRIVATE
    kurt
    kurt_macos
)
