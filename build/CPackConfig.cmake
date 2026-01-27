# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. The list of available CPACK_xxx variables and their associated
# documentation may be obtained using
#  cpack --help-variable-list
#
# Some variables are common to all generators (e.g. CPACK_PACKAGE_NAME)
# and some are specific to a generator
# (e.g. CPACK_NSIS_EXTRA_INSTALL_COMMANDS). The generator specific variables
# usually begin with CPACK_<GENNAME>_xxxx.


set(CPACK_BINARY_DEB "OFF")
set(CPACK_BINARY_FREEBSD "OFF")
set(CPACK_BINARY_IFW "OFF")
set(CPACK_BINARY_NSIS "OFF")
set(CPACK_BINARY_RPM "OFF")
set(CPACK_BINARY_STGZ "ON")
set(CPACK_BINARY_TBZ2 "OFF")
set(CPACK_BINARY_TGZ "ON")
set(CPACK_BINARY_TXZ "OFF")
set(CPACK_BINARY_TZ "ON")
set(CPACK_BUILD_SOURCE_DIRS "/home/khalils/Desktop/Projects/PFA/netlist_converter;/home/khalils/Desktop/Projects/PFA/netlist_converter/build")
set(CPACK_CMAKE_GENERATOR "Unix Makefiles")
set(CPACK_COMPONENTS_ALL "COIN;COINheaders;OGDF;OGDFheaders;Unspecified")
set(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
set(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libunwind-dev")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_FILE "/usr/share/cmake/Templates/CPack.GenericDescription.txt")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_SUMMARY "netlist_converter built using CMake")
set(CPACK_GENERATOR "STGZ;TGZ;TZ")
set(CPACK_INNOSETUP_ARCHITECTURE "x64")
set(CPACK_INSTALL_CMAKE_PROJECTS "/home/khalils/Desktop/Projects/PFA/netlist_converter/build;netlist_converter;ALL;/")
set(CPACK_INSTALL_PREFIX "/usr/local")
set(CPACK_MODULE_PATH "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-src/cmake")
set(CPACK_NSIS_DISPLAY_NAME "netlist_converter 0.1.1")
set(CPACK_NSIS_INSTALLER_ICON_CODE "")
set(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
set(CPACK_NSIS_PACKAGE_NAME "netlist_converter 0.1.1")
set(CPACK_NSIS_UNINSTALL_NAME "Uninstall")
set(CPACK_OBJCOPY_EXECUTABLE "/usr/bin/objcopy")
set(CPACK_OBJDUMP_EXECUTABLE "/usr/bin/objdump")
set(CPACK_OUTPUT_CONFIG_FILE "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/CPackConfig.cmake")
set(CPACK_OUTPUT_FILE_PREFIX "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-src/packages")
set(CPACK_PACKAGE_CONTACT "ogdf@googlegroups.com")
set(CPACK_PACKAGE_DEFAULT_LOCATION "/")
set(CPACK_PACKAGE_DESCRIPTION_FILE "/usr/share/cmake/Templates/CPack.GenericDescription.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "netlist_converter built using CMake")
set(CPACK_PACKAGE_FILE_NAME "netlist_converter-0.1.1-Linux")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "netlist_converter 0.1.1")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "netlist_converter 0.1.1")
set(CPACK_PACKAGE_NAME "netlist_converter")
set(CPACK_PACKAGE_RELOCATABLE "true")
set(CPACK_PACKAGE_VENDOR "Humanity")
set(CPACK_PACKAGE_VERSION "0.1.1")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "1")
set(CPACK_PRODUCTBUILD_DOMAINS "ON")
set(CPACK_PRODUCTBUILD_IDENTIFIER "net.ogdf.pkg")
set(CPACK_READELF_EXECUTABLE "/usr/bin/readelf")
set(CPACK_RESOURCE_FILE_LICENSE "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/_deps/ogdf-src/LICENSE.txt")
set(CPACK_RESOURCE_FILE_README "/usr/share/cmake/Templates/CPack.GenericDescription.txt")
set(CPACK_RESOURCE_FILE_WELCOME "/usr/share/cmake/Templates/CPack.GenericWelcome.txt")
set(CPACK_RPM_PACKAGE_REQUIRES "libunwind-devel")
set(CPACK_SET_DESTDIR "OFF")
set(CPACK_SOURCE_GENERATOR "TBZ2;TGZ;TXZ;TZ")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/CPackSourceConfig.cmake")
set(CPACK_SOURCE_RPM "OFF")
set(CPACK_SOURCE_TBZ2 "ON")
set(CPACK_SOURCE_TGZ "ON")
set(CPACK_SOURCE_TXZ "ON")
set(CPACK_SOURCE_TZ "ON")
set(CPACK_SOURCE_ZIP "OFF")
set(CPACK_SYSTEM_NAME "Linux")
set(CPACK_THREADS "1")
set(CPACK_TOPLEVEL_TAG "Linux")
set(CPACK_WIX_INSTALL_SCOPE "perMachine")
set(CPACK_WIX_SIZEOF_VOID_P "8")

if(NOT CPACK_PROPERTIES_FILE)
  set(CPACK_PROPERTIES_FILE "/home/khalils/Desktop/Projects/PFA/netlist_converter/build/CPackProperties.cmake")
endif()

if(EXISTS ${CPACK_PROPERTIES_FILE})
  include(${CPACK_PROPERTIES_FILE})
endif()

# Configuration for component "COIN"

SET(CPACK_COMPONENTS_ALL COIN COINheaders OGDF OGDFheaders Unspecified)

# Configuration for component "COINheaders"

SET(CPACK_COMPONENTS_ALL COIN COINheaders OGDF OGDFheaders Unspecified)

# Configuration for component "OGDF"

SET(CPACK_COMPONENTS_ALL COIN COINheaders OGDF OGDFheaders Unspecified)

# Configuration for component "OGDFheaders"

SET(CPACK_COMPONENTS_ALL COIN COINheaders OGDF OGDFheaders Unspecified)
