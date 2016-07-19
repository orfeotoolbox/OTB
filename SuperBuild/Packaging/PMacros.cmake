macro(macro_setup_cmake_project pkg)

  message( "-- Configuring ${pkg} package")

  include(${SUPERBUILD_SOURCE_DIR}/CMake/External_pkgtools.cmake)

  #reset it again in macro(macro_create_targets_for_package pkg)
  #because thats the cmake macros way.
  set(PACKAGE_PROJECT_DIR ${CMAKE_BINARY_DIR}/PACKAGE-${pkg})
  execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${PACKAGE_PROJECT_DIR}/build")

  if("${pkg}" STREQUAL "XDK")
    set(archive_name ${PACKAGE_NAME}-${PACKAGE_VERSION_STRING}-xdk-${PACKAGE_PLATFORM_NAME}${PACKAGE_ARCH})
  else()
    set(archive_name ${PACKAGE_NAME}-${PACKAGE_VERSION_STRING}-${PACKAGE_PLATFORM_NAME}${PACKAGE_ARCH})
  endif()

  if("${pkg}" STREQUAL "XDK")
    set(PKG_GENERATE_XDK ON)
  else()
    set(PKG_GENERATE_XDK OFF)
  endif()


  #set archive name inside loop
  file(WRITE "${PACKAGE_PROJECT_DIR}/src/CMakeLists.txt"
  "cmake_minimum_required(VERSION 3.2)
   include(CMakeParseArguments)
   include(CMakeDetermineSystem)
   set(CMAKE_BUILD_TYPE             Release)
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
   set(ITK_VERSION_STRING           \"${ITK_VERSION_STRING}\")
   set(PKG_GENERATE_XDK              ${PKG_GENERATE_XDK})
   set(PATCHELF_PROGRAM              ${PATCHELF_PROGRAM})
   ${EXTRA_CACHE_CONFIG}
   include(\"${SUPERBUILD_SOURCE_DIR}/Packaging/PackageHelper.cmake\")
   macro_super_package(STAGE_DIR \"${archive_name}\")"
  )

  macro_create_targets_for_package(${pkg})

endmacro()

macro(macro_create_targets_for_package pkg)

  if(WIN32 AND NOT MSVC)
    add_custom_target(PACKAGE-${pkg}-check
      COMMAND ${CMAKE_COMMAND} --build "." --target install
      WORKING_DIRECTORY "${Monteverdi_BINARY_DIR}"
      )
  else() #Using SuperBuild
    #For out of source build,
    #we assume the otb is built correctly with superbuild
    if(OUT_OF_SOURCE_BUILD)
      add_custom_target(PACKAGE-${pkg}-check
        COMMAND ${CMAKE_COMMAND} -E echo "Building PACKAGE-${pkg}-check")
    else(OUT_OF_SOURCE_BUILD)
      if(ENABLE_MONTEVERDI)
        add_custom_target(PACKAGE-${pkg}-check
          COMMAND ${CMAKE_COMMAND} -E echo "Building PACKAGE-${pkg}-check"
	  DEPENDS MVD
          WORKING_DIRECTORY ${SUPERBUILD_BINARY_DIR}
          )
      else(ENABLE_MONTEVERDI)
        add_custom_target(PACKAGE-${pkg}-check
          COMMAND ${CMAKE_COMMAND} -E echo "Building PACKAGE-${pkg}-check"
	  DEPENDS OTB
          WORKING_DIRECTORY ${SUPERBUILD_BINARY_DIR}
          )
      endif(ENABLE_MONTEVERDI)
    endif(OUT_OF_SOURCE_BUILD)
  endif()

  add_dependencies(PACKAGE-${pkg}-check PACKAGE-TOOLS)

  if("${pkg}" STREQUAL "XDK")
    set(PACKAGE_PLATFORM_NAME_ "xdk-${PACKAGE_PLATFORM_NAME}")
    set(PKG_GENERATE_XDK ON)
  else()
    set(PACKAGE_PLATFORM_NAME_ "${PACKAGE_PLATFORM_NAME}")
    set(PKG_GENERATE_XDK OFF)
  endif()

  set(PACKAGE_PROJECT_DIR ${CMAKE_BINARY_DIR}/PACKAGE-${pkg})
  if("${pkg}" STREQUAL "XDK")
    set(archive_name ${PACKAGE_NAME}-${PACKAGE_VERSION_STRING}-xdk-${PACKAGE_PLATFORM_NAME}${PACKAGE_ARCH})
  else()
    set(archive_name ${PACKAGE_NAME}-${PACKAGE_VERSION_STRING}-${PACKAGE_PLATFORM_NAME}${PACKAGE_ARCH})
  endif()

  #configure
  add_custom_target(PACKAGE-${pkg}-configure
  COMMAND ${CMAKE_COMMAND} -E make_directory "${PACKAGE_PROJECT_DIR}/build"
   WORKING_DIRECTORY "${PACKAGE_PROJECT_DIR}"
    COMMAND ${CMAKE_COMMAND} "-G${CMAKE_GENERATOR}"
    "${PACKAGE_PROJECT_DIR}/src"
    WORKING_DIRECTORY "${PACKAGE_PROJECT_DIR}/build"
    DEPENDS PACKAGE-${pkg}-check
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
  if(WIN32)
    add_custom_target(PACKAGE-${pkg}
      COMMAND ${ZIP_EXECUTABLE} "a"
      "-ry" "${CMAKE_BINARY_DIR}/${archive_name}.zip" "${archive_name}/*"
      WORKING_DIRECTORY "${CMAKE_INSTALL_PREFIX}"
      DEPENDS PACKAGE-${pkg}-build
      COMMENT "Creating ${CMAKE_BINARY_DIR}/${archive_name}.zip"
      )
  else()
    add_custom_target(PACKAGE-${pkg}
      COMMAND ${MAKESELF_SCRIPT}
      "--quiet"
      "--target"
      "${archive_name}"
      "${CMAKE_INSTALL_PREFIX}/${archive_name}"
      "${archive_name}.run"
      "${PACKAGE_LONG_NAME} ${PACKAGE_VERSION_STRING}"
      "./pkgsetup"
      WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
      DEPENDS PACKAGE-${pkg}-build
      COMMENT "Creating ${CMAKE_CURRENT_BINARY_DIR}/${archive_name}.run"
      )
  endif()

  set(PACKAGE_EXTENSION .run)
  if(WIN32)
    set(PACKAGE_EXTENSION .zip)
  endif()

  #clean
  add_custom_target(PACKAGE-${pkg}-clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/PACKAGE-${pkg}"
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/PACKAGE-TOOLS"
    COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_BINARY_DIR}/${archive_name}${PACKAGE_EXTENSION}"
    COMMAND ${CMAKE_COMMAND} "${CMAKE_BINARY_DIR}"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
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