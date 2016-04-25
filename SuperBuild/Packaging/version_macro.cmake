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
