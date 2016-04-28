macro(macro_setup_cmake_sources pkg)

  message( "-- Configuring ${pkg} package")

  set(PACKAGE_PROJECT_DIR ${CMAKE_BINARY_DIR}/PACKAGE-${pkg})
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

  file(WRITE "${PACKAGE_PROJECT_DIR}/src/CMakeLists.txt"
  "cmake_minimum_required(VERSION 2.6)
   include(CMakeParseArguments)
   include(CMakeDetermineSystem)
   set(Monteverdi_SOURCE_DIR        \"${Monteverdi_SOURCE_DIR}\")
   set(Monteverdi_BINARY_DIR        \"${Monteverdi_BINARY_DIR}\")
   set(MONTEVERDI_INSTALL_DIR       \"${MONTEVERDI_INSTALL_DIR}\")
   set(Monteverdi_INSTALL_DATA_DIR  \"share/otb\")
   set(QT_PLUGINS_DIR               \"${QT_PLUGINS_DIR}\")
   set(QT_TRANSLATIONS_DIR          \"${QT_TRANSLATIONS_DIR}\")
   set(DEPENDENCIES_INSTALL_DIR     \"${DEPENDENCIES_INSTALL_DIR}\")
   set(OTB_INSTALL_DIR              \"${OTB_INSTALL_DIR}\")
   set(PACKAGE_SUPPORT_FILES_DIR    \"${OTB_SOURCE_DIR}/SuperBuild/Packaging/Files\")
   set(CMAKE_INSTALL_PREFIX         \"${CMAKE_INSTALL_PREFIX}\")
   set(ITK_VERSION_STRING           \"${cache_ITK_VERSION_STRING}\")
   set(PKG_GENERATE_XDK              ${PKG_GENERATE_XDK})
   ${EXTRA_CACHE_CONFIG}
   include(\"${SUPERBUILD_SOURCE_DIR}/Packaging/PackageHelper.cmake\")
   super_package(STAGE_DIR \"${ARCHIVE_NAME}\")" )

endmacro()

macro(macro_update_dependencies_list list_variable)
  add_custom_target(REMAKE
    COMMAND ${CMAKE_COMMAND} --build "${SUPERBUILD_BINARY_DIR}"
    WORKING_DIRECTORY "${SUPERBUILD_BINARY_DIR}"
    )
  if(WIN32 OR CMAKE_CROSSCOMPILING)
    add_custom_target(PACKAGE-check)
  else() #Unxies Using SuperBuild

    if(ENABLE_MONTEVERDI)
      add_custom_target(PACKAGE-check
        COMMAND ${CMAKE_COMMAND} --build "${SUPERBUILD_BINARY_DIR}/MVD/build"
        WORKING_DIRECTORY "${SUPERBUILD_BINARY_DIR}/MVD/build"
        DEPENDS REMAKE
        )
    else()
      add_custom_target(PACKAGE-check
        COMMAND ${CMAKE_COMMAND} --build "${SUPERBUILD_BINARY_DIR}/OTB/build"
        WORKING_DIRECTORY "${SUPERBUILD_BINARY_DIR}/OTB/build"
        DEPENDS REMAKE
        )
    endif(ENABLE_MONTEVERDI)
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
      ALL DEPENDS
      COMMAND ${ZIP_EXECUTABLE}
      "-r" "${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.zip" "${CMAKE_INSTALL_PREFIX}/${ARCHIVE_NAME}"
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
      DEPENDS PACKAGE-${pkg}-build
      )
  else()
    add_custom_target(PACKAGE-${pkg}
      ALL DEPENDS
      COMMAND ${MAKESELF_SCRIPT}
      "--target"
      "${ARCHIVE_NAME}"
      "${CMAKE_INSTALL_PREFIX}/${ARCHIVE_NAME}"
      "${ARCHIVE_NAME}.run"
      "${PACKAGE_LONG_NAME} ${PACKAGE_VERSION_STRING}"
      "./pkgsetup"
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
      DEPENDS PACKAGE-${pkg}-build
      )
  endif()

  set(PACKAGE_EXTENSION .run)
  if(WIN32 OR CMAKE_CROSSCOMPILING)
    set(PACKAGE_EXTENSION .zip)
  endif()

  #clean
  add_custom_target(PACKAGE-${pkg}-clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/PACKAGE-${pkg}"
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/PACKAGE-TOOLS"
    COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}${PACKAGE_EXTENSION}"
    COMMAND ${CMAKE_COMMAND} "${CMAKE_BINARY_DIR}" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )

endmacro(macro_create_targets_for_package)


#macro:  get_version
#args :
 # input_file         - Input cmake file where the version variable is set via cmake set() command.
 #                      Example: ${SUPERBUILD_SOURCE_DIR}/CMake/External_itk.cmake
 #match_string        - A match string to filter out required set() commands.
 #                      Example: "ITK_SB_VERSION" will get all set(ITK_SB_VERSION_MAJOR)
 #                      set(ITK_SB_VERSION_MINOR) set(ITK_SB_VERSION_PATCH)
 # cmake_var_to_check - Name of variable that has the version string stored using set(..)
 #                      command inside the input_file.
 #                      Example: ITK_SB_VERSION. This is same as match_string but not always
 # Sample usage:
 # get_version(
 # "${SUPERBUILD_SOURCE_DIR}/CMake/External_itk.cmake"
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
