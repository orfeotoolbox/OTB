#-----------------------------------------------------------------------------
# Set up CPack support, to be able to distribute OTB binary packages
#
OPTION(OTB_USE_CPACK "Generate installer using CPack" OFF)
MARK_AS_ADVANCED(OTB_USE_CPACK)

IF(OTB_USE_CPACK)

    # By default, do not warn when built on machines using only VS Express:
    IF(NOT DEFINED CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS)
      SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS ON)
    ENDIF()

    INCLUDE(InstallRequiredSystemLibraries)
    
    SET(CPACK_PACKAGE_NAME "OTB" CACHE STRING "Package name")
    MARK_AS_ADVANCED(CPACK_PACKAGE_NAME)
    
    SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Orfeo Toolbox")
    MARK_AS_ADVANCED(CPACK_PACKAGE_DESCRIPTION_SUMMARY)
    
    SET(CPACK_PACKAGE_VERSION  "${OTB_VERSION_STRING}")
    SET(CPACK_PACKAGE_VERSION_MAJOR "${OTB_VERSION_MAJOR}")
    SET(CPACK_PACKAGE_VERSION_MINOR "${OTB_VERSION_MINOR}")
    SET(CPACK_PACKAGE_VERSION_PATCH "${OTB_VERSION_PATCH}")
    
    SET(CPACK_PACKAGE_CONTACT "contact@orfeo-toolbox.org" CACHE STRING  "Orfeo toolbox contact email")
    
    SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Description.txt")
    
    #Debian specific
    SET(DEBIAN_PACKAGE_MAINTAINER  "debian@orfeo-toolbox.org" CACHE STRING  "Debian package maintainer email")
    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libgdal1-1.5.0 (>= 1.5.1-0), libfltk1.1 (>= 1.1.7-3), libcurl3 (>=7.15.5-1etch1), libfftw3-3 (>=3.1.2-3.1)" CACHE STRING "Debian package dependance" )
    SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64" CACHE STRING "arch")
    MARK_AS_ADVANCED(CPACK_DEBIAN_PACKAGE_ARCHITECTURE)
    SET(CPACK_DEBIAN_PACKAGE_NAME "libotb" CACHE STRING "Debian package name")
    
    SET(CPACK_PACKAGE_INSTALL_DIRECTORY "OrfeoToolbox-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}")
    
    SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright/OTBCopyright.txt")
    
    FILE(READ ${CPACK_PACKAGE_DESCRIPTION_FILE} CPACK_RPM_PACKAGE_DESCRIPTION)
    FILE(READ ${CPACK_PACKAGE_DESCRIPTION_FILE} CPACK_DEBIAN_PACKAGE_DESCRIPTION)
    
    IF(WIN32 AND NOT UNIX AND NOT CYGWIN)
    #Find gdal dll files, localized in the GDAL_LIBRARY directory
    GET_FILENAME_COMPONENT(GDAL_LIB_DIR "${GDAL_LIBRARY}" PATH )
    SET(GDAL_LIB_DIR "${GDAL_LIB_DIR}/" )
    INSTALL(DIRECTORY ${GDAL_LIB_DIR}
            DESTINATION                 bin
            FILES_MATCHING  PATTERN     "*.dll" )
    INSTALL(DIRECTORY ${GDAL_LIB_DIR}
            DESTINATION                 lib
            FILES_MATCHING  PATTERN     "*.lib" )
    ENDIF(WIN32 AND NOT UNIX AND NOT CYGWIN)
    
    INCLUDE(CPack)

ENDIF(OTB_USE_CPACK)
