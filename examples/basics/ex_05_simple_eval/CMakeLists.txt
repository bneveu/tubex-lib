# ==================================================================
#  tubex-lib / example - cmake configuration file
# ==================================================================

cmake_minimum_required(VERSION 3.0.2)
project(TUBEX_BASICS_05 LANGUAGES CXX)

# Compilation options
set(CMAKE_CXX_STANDARD 11)
add_compile_options(-O3 -DNDEBUG) # comment for debug mode
if(WIN32)
  add_definitions(-U__STRICT_ANSI__)
endif()

# Dependencies
include(FindPkgConfig)
pkg_search_module(TUBEX REQUIRED tubex)
message(STATUS "Found Tubex version ${TUBEX_VERSION}")

################################################################################
# (to be removed) With Gaol, we add "-Wno-deprecated" to remove lots of warnings
#                 generated by a function definition in gaol_interval.h
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-Wno-deprecated" COMPILER_SUPPORTS_WNO_DEPRECATED)
if(COMPILER_SUPPORTS_WNO_DEPRECATED)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
endif()
################################################################################

# Compilation
add_executable(basics_05 main.cpp)
target_compile_options(basics_05 PUBLIC ${TUBEX_CFLAGS})
target_include_directories(basics_05 PUBLIC ${TUBEX_INCLUDE_DIRS})
target_link_libraries(basics_05 PUBLIC ${TUBEX_LDFLAGS})