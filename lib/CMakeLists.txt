
file(GLOB_RECURSE ALL_SRC CONFIGURE_DEPENDS "*.hpp" "*.cpp")

message(ALL_SRC cutecpp ="${ALL_SRC}")

add_library(cutecpp STATIC ${ALL_SRC})
add_library(cutecpp::cutecpp ALIAS cutecpp)

target_include_directories(cutecpp
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>

  PRIVATE
   ${CMAKE_CURRENT_LIST_DIR}/include_private
)

find_path(KISSNET_INCLUDE_DIRS "kissnet.hpp")
if(NOT KISSNET_INCLUDE_DIRS)
  message(FATAL_ERROR "Failed to locate kissnet.hpp. Ensure the kissnet dependency is available.")
endif()
target_include_directories(cutecpp PRIVATE ${KISSNET_INCLUDE_DIRS})

target_compile_options(cutecpp PRIVATE -Wfatal-errors)
