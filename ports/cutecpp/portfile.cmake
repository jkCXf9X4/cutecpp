vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO jkCXf9X4/cutecpp
    REF "${VERSION}"
    SHA512 2dbb4f48d233094ca4bbe2eb4a43667b18f18abe9bb2185c2345480c925bc464e2dbf147a4af273cfbd8be86bd1d5eecf8629633a0b67a1c1a5b3c55135fa041
    HEAD_REF main
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DCUTECPP_BUILD_TESTS=OFF
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(
    PACKAGE_NAME cutecpp
    CONFIG_PATH lib/cmake/cutecpp
)

file(REMOVE_RECURSE
    "${CURRENT_PACKAGES_DIR}/debug/include"
)

file(INSTALL "${CMAKE_CURRENT_LIST_DIR}/usage" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")
vcpkg_install_copyright(
    FILES "${SOURCE_PATH}/LICENSE"
)
