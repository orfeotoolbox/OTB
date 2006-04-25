# Generate the CAIConfig.cmake file in the build tree.  Also configure
# one for installation.  The file tells external projects how to use
# CAI.

#-----------------------------------------------------------------------------
# Settings specific to the build tree.

# The "use" file.
SET(CAI_USE_FILE ${CAI_BINARY_DIR}/UseCAI.cmake)

# The library dependencies file.
SET(CAI_LIBRARY_DEPENDS_FILE ${CAI_BINARY_DIR}/CAILibraryDepends.cmake)

# The build settings file.
SET(CAI_BUILD_SETTINGS_FILE ${CAI_BINARY_DIR}/CAIBuildSettings.cmake)

# Library directory.
SET(CAI_LIBRARY_DIRS_CONFIG ${CAI_LIBRARY_PATH})

# Determine the include directories needed.
SET(CAI_INCLUDE_DIRS_CONFIG
  ${CAI_INCLUDE_DIRS_BUILD_TREE}
  ${CAI_INCLUDE_DIRS_SOURCE_TREE}
  ${CAI_INCLUDE_DIRS_SYSTEM}
)

#-----------------------------------------------------------------------------
# Configure CAIConfig.cmake for the build tree.
CONFIGURE_FILE(${CAI_SOURCE_DIR}/CAIConfig.cmake.in
               ${CAI_BINARY_DIR}/CAIConfig.cmake @ONLY IMMEDIATE)

#-----------------------------------------------------------------------------
# Settings specific to the install tree.

# The "use" file.
SET(CAI_USE_FILE ${CMAKE_INSTALL_PREFIX}/lib/cai/UseCAI.cmake)

# The library dependencies file.
SET(CAI_LIBRARY_DEPENDS_FILE
    ${CMAKE_INSTALL_PREFIX}/lib/cai/CAILibraryDepends.cmake)

# The build settings file.
SET(CAI_BUILD_SETTINGS_FILE
    ${CMAKE_INSTALL_PREFIX}/lib/cai/CAIBuildSettings.cmake)

# Include directories.
SET(CAI_INCLUDE_DIRS_CONFIG
  ${CAI_INCLUDE_DIRS_INSTALL_TREE}
  ${CAI_INCLUDE_DIRS_SYSTEM}
)

# Link directories.
SET(CAI_LIBRARY_DIRS_CONFIG ${CMAKE_INSTALL_PREFIX}/lib/cai)

