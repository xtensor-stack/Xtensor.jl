project(Examples)

cmake_minimum_required(VERSION 3.1)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

list(APPEND CMAKE_CXX_FLAGS "-std=c++14 -march=native")
add_definitions(-DJULIA_ENABLE_THREADING)

find_package(CxxWrap REQUIRED)
find_package(xtensor REQUIRED)
find_package(xtensor-julia REQUIRED)

include_directories(${xtensor_INCLUDE_DIR})
include_directories(${xtensor-julia_INCLUDE_DIR})

add_library(tensors SHARED tensors.cpp)
target_link_libraries(tensors CxxWrap::cxx_wrap)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS
  tensors
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xtensor-julia-examples)
