
file(GLOB_RECURSE ALL_SRC CONFIGURE_DEPENDS "*.hpp" "*.cpp")

message(ALL_SRC cutecpp ="${ALL_SRC}")

add_library(cutecpp STATIC ${ALL_SRC})

target_include_directories(cutecpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/include/cutecpp

  PRIVATE
   ${CMAKE_CURRENT_LIST_DIR}/include_private
)

find_path(KISSNET_INCLUDE_DIRS "kissnet.hpp")
target_include_directories(cutecpp PRIVATE ${KISSNET_INCLUDE_DIRS})

target_compile_options(cutecpp PRIVATE -Wfatal-errors)