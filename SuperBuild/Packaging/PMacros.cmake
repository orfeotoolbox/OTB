macro(macro_setup_cmake_sources pkg)

  message( "-- Configuring ${pkg} package")

  execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${PACKAGE_PROJECT_DIR}/build")

  #Easy way to have the write "PACKAGE_PROJECT_DIR/src/CMakeLists.txt"
  #inside foreach()
  if("${pkg}" STREQUAL "XDK")
    set(PACKAGE_PLATFORM_NAME "xdk-${PACKAGE_PLATFORM_NAME}")
    set(PKG_GENERATE_XDK ON)
  else()
    set(PKG_GENERATE_XDK OFF)
  endif()
  #set archive name inside loop
  set(ARCHIVE_NAME ${PACKAGE_NAME}-${PACKAGE_VERSION_STRING}-${PACKAGE_PLATFORM_NAME}${PACKAGE_ARCH})

  if(WIN32 OR CMAKE_CROSSCOMPILING)
    set(cache_Monteverdi_SOURCE_DIR "${Monteverdi_SOURCE_DIR}")
    set(cache_Monteverdi_BINARY_DIR "${Monteverdi_BINARY_DIR}")
    set(cache_QT_PLUGINS_DIR "${QT_PLUGINS_DIR}")
    set(cache_QT_TRANSLATIONS_DIR "${QT_TRANSLATIONS_DIR}")
    set(cache_PKG_INSTALL_PREFIX "${PKG_INSTALL_PREFIX}")
    set(cache_CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
    #guess install directory from OTB_MODULES_DIR
    set(cache_OTB_INSTALL_DIR "${OTB_MODULES_DIR}/../../../..")
    set(cache_ITK_VERSION_STRING)
    set(EXTRA_CACHE_CONFIG "
        set(CMAKE_CROSSCOMPILING ON)
        set(MXE_ARCH \"${MXE_ARCH}\")
        set(MXE_MXEROOT \"${MXE_MXEROOT}\")")
  else() #unixes
    set(cache_Monteverdi_SOURCE_DIR "${SUPERBUILD_BINARY_DIR}/MVD/src/MVD")
    set(cache_Monteverdi_BINARY_DIR "${SUPERBUILD_BINARY_DIR}/MVD/build")
    set(cache_PACKAGE_SUPPORT_FILES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Files")
    set(cache_QT_PLUGINS_DIR "${PKG_INSTALL_PREFIX}/plugins")
    set(cache_QT_TRANSLATIONS_DIR "${PKG_INSTALL_PREFIX}/translations")
    set(cache_PKG_INSTALL_PREFIX "${PKG_INSTALL_PREFIX}")
    set(cache_CMAKE_INSTALL_PREFIX "${PKG_INSTALL_PREFIX}")
    set(cache_OTB_INSTALL_DIR "${SUPERBUILD_BINARY_DIR}/OTB/build")
    set(cache_ITK_VERSION_STRING "${get_version_ITK_SB_VERSION}")
    set(EXTRA_CACHE_CONFIG)
  endif()

  file(WRITE "${PACKAGE_PROJECT_DIR}/src/CMakeLists.txt"
  "cmake_minimum_required(VERSION 2.6)
   include(CMakeParseArguments)
   include(CMakeDetermineSystem)
   set(Monteverdi_SOURCE_DIR \"${cache_Monteverdi_SOURCE_DIR}\")
   set(Monteverdi_BINARY_DIR \"${cache_Monteverdi_BINARY_DIR}\")
   set(PACKAGE_SUPPORT_FILES_DIR \"${OTB_SOURCE_DIR}/SuperBuild/Packaging\")
   set(QT_PLUGINS_DIR \"${cache_QT_PLUGINS_DIR}\")
   set(QT_TRANSLATIONS_DIR \"${cache_QT_TRANSLATIONS_DIR}\")
   set(PKG_INSTALL_PREFIX \"${cache_PKG_INSTALL_PREFIX}\")
   set(CMAKE_INSTALL_PREFIX \"${cache_CMAKE_INSTALL_PREFIX}\")
   set(OTB_INSTALL_DIR ${cache_OTB_INSTALL_DIR})
   set(ITK_VERSION_STRING \"${cache_ITK_VERSION_STRING}\")
   set(Monteverdi_INSTALL_DATA_DIR \"share/otb\")
   set(PKG_GENERATE_XDK ${PKG_GENERATE_XDK})
   ${EXTRA_CACHE_CONFIG}
   include(\"${OTB_SUPERBUILD_SOURCE_DIR}/CMake/PackageHelper.cmake\")
   super_package(STAGE_DIR \"${ARCHIVE_NAME}\")" )

endmacro()

macro(macro_update_dependencies_list list_variable)
  if(WIN32 OR CMAKE_CROSSCOMPILING)
    add_custom_target(PACKAGE-check)
  else() #Unxies Using SuperBuild
    if(OUT_OF_SOURCE_BUILD)
      if(ENABLE_MONTEVERDI)
        add_custom_target(PACKAGE-check
          COMMAND ${CMAKE_COMMAND} --build "${SUPERBUILD_BINARY_DIR}/MVD/build"
          WORKING_DIRECTORY "${SUPERBUILD_BINARY_DIR}/MVD/build"
          DEPENDS PACKAGE-check-otb
          )
      else()
        add_custom_target(PACKAGE-check
          COMMAND ${CMAKE_COMMAND} --build "${SUPERBUILD_BINARY_DIR}/OTB/build"
          WORKING_DIRECTORY "${SUPERBUILD_BINARY_DIR}/OTB/build"
          )
      endif(ENABLE_MONTEVERDI)
    else(OUT_OF_SOURCE_BUILD)
      add_custom_target(PACKAGE-check)
    endif(OUT_OF_SOURCE_BUILD)
  endif()
  list(APPEND ${list_variable} PACKAGE-check)
endmacro()

macro(macro_create_targets_for_package pkg)
  #configure
  add_custom_target(PACKAGE-${pkg}-configure
    COMMAND ${CMAKE_COMMAND}
    "${PACKAGE_PROJECT_DIR}/src"
    WORKING_DIRECTORY "${PACKAGE_PROJECT_DIR}/build"
    DEPENDS ${PACKAGE-configure_DEPENDS}
    )

  #build
  add_custom_target(PACKAGE-${pkg}-build
    COMMAND ${CMAKE_COMMAND}
    "--build" "${PACKAGE_PROJECT_DIR}/build" "--target" "install"
    WORKING_DIRECTORY "${PACKAGE_PROJECT_DIR}/build"
    DEPENDS PACKAGE-${pkg}-configure
    )

  #create package
  # creation of package is different from windows and unix like
  if(WIN32 OR CMAKE_CROSSCOMPILING)
    add_custom_target(PACKAGE-${pkg}
      COMMAND ${ZIP_EXECUTABLE}
      "-r" "${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.zip" "${PKG_INSTALL_PREFIX}/${ARCHIVE_NAME}"
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
      DEPENDS PACKAGE-${pkg}-build
      )
  else()
    add_custom_target(PACKAGE-${pkg}
      ALL DEPENDS
      COMMAND ${MAKESELF_SCRIPT}
      "--target"
      "${ARCHIVE_NAME}"
      "${PKG_INSTALL_PREFIX}/${ARCHIVE_NAME}"
      "${ARCHIVE_NAME}.run"
      "${PACKAGE_LONG_NAME} ${PACKAGE_VERSION_STRING}"
      "./pkgsetup"
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
      DEPENDS PACKAGE-${pkg}-build
      )
  endif()

  #clean
  add_custom_target(PACKAGE-${pkg}-clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${PACKAGE}-PACKAGE"
    COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}${PACKAGE_EXTENSION}"
    COMMAND ${CMAKE_COMMAND} "${CMAKE_BINARY_DIR}" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
endmacro(macro_create_targets_for_package)


#macro:  get_version
#args :
 # input_file         - Input cmake file where the version variable is set via cmake set() command.
 #                      Example: ${OTB_SUPERBUILD_SOURCE_DIR}/CMake/External_itk.cmake
 #match_string        - A match string to filter out required set() commands.
 #                      Example: "ITK_SB_VERSION" will get all set(ITK_SB_VERSION_MAJOR)
 #                      set(ITK_SB_VERSION_MINOR) set(ITK_SB_VERSION_PATCH)
 # cmake_var_to_check - Name of variable that has the version string stored using set(..)
 #                      command inside the input_file.
 #                      Example: ITK_SB_VERSION. This is same as match_string but not always
 # Sample usage:
 # get_version(
 # "${OTB_SUPERBUILD_SOURCE_DIR}/CMake/External_itk.cmake"
 # "ITK_SB_VERSION"
 #  ITK_SB_VERSION)
macro(get_version input_file match_string check_cmake_var)
  if(EXISTS "${input_file}")
    file(STRINGS "${input_file}" _version_vars   REGEX "set\\\(${match_string}")
    set(temp_file_name "${CMAKE_BINARY_DIR}/CMakeFiles/version_vars_${match_string}.cmake")
    file(WRITE "${temp_file_name}" "#version\n")
    foreach(_version_var ${_version_vars})
      file(APPEND "${temp_file_name}" "${_version_var}\n")
    endforeach()
    include("${temp_file_name}")
    if(${check_cmake_var})
      set(get_version_${check_cmake_var} "${${check_cmake_var}}")
    else()
      message(FATAL_ERROR "macro(get_version): Cannot find ${check_cmake_var}!")
    endif()
  else()
    message(FATAL_ERROR "macro(get_version): File '${input_file}' does not exists")
  endif()
endmacro()
