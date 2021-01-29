# ==================================================================
#  tubex-lib - cmake configuration file
# ==================================================================

# Settings

cmake_minimum_required(VERSION 3.0)
 project(tubex-lib)


  set(CMAKE_CXX_STANDARD  11)
  set(PACKAGE_NAME        tubex-lib)
  set(PACKAGE_VERSION     "2.0")

 

  #  project(tubex-lib VERSION ${VERSION} LANGUAGES CXX)


  set_property(GLOBAL PROPERTY COMPILE_FLAGS "-std=c++11 -msse3")

  set(CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS} -O3")
  set(CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS} -DNDEBUG") # comment for debug


  project(${PACKAGE_NAME})

  get_filename_component(TUBEX_ROOT          "./"         ABSOLUTE) # used for find_packages modules
  get_filename_component(TUBELIB_CMAKE_DIR   "./cmake"    ABSOLUTE)
  get_filename_component(TUBELIB_EXAMPLE_DIR "./examples" ABSOLUTE)
  get_filename_component(TUBELIB_BUILD_DIR   "./build"     ABSOLUTE)
  get_filename_component(TUBELIB_DATA_DIR    "./data"     ABSOLUTE)

################################################################################
# Options for install directory
################################################################################

  set(CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "C++ header files (include)")
  set(CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "object code libraries (lib)")
  set(CMAKE_INSTALL_BINDIR "bin" CACHE PATH "user executables (bin)")
  set(CMAKE_INSTALL_PKGCONFIG "share/pkgconfig" CACHE PATH "pkg files (share/pkgconfig)")

# Dependencies


#list(APPEND CMAKE_MODULE_PATH "${TUBELIB_CMAKE_DIR}/modules") # location of find_package modules
 # find_package(IbexLib REQUIRED) # will set variables IBEX_INCLUDE_DIR, IBEX_LIBRARY
  #include_directories("${IBEX_INCLUDE_DIR}")

# IBEX-lib package

  link_directories(/home/neveub/tubex/ibex-lib/install/lib/)
  link_directories(/home/neveub/tubex/ibex-lib/install/lib/ibex/3rd/)


  include_directories(/home/neveub/tubex/ibex-lib/install/include/ibex/)
  include_directories(/home/neveub/tubex/ibex-lib/install/include/ibex/3rd/)

  #  set(LIBS ${LIBS} -libex -lm -lgaol -lgdtoa -lultim) # using library Gaol

  set(LIBS ${LIBS} -libex -lm -lprim) # using library Filib
  set(IBEX_LIBRARY ${LIBS})

  list(APPEND CMAKE_MODULE_PATH "${TUBELIB_CMAKE_DIR}/modules") # location of find_package modules








# Optional binary tree activation for all tubes (for tests purposes mainly)

  if(NOT DEFINED AUTO_SYNTHESIS_BY_DEFAULT)
    SET(AUTO_SYNTHESIS_BY_DEFAULT false) # direct tubes by default
  endif()
  add_definitions(-DAUTO_SYNTHESIS_BY_DEFAULT=${AUTO_SYNTHESIS_BY_DEFAULT})
  message("-- Using binary trees for tubes: " ${AUTO_SYNTHESIS_BY_DEFAULT})



# Tubex sources

  add_subdirectory(src) # sources of core, robotics
  include_directories(${TUBELIB_BUILD_DIR}/include/)


# Tests (unit tests + examples)

  option(BUILD_TESTS "Build test" OFF)
  if(BUILD_TESTS)
    enable_testing(true)

    # Unit tests
    add_subdirectory(tests)
    include(CTest)

    


    # Basics
    add_test(NAME basics_01
             COMMAND ${TUBELIB_EXAMPLE_DIR}/basics/ex_01_arithmetic/make/basics_01_arithmetic 0)
    add_test(NAME basics_02
             COMMAND ${TUBELIB_EXAMPLE_DIR}/basics/ex_02_simple_ctc/make/basics_02_simple_ctc 0)
    add_test(NAME basics_03
             COMMAND ${TUBELIB_EXAMPLE_DIR}/basics/ex_03_ivp_msinx/make/basics_03_ivp_msinx 0)
    add_test(NAME basics_04
             COMMAND ${TUBELIB_EXAMPLE_DIR}/basics/rob_04_lissajous/make/basics_04_lissajous 0)
    add_test(NAME basics_05
             COMMAND ${TUBELIB_EXAMPLE_DIR}/basics/ex_05_simple_eval/make/basics_05_simple_eval 0)
    add_test(NAME basics_06
             COMMAND ${TUBELIB_EXAMPLE_DIR}/basics/ex_06_graphics/make/basics_06_graphics 0)
  endif()

  # Python binding:
  if(WITH_PYTHON)
    add_subdirectory(python)

  endif()


# Uninstallation

  # todo
