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


SET(OTB_LIBRARY_DIRS_CONFIG ${OTB_LIBRARY_DIRS_CONFIG} ${GDAL_LIBRARY_DIRS} ${ITK_LIBRARY_DIRS} ${FLTK_LIBRARY_DIRS})

# Determine the include directories needed.
SET(OTB_INCLUDE_DIRS_CONFIG
  ${OTB_INCLUDE_DIRS_BUILD_TREE}
  ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
  ${OTB_INCLUDE_DIRS_SYSTEM} 
)

# Built Configure FLTK commands and directories
IF(OTB_USE_EXTERNAL_FLTK)
        SET(OTB_FLTK_FLUID_EXECUTABLE ${FLTK_FLUID_EXECUTABLE})
ELSE(OTB_USE_EXTERNAL_FLTK)
        SET(OTB_FLTK_FLUID_EXECUTABLE ${FLUID_COMMAND})
ENDIF(OTB_USE_EXTERNAL_FLTK)

SET(OTB_FLUID_COMMAND  ${FLUID_COMMAND})
SET(OTB_FLTK_LIBRARY_DIRS ${FLTK_LIBRARY_DIRS})
SET(OTB_FLTK_EXECUTABLE_DIRS ${FLTK_EXECUTABLE_DIRS})

#-----------------------------------------------------------------------------
# Configure OTBConfig.cmake for the BUILD tree.
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

SET(OTB_LIBRARY_DIRS_CONFIG ${OTB_LIBRARY_DIRS_CONFIG} ${GDAL_LIBRARY_DIRS} ${ITK_LIBRARY_DIRS} ${FLTK_LIBRARY_DIRS})

# Install Configure FLTK commands and directories
IF(OTB_USE_EXTERNAL_FLTK)
        SET(OTB_FLTK_FLUID_EXECUTABLE ${FLTK_FLUID_EXECUTABLE})
        SET(OTB_FLUID_COMMAND  ${FLUID_COMMAND})
        SET(OTB_FLTK_LIBRARY_DIRS ${FLTK_LIBRARY_DIRS})
        SET(OTB_FLTK_EXECUTABLE_DIRS ${FLTK_EXECUTABLE_DIRS})
ELSE(OTB_USE_EXTERNAL_FLTK)
        GET_FILENAME_COMPONENT(FLUID_FILE_NAME "${FLUID_COMMAND}" NAME )
        SET(OTB_FLUID_COMMAND ${CMAKE_INSTALL_PREFIX}/bin/${FLUID_FILE_NAME})
        SET(OTB_FLTK_FLUID_EXECUTABLE ${OTB_FLUID_COMMAND})
        SET(OTB_FLTK_LIBRARY_DIRS ${CMAKE_INSTALL_PREFIX}/lib)
        SET(OTB_FLTK_EXECUTABLE_DIRS ${CMAKE_INSTALL_PREFIX}/bin)
ENDIF(OTB_USE_EXTERNAL_FLTK)


#-----------------------------------------------------------------------------
# Configure OTBConfig.cmake for the INSTALL tree.
CONFIGURE_FILE(${OTB_SOURCE_DIR}/OTBConfig.cmake.in
               ${OTB_BINARY_DIR}/Utilities/OTBConfig.cmake @ONLY IMMEDIATE)
