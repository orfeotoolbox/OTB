#-----------------------------------------------------------------------------
# Set up CPack support, to be able to distribute OTB binary packages
#
option(OTB_USE_CPACK "Generate installer using CPack" OFF)
mark_as_advanced(OTB_USE_CPACK)

if(OTB_USE_CPACK)

    # By default, do not warn when built on machines using only VS Express:
    if(NOT DEFINED CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS)
      set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_NO_WARNINGS ON)
    endif()

    include(InstallRequiredSystemLibraries)

    set(CPACK_PACKAGE_NAME "OTB" CACHE STRING "Package name")
    mark_as_advanced(CPACK_PACKAGE_NAME)

    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Orfeo Toolbox")
    mark_as_advanced(CPACK_PACKAGE_DESCRIPTION_SUMMARY)

    set(CPACK_PACKAGE_VERSION  "${OTB_VERSION_STRING}")
    set(CPACK_PACKAGE_VERSION_MAJOR "${OTB_VERSION_MAJOR}")
    set(CPACK_PACKAGE_VERSION_MINOR "${OTB_VERSION_MINOR}")
    set(CPACK_PACKAGE_VERSION_PATCH "${OTB_VERSION_PATCH}")

    set(CPACK_PACKAGE_CONTACT "contact@orfeo-toolbox.org" CACHE STRING  "Orfeo toolbox contact email")

    set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Description.txt")

    #Debian specific
    set(DEBIAN_PACKAGE_MAINTAINER  "debian@orfeo-toolbox.org" CACHE STRING  "Debian package maintainer email")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libgdal1-1.5.0 (>= 1.5.1-0), libfltk1.1 (>= 1.1.7-3), libcurl3 (>=7.15.5-1etch1), libfftw3-3 (>=3.1.2-3.1)" CACHE STRING "Debian package dependance" )
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64" CACHE STRING "arch")
    mark_as_advanced(CPACK_DEBIAN_PACKAGE_ARCHITECTURE)
    set(CPACK_DEBIAN_PACKAGE_NAME "libotb" CACHE STRING "Debian package name")

    set(CPACK_PACKAGE_INSTALL_DIRECTORY "OrfeoToolbox-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}")

    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright/OTBCopyright.txt")

    file(READ ${CPACK_PACKAGE_DESCRIPTION_FILE} CPACK_RPM_PACKAGE_DESCRIPTION)
    file(READ ${CPACK_PACKAGE_DESCRIPTION_FILE} CPACK_DEBIAN_PACKAGE_DESCRIPTION)

    if(WIN32 AND NOT UNIX AND NOT CYGWIN)
    #Find gdal dll files, localized in the GDAL_LIBRARY directory
    get_filename_component(GDAL_LIB_DIR "${GDAL_LIBRARY}" PATH )
    set(GDAL_LIB_DIR "${GDAL_LIB_DIR}/" )
    install(DIRECTORY ${GDAL_LIB_DIR}
            DESTINATION                 bin
            FILES_MATCHING  PATTERN     "*.dll" )
    install(DIRECTORY ${GDAL_LIB_DIR}
            DESTINATION                 lib
            FILES_MATCHING  PATTERN     "*.lib" )
    endif()

    include(CPack)

endif()
