cmake_minimum_required(VERSION 3.10)

file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/version.txt" projectVersion)
project(cutecpp VERSION ${projectVersion})
message("cutecpp, version: ${projectVersion}\n")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CUTECPP_BUILD_TESTS "Build cutecpp tests" OFF)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

## Link static
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++")
set(CMAKE_SHARED_LINKER_FLAGS " -static-libstdc++")


# COMPILER
## Optimize release builds
set(CMAKE_CXX_FLAGS_RELEASE "-flto=auto -O3 -g -march=native -fno-omit-frame-pointer -DNDEBUG" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_RELEASE "-flto=auto -O3 -g -march=native -fno-omit-frame-pointer -DNDEBUG" CACHE STRING "" FORCE)

## Debug builds keep symbols, low optimizations
set(CMAKE_CXX_FLAGS_DEBUG "-O1 -fno-lto -g" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_DEBUG   "-O1 -fno-lto -g" CACHE STRING "" FORCE)


# Status
message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}\n")
message("CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}\n")
message("CMAKE_C_FLAGS_DEBUG: ${CMAKE_C_FLAGS_DEBUG}\n")

message("CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}\n")
message("CMAKE_C_FLAGS_RELEASE: ${CMAKE_C_FLAGS_RELEASE}\n")

message("CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}\n")
message("CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}\n")

message("CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}\n")
message("CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}\n")
message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}\n")

# Subdirectories
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_subdirectory(lib)

if(CUTECPP_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

install(
  TARGETS cutecpp
  EXPORT cutecppTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/include/cutecpp
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
  EXPORT cutecppTargets
  NAMESPACE cutecpp::
  FILE cutecppTargets.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cutecpp
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/cutecppConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cutecppConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cutecppConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cutecpp
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/cutecppConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/cutecppConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cutecpp
)
