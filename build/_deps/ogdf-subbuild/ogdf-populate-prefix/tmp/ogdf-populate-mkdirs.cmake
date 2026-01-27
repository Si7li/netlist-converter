# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-src")
  file(MAKE_DIRECTORY "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-src")
endif()
file(MAKE_DIRECTORY
  "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-build"
  "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-subbuild/ogdf-populate-prefix"
  "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-subbuild/ogdf-populate-prefix/tmp"
  "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-subbuild/ogdf-populate-prefix/src/ogdf-populate-stamp"
  "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-subbuild/ogdf-populate-prefix/src"
  "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-subbuild/ogdf-populate-prefix/src/ogdf-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-subbuild/ogdf-populate-prefix/src/ogdf-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-subbuild/ogdf-populate-prefix/src/ogdf-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
