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


# THOMAS 
SET(OTB_LIBRARY_DIRS_CONFIG ${OTB_LIBRARY_DIRS_CONFIG} ${GDAL_LIBRARY_DIRS} ${ITK_LIBRARY_DIRS} )

# Determine the include directories needed.
SET(OTB_INCLUDE_DIRS_CONFIG
  ${OTB_INCLUDE_DIRS_BUILD_TREE}
#THOMAS
  ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
  ${OTB_INCLUDE_DIRS_SYSTEM} 
)

#-----------------------------------------------------------------------------
# Configure OTBConfig.cmake for the build tree.
CONFIGURE_FILE(${OTB_SOURCE_DIR}/OTBConfig.cmake.in
               ${OTB_BINARY_DIR}/OTBConfig.cmake @ONLY IMMEDIATE)

#-----------------------------------------------------------------------------
# Settings specific to the install tree.

# The "use" file.
SET(OTB_USE_FILE ${CMAKE_INSTALL_PREFIX}/lib/otb/UseOTB.cmake)

# The library dependencies file.
SET(OTB_LIBRARY_DEPENDS_FILE
    ${CMAKE_INSTALL_PREFIX}/lib/otb/OTBLibraryDepends.cmake)

# The build settings file.
SET(OTB_BUILD_SETTINGS_FILE
    ${CMAKE_INSTALL_PREFIX}/lib/otb/OTBBuildSettings.cmake)

# Include directories.
SET(OTB_INCLUDE_DIRS_CONFIG 
  ${OTB_INCLUDE_DIRS_INSTALL_TREE}
  ${OTB_INCLUDE_DIRS_SYSTEM}
)

# Link directories.
SET(OTB_LIBRARY_DIRS_CONFIG ${CMAKE_INSTALL_PREFIX}/lib/otb)

#THOMAS
SET(OTB_LIBRARY_DIRS_CONFIG ${OTB_LIBRARY_DIRS_CONFIG} ${GDAL_LIBRARY_DIRS} ${ITK_LIBRARY_DIRS} )

#-----------------------------------------------------------------------------
# Configure OTBConfig.cmake for the install tree.
#THOMAS
CONFIGURE_FILE(${OTB_SOURCE_DIR}/OTBConfig.cmake.in
               ${OTB_BINARY_DIR}/Utilities/OTBConfig.cmake @ONLY IMMEDIATE)
