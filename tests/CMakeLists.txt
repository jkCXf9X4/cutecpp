find_package(Catch2 3 CONFIG REQUIRED)

file(GLOB_RECURSE ALL_SRC CONFIGURE_DEPENDS "*.hpp" "*.cpp")

add_executable(cutecpp_tests
    ${ALL_SRC}
)

target_link_libraries(cutecpp_tests
    PUBLIC
        Catch2::Catch2WithMain
        cutecpp
)

include(CTest)
include(Catch)
catch_discover_tests(cutecpp_tests)
