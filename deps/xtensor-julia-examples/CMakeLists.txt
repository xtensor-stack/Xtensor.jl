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

install(TARGETS
  tensors 
LIBRARY DESTINATION lib${LIBDIR_SUFFIX}
ARCHIVE DESTINATION lib${LIBDIR_SUFFIX}
RUNTIME DESTINATION lib${LIBDIR_SUFFIX}
INCLUDES DESTINATION include)
