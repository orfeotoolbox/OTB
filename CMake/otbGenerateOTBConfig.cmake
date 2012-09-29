# Generate the OTBConfig.cmake file in the build tree.  Also configure
# one for installation.  The file tells external projects how to use
# OTB.

#-----------------------------------------------------------------------------
# Settings specific to the build tree.

# The "use" file.
SET(OTB_USE_FILE ${OTB_BINARY_DIR}/UseOTB.cmake)

# The library dependencies file.
SET(OTB_LIBRARY_DEPENDS_FILE ${OTB_BINARY_DIR}/OTBLibraryDepends.cmake)

# The build settings file.
SET(OTB_BUILD_SETTINGS_FILE ${OTB_BINARY_DIR}/OTBBuildSettings.cmake)

# Library directory.
SET(OTB_LIBRARY_DIRS_CONFIG ${OTB_LIBRARY_PATH})

# Binary directory
SET(OTB_BINARY_DIRS_CONFIG ${EXECUTABLE_OUTPUT_PATH})

# Package directories (cmake files...)
SET(OTB_CMAKEMODULE_DIRS_CONFIG ${OTB_SOURCE_DIR}/CMake)

#            SET(OTB_LIBRARY_DIRS_CONFIG ${OTB_LIBRARY_DIRS_CONFIG} ${GDAL_LIBRARY_DIRS} ${ITK_LIBRARY_DIRS} ${FLTK_LIBRARY_DIRS})





# Determine the include directories needed.
SET(OTB_INCLUDE_DIRS_CONFIG
  ${OTB_INCLUDE_DIRS_BUILD_TREE}
  ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
)

#-----------------------------------------------------------------------------
# Configure OTBConfig.cmake for the BUILD tree.
CONFIGURE_FILE(${OTB_SOURCE_DIR}/CMake/OTBConfig.cmake.in
               ${OTB_BINARY_DIR}/OTBConfig.cmake @ONLY IMMEDIATE)

#-----------------------------------------------------------------------------
# Settings specific to the install tree.

# The library dependencies file.
SET(OTB_LIBRARY_DEPENDS_FILE "\${OTB_INSTALL_PREFIX}/${OTB_INSTALL_PACKAGE_DIR}/OTBLibraryDepends.cmake")

# The "use" file.
SET(OTB_USE_FILE \${OTB_INSTALL_PREFIX}/${OTB_INSTALL_PACKAGE_DIR}/UseOTB.cmake)

# The build settings file.
SET(OTB_BUILD_SETTINGS_FILE \${OTB_INSTALL_PREFIX}/${OTB_INSTALL_PACKAGE_DIR}/OTBBuildSettings.cmake)


# Include directories.
SET(OTB_INCLUDE_DIRS_CONFIG \${OTB_INSTALL_PREFIX}/${OTB_INSTALL_INCLUDE_DIR})
FOREACH(DIR ${OTB_INCLUDE_RELATIVE_DIRS})
  LIST(APPEND OTB_INCLUDE_DIRS_CONFIG \${OTB_INSTALL_PREFIX}/${OTB_INSTALL_INCLUDE_DIR}/${DIR})
ENDFOREACH(DIR)

# Add absolue includes paths
SET(OTB_INCLUDE_DIRS_CONFIG ${OTB_INCLUDE_DIRS_CONFIG}
        ${OTB_INCLUDE_ABSOLUE_DIRS}  )

# Link directories.
SET(OTB_LIBRARY_DIRS_CONFIG "\${OTB_INSTALL_PREFIX}/${OTB_INSTALL_LIB_DIR}")

# Binary directories
SET(OTB_BINARY_DIRS_CONFIG "\${OTB_INSTALL_PREFIX}/${OTB_INSTALL_BIN_DIR}")

# Package directories (cmake files...)
SET(OTB_CMAKEMODULE_DIRS_CONFIG "\${OTB_INSTALL_PREFIX}/${OTB_INSTALL_CMAKEMODULE_DIR}")

#-----------------------------------------------------------------------------
# Configure OTBConfig.cmake for the INSTALL tree.

# Construct the proper number of GET_FILENAME_COMPONENT(... PATH)
# calls to compute the installation prefix.
STRING(REGEX REPLACE "/" ";" OTB_INSTALL_PACKAGE_DIR_COUNT
  "${OTB_INSTALL_PACKAGE_DIR}")
SET(OTB_CONFIG_CODE "
# Compute the installation prefix from this OTBConfig.cmake file location.
GET_FILENAME_COMPONENT(OTB_INSTALL_PREFIX \"\${CMAKE_CURRENT_LIST_FILE}\" PATH)")
FOREACH(p ${OTB_INSTALL_PACKAGE_DIR_COUNT})
  SET(OTB_CONFIG_CODE
    "${OTB_CONFIG_CODE}\nGET_FILENAME_COMPONENT(OTB_INSTALL_PREFIX \"\${OTB_INSTALL_PREFIX}\" PATH)"
    )
ENDFOREACH(p)

CONFIGURE_FILE(${OTB_SOURCE_DIR}/CMake/OTBConfig.cmake.in
               ${OTB_BINARY_DIR}/Utilities/OTBConfig.cmake @ONLY IMMEDIATE)
               
