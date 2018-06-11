project(Examples)

cmake_minimum_required(VERSION 3.1)

include(CheckCXXCompilerFlag)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Intel")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
    CHECK_CXX_COMPILER_FLAG("-std=c++14" HAS_CPP14_FLAG)

    if (HAS_CPP14_FLAG)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    else()
        message(FATAL_ERROR "Unsupported compiler -- xtensor requires C++14 support!")
    endif()
endif()

add_definitions(-DJULIA_ENABLE_THREADING)

find_package(JlCxx REQUIRED)
find_package(xtensor REQUIRED)
find_package(xtensor-julia REQUIRED)

include_directories(${xtensor_INCLUDE_DIRS})
include_directories(${xtensor-julia_INCLUDE_DIRS})

add_library(tensors SHARED tensors.cpp)
target_link_libraries(tensors JlCxx::cxxwrap_julia)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS
  tensors
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xtensor-julia-examples)
