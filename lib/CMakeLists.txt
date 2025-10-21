
file(GLOB_RECURSE ALL_SRC CONFIGURE_DEPENDS "*.hpp" "*.cpp")

message(ALL_SRC cutelog ="${ALL_SRC}")

add_library(cutelog STATIC ${ALL_SRC})

target_include_directories(cutelog
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include/cutecpp

  PRIVATE
   ${CMAKE_CURRENT_LIST_DIR}/include_private
)

target_compile_options(cutelog PRIVATE -Wfatal-errors)