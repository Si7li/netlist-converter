# Install script for directory: /home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "COIN" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-build/libCOIN.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ogdf/CoinTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ogdf/CoinTargets.cmake"
         "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-build/CMakeFiles/Export/b0420fb67b8ce1b5a79fe00e53c3b87e/CoinTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ogdf/CoinTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ogdf/CoinTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ogdf" TYPE FILE FILES "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-build/CMakeFiles/Export/b0420fb67b8ce1b5a79fe00e53c3b87e/CoinTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ogdf" TYPE FILE FILES "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-build/CMakeFiles/Export/b0420fb67b8ce1b5a79fe00e53c3b87e/CoinTargets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "COINheaders" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-src/include/coin" FILES_MATCHING REGEX "/[^/]*\\.h$" REGEX "/[^/]*\\.hpp$")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ogdf" TYPE FILE FILES
    "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-build/ogdf-config.cmake"
    "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-src/cmake/FindLibbfd.cmake"
    "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-src/cmake/FindLibdw.cmake"
    "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-src/cmake/FindLibunwind.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "OGDF" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-build/libOGDF.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ogdf/OgdfTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ogdf/OgdfTargets.cmake"
         "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-build/CMakeFiles/Export/b0420fb67b8ce1b5a79fe00e53c3b87e/OgdfTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ogdf/OgdfTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/ogdf/OgdfTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ogdf" TYPE FILE FILES "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-build/CMakeFiles/Export/b0420fb67b8ce1b5a79fe00e53c3b87e/OgdfTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ogdf" TYPE FILE FILES "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-build/CMakeFiles/Export/b0420fb67b8ce1b5a79fe00e53c3b87e/OgdfTargets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "OGDFheaders" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES
    "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-build/include/"
    "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-src/include/ogdf"
    FILES_MATCHING REGEX "/[^/]*\\.h$" REGEX "/[^/]*\\.hpp$" REGEX "/[^/]*\\.inc$")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/libogdf" TYPE DIRECTORY FILES "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-src/doc/examples" FILES_MATCHING REGEX "/[^/]*\\.cpp$" REGEX "/[^/]*\\.txt$" REGEX "/[^/]*\\.gml$" REGEX "/[^/]*\\.bench$" REGEX "/output\\-[^/]*$" EXCLUDE)
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-build/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
