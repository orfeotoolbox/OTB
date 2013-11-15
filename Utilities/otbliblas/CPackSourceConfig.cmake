# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. Example variables are:
#   CPACK_GENERATOR                     - Generator used to create package
#   CPACK_INSTALL_CMAKE_PROJECTS        - For each project (path, name, component)
#   CPACK_CMAKE_GENERATOR               - CMake Generator used for the projects
#   CPACK_INSTALL_COMMANDS              - Extra commands to install components
#   CPACK_INSTALL_DIRECTORIES           - Extra directories to install
#   CPACK_PACKAGE_DESCRIPTION_FILE      - Description file for the package
#   CPACK_PACKAGE_DESCRIPTION_SUMMARY   - Summary of the package
#   CPACK_PACKAGE_EXECUTABLES           - List of pairs of executables and labels
#   CPACK_PACKAGE_FILE_NAME             - Name of the package generated
#   CPACK_PACKAGE_ICON                  - Icon used for the package
#   CPACK_PACKAGE_INSTALL_DIRECTORY     - Name of directory for the installer
#   CPACK_PACKAGE_NAME                  - Package project name
#   CPACK_PACKAGE_VENDOR                - Package project vendor
#   CPACK_PACKAGE_VERSION               - Package project version
#   CPACK_PACKAGE_VERSION_MAJOR         - Package project version (major)
#   CPACK_PACKAGE_VERSION_MINOR         - Package project version (minor)
#   CPACK_PACKAGE_VERSION_PATCH         - Package project version (patch)

# There are certain generator specific ones

# NSIS Generator:
#   CPACK_PACKAGE_INSTALL_REGISTRY_KEY  - Name of the registry key for the installer
#   CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS - Extra commands used during uninstall
#   CPACK_NSIS_EXTRA_INSTALL_COMMANDS   - Extra commands used during install


set(CPACK_BINARY_BUNDLE "OFF")
set(CPACK_BINARY_CYGWIN "")
set(CPACK_BINARY_DEB "OFF")
set(CPACK_BINARY_DRAGNDROP "OFF")
set(CPACK_BINARY_NSIS "OFF")
set(CPACK_BINARY_OSXX11 "OFF")
set(CPACK_BINARY_PACKAGEMAKER "ON")
set(CPACK_BINARY_RPM "OFF")
set(CPACK_BINARY_STGZ "ON")
set(CPACK_BINARY_TBZ2 "OFF")
set(CPACK_BINARY_TGZ "ON")
set(CPACK_BINARY_TZ "")
set(CPACK_BINARY_ZIP "")
set(CPACK_CMAKE_GENERATOR "Unix Makefiles")
set(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
set(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
set(CPACK_GENERATOR "TGZ;ZIP;TBZ2")
set(CPACK_IGNORE_FILES "/test/data/TO_core_last_zoom.las;_CPack_Packages;.gz;.bz2;.zip;.hg;README;HOWTORELEASE.txt;doc;CMakeCache.txt;CPackConfig.cmake;schemas;hobu-config.bat;liblas-osgeo4w-init.bat;liblas-osgeo4w-start.bat.tmpl")
set(CPACK_INSTALLED_DIRECTORIES "/tmp/liblas;/")
set(CPACK_INSTALL_CMAKE_PROJECTS "")
set(CPACK_INSTALL_PREFIX "/usr/local")
set(CPACK_MODULE_PATH "/tmp/liblas/cmake/modules")
set(CPACK_NSIS_DISPLAY_NAME "libLAS 1.6.0b2")
set(CPACK_NSIS_INSTALLER_ICON_CODE "")
set(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
set(CPACK_NSIS_PACKAGE_NAME "libLAS 1.6.0b2")
set(CPACK_OUTPUT_CONFIG_FILE "/tmp/liblas/CPackConfig.cmake")
set(CPACK_PACKAGE_DEFAULT_LOCATION "/")
set(CPACK_PACKAGE_DESCRIPTION_FILE "/Applications/CMake 2.8-0.app/Contents/share/cmake-2.8/Templates/CPack.GenericDescription.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "libLAS built using CMake")
set(CPACK_PACKAGE_FILE_NAME "libLAS-1.6.0b2")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "libLAS 1.6.0b2")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "libLAS 1.6.0b2")
set(CPACK_PACKAGE_NAME "libLAS")
set(CPACK_PACKAGE_RELOCATABLE "true")
set(CPACK_PACKAGE_VENDOR "Humanity")
set(CPACK_PACKAGE_VERSION "1.6.0b2")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "6")
set(CPACK_PACKAGE_VERSION_PATCH "0b2")
set(CPACK_RESOURCE_FILE_LICENSE "/Applications/CMake 2.8-0.app/Contents/share/cmake-2.8/Templates/CPack.GenericLicense.txt")
set(CPACK_RESOURCE_FILE_README "/Applications/CMake 2.8-0.app/Contents/share/cmake-2.8/Templates/CPack.GenericDescription.txt")
set(CPACK_RESOURCE_FILE_WELCOME "/Applications/CMake 2.8-0.app/Contents/share/cmake-2.8/Templates/CPack.GenericWelcome.txt")
set(CPACK_SET_DESTDIR "OFF")
set(CPACK_SOURCE_CYGWIN "")
set(CPACK_SOURCE_GENERATOR "TGZ;ZIP;TBZ2")
set(CPACK_SOURCE_IGNORE_FILES "/test/data/TO_core_last_zoom.las;_CPack_Packages;.gz;.bz2;.zip;.hg;README;HOWTORELEASE.txt;doc;CMakeCache.txt;CPackConfig.cmake;schemas;hobu-config.bat;liblas-osgeo4w-init.bat;liblas-osgeo4w-start.bat.tmpl")
set(CPACK_SOURCE_INSTALLED_DIRECTORIES "/tmp/liblas;/")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE "/tmp/liblas/CPackSourceConfig.cmake")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "libLAS-1.6.0b2")
set(CPACK_SOURCE_TBZ2 "")
set(CPACK_SOURCE_TGZ "")
set(CPACK_SOURCE_TOPLEVEL_TAG "Darwin-Source")
set(CPACK_SOURCE_TZ "")
set(CPACK_SOURCE_ZIP "")
set(CPACK_STRIP_FILES "")
set(CPACK_SYSTEM_NAME "Darwin")
set(CPACK_TOPLEVEL_TAG "Darwin-Source")
