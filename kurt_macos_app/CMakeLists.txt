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
    XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++2b" # Needed?
)

set_target_properties(kurtmacosapp PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in
)

target_compile_options(kurtmacosapp PRIVATE
    "-cxx-interoperability-mode=default"
)

target_link_libraries(kurtmacosapp PRIVATE
    kurt
    kurt_macos
)
