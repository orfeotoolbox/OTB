# Generate the OTBConfig.cmake file in the build tree.  Also configure
# one for installation.  The file tells external projects how to use
# OTB.

#-----------------------------------------------------------------------------
# Settings specific to the build tree.

# The "use" file.
set(OTB_USE_FILE ${OTB_BINARY_DIR}/UseOTB.cmake)

# The library dependencies file.
set(OTB_LIBRARY_DEPENDS_FILE ${OTB_BINARY_DIR}/OTBLibraryDepends.cmake)

# The build settings file.
#Comment Import/Export of build settings as it as been dropped by cmake 
# https://github.com/Kitware/CMake/commit/fd47a2ae966c3ac1fac1ee699728c55862fb23a2
#It is still used?
#set(OTB_BUILD_SETTINGS_FILE ${OTB_BINARY_DIR}/OTBBuildSettings.cmake)

# Library directory.
set(OTB_LIBRARY_DIRS_CONFIG ${OTB_LIBRARY_PATH})

# Binary directory
set(OTB_BINARY_DIRS_CONFIG ${EXECUTABLE_OUTPUT_PATH})

# Package directories (cmake files...)
set(OTB_CMAKEMODULE_DIRS_CONFIG ${OTB_SOURCE_DIR}/CMake)

# Determine the include directories needed.
set(OTB_INCLUDE_DIRS_CONFIG
  ${OTB_INCLUDE_DIRS_BUILD_TREE}
  ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
)

#-----------------------------------------------------------------------------
# Configure OTBConfig.cmake for the BUILD tree.
configure_file(${OTB_SOURCE_DIR}/CMake/OTBConfig.cmake.in
               ${OTB_BINARY_DIR}/OTBConfig.cmake @ONLY IMMEDIATE)

#-----------------------------------------------------------------------------
# Settings specific to the install tree.

# The library dependencies file.
set(OTB_LIBRARY_DEPENDS_FILE "\${OTB_INSTALL_PREFIX}/${OTB_INSTALL_PACKAGE_DIR}/OTBLibraryDepends.cmake")

# The "use" file.
set(OTB_USE_FILE \${OTB_INSTALL_PREFIX}/${OTB_INSTALL_PACKAGE_DIR}/UseOTB.cmake)

# The build settings file.
#set(OTB_BUILD_SETTINGS_FILE \${OTB_INSTALL_PREFIX}/${OTB_INSTALL_PACKAGE_DIR}/OTBBuildSettings.cmake)


# Include directories.
set(OTB_INCLUDE_DIRS_CONFIG \${OTB_INSTALL_PREFIX}/${OTB_INSTALL_INCLUDE_DIR})
foreach(DIR ${OTB_INCLUDE_RELATIVE_DIRS})
  list(APPEND OTB_INCLUDE_DIRS_CONFIG \${OTB_INSTALL_PREFIX}/${OTB_INSTALL_INCLUDE_DIR}/${DIR})
endforeach()

# Add absolue includes paths
set(OTB_INCLUDE_DIRS_CONFIG ${OTB_INCLUDE_DIRS_CONFIG}
        ${OTB_INCLUDE_ABSOLUE_DIRS}  )

# Link directories.
set(OTB_LIBRARY_DIRS_CONFIG "\${OTB_INSTALL_PREFIX}/${OTB_INSTALL_LIB_DIR}")

# Binary directories
set(OTB_BINARY_DIRS_CONFIG "\${OTB_INSTALL_PREFIX}/${OTB_INSTALL_BIN_DIR}")

# Package directories (cmake files...)
set(OTB_CMAKEMODULE_DIRS_CONFIG "\${OTB_INSTALL_PREFIX}/${OTB_INSTALL_CMAKEMODULE_DIR}")

#-----------------------------------------------------------------------------
# Configure OTBConfig.cmake for the INSTALL tree.

# Construct the proper number of get_filename_component(... PATH)
# calls to compute the installation prefix.
string(REGEX REPLACE "/" ";" OTB_INSTALL_PACKAGE_DIR_COUNT
  "${OTB_INSTALL_PACKAGE_DIR}")
set(OTB_CONFIG_CODE "
# Compute the installation prefix from this OTBConfig.cmake file location.
get_filename_component(OTB_INSTALL_PREFIX \"\${CMAKE_CURRENT_LIST_FILE}\" PATH)")
foreach(p ${OTB_INSTALL_PACKAGE_DIR_COUNT})
  set(OTB_CONFIG_CODE
    "${OTB_CONFIG_CODE}\nGET_FILENAME_COMPONENT(OTB_INSTALL_PREFIX \"\${OTB_INSTALL_PREFIX}\" PATH)"
    )
endforeach()

configure_file(${OTB_SOURCE_DIR}/CMake/OTBConfig.cmake.in
               ${OTB_BINARY_DIR}/Utilities/OTBConfig.cmake @ONLY IMMEDIATE)

