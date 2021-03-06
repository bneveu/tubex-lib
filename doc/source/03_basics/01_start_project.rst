Start a project
===============

Tubex is ready to be used on your computer. You can now copy-paste the following example code in a file named ``main.cpp``:

.. code-block:: c++

  #include <tubex.h>
  
  using namespace std;
  using namespace ibex;
  using namespace tubex;
  
  int main()
  {
    Interval domain(0.,10.);
    Tube x(domain);
  
    cout << "My first tube: " << x << endl;
  }

For the compilation of your project, you can use CMake with the following ``CMakeLists.txt`` that you will add in the same directory:

.. code-block:: cmake

  cmake_minimum_required(VERSION 3.0.2)
  project(TUBEX_BASICS_01 LANGUAGES CXX)

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
  add_executable(my_project main.cpp)
  target_compile_options(my_project PUBLIC ${TUBEX_CFLAGS})
  target_include_directories(my_project PUBLIC ${TUBEX_INCLUDE_DIRS})
  target_link_libraries(my_project PUBLIC ${TUBEX_LDFLAGS})

The files ``main.cpp`` and ``CMakeLists.txt`` appear in the same directory:

.. code-block:: bash

  my_project
  ├── CMakeLists.txt
  └── main.cpp

The compilation is made by the following command line:

.. code-block:: bash

  mkdir make -p ; cd make ; cmake .. ; make ; cd ..

Lastly, the project can be run with:

.. code-block:: bash

  ./make/my_project