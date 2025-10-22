vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO jkCXf9X4/cutecpp
    REF "${VERSION}"
    SHA512 6d9b4dca1b978dc95f955b43fe585e4377394da40d81b9d604f7df68a6066a94cf5c52243fa5636c816d09cfd1ade7696b40abe4eaff99a66c0fd41f6bdbba72
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
    FILE_LIST "${SOURCE_PATH}/LICENSE"
)
