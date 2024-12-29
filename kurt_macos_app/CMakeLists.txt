cmake_minimum_required(VERSION 3.29)
project(kurt_macos_app
    VERSION 1.0
    LANGUAGES Swift
)

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
)

set_target_properties(kurtmacosapp PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in
)
