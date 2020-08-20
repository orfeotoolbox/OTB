#
# This cmake module sets the project version and partial version
# variables by analysing the git tag and commit history. It expects git
# tags defined with semantic versioning 2.0.0 (http://semver.org/).
#
# The module expects the PROJECT_NAME variable to be set, and recognizes
# the GIT_FOUND, GIT_EXECUTABLE and VERSION_UPDATE_FROM_GIT variables.
# If Git is found and VERSION_UPDATE_FROM_GIT is set to boolean TRUE,
# the project version will be updated using information fetched from the
# most recent git tag and commit. Otherwise, the module will try to read
# a VERSION file containing the full and partial versions. The module
# will update this file each time the project version is updated.
#
# Once done, this module will define the following variables:
#
# ${PROJECT_NAME}_VERSION_STRING - Version string without metadata
# such as "v2.0.0" or "v1.2.41-beta.1". This should correspond to the
# most recent git tag.
# ${PROJECT_NAME}_VERSION_STRING_FULL - Version string with metadata
# such as "v2.0.0+3.a23fbc" or "v1.3.1-alpha.2+4.9c4fd1"
# ${PROJECT_NAME}_VERSION - Same as ${PROJECT_NAME}_VERSION_STRING,
# without the preceding 'v', e.g. "2.0.0" or "1.2.41-beta.1"
# ${PROJECT_NAME}_VERSION_MAJOR - Major version integer (e.g. 2 in v2.3.1-RC.2+21.ef12c8)
# ${PROJECT_NAME}_VERSION_MINOR - Minor version integer (e.g. 3 in v2.3.1-RC.2+21.ef12c8)
# ${PROJECT_NAME}_VERSION_PATCH - Patch version integer (e.g. 1 in v2.3.1-RC.2+21.ef12c8)
# ${PROJECT_NAME}_VERSION_TWEAK - Tweak version string (e.g. "RC.2" in v2.3.1-RC.2+21.ef12c8)
# ${PROJECT_NAME}_VERSION_AHEAD - How many commits ahead of last tag (e.g. 21 in v2.3.1-RC.2+21.ef12c8)
# ${PROJECT_NAME}_VERSION_GIT_SHA - The git sha1 of the most recent commit (e.g. the "ef12c8" in v2.3.1-RC.2+21.ef12c8)
#
# This module is public domain, use it as it fits you best.
#
# Author: Nuno Fachada


function(get_package_name root_repo_dir project_version_string)
  
  if(EXISTS "${root_repo_dir}/.git")  
    find_package(Git)
    if(NOT GIT_FOUND)
      message(ERROR "git not found. Make sure git can be found in your PATH")
      return()
    endif()
    
    message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")
    message(STATUS "VERSION MINOR: ${${PROJECT_NAME}_VERSION_MAJOR}")
    message(STATUS "VERSION MAJOR: ${${PROJECT_NAME}_VERSION_MINOR}")
    message(STATUS "VERSION PATCH: ${${PROJECT_NAME}_VERSION_PATCH}")


    if(DEFINED ENV{CI_COMMIT_REF_NAME})
      set(branch_name "$ENV{CI_COMMIT_REF_NAME}")
    else()
      execute_process(COMMAND ${GIT_EXECUTABLE} symbolic-ref -q HEAD
        WORKING_DIRECTORY ${root_repo_dir}
        OUTPUT_VARIABLE git_symbolic_ref_output
        OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)

      set(branch_name)
      if(git_symbolic_ref_output)
        get_filename_component(branch_name ${git_symbolic_ref_output} NAME)
      endif()
    endif()

    message(STATUS "branch_name: ${branch_name}")

    if("${branch_name}" MATCHES "^release-[0-9]+\\.[0-9]+\$")

      set(${project_version_string} "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}" PARENT_SCOPE)

    else()
      if(DEFINED ENV{CI_COMMIT_SHORT_SHA})
        set(${project_version_string} "${branch_name}-$ENV{CI_COMMIT_SHORT_SHA}" PARENT_SCOPE)
      else()
        execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
          WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
          OUTPUT_VARIABLE ${PROJECT_NAME}_COMMIT_SHA_STRING
          OUTPUT_STRIP_TRAILING_WHITESPACE)
        set(${project_version_string} "${branch_name}-${${PROJECT_NAME}_COMMIT_SHA_STRING}" PARENT_SCOPE)
      endif()

    endif()

  else()

    # Standalone source directory, get version from RELEASE_NOTE file
    file(STRINGS ${root_repo_dir}/RELEASE_NOTES.txt RN_FIRSTLINE LIMIT_COUNT 1)
    message(STATUS "${RN_FIRSTLINE}")
    string(REPLACE " " ";" RN_FIRSTLINE_LIST ${RN_FIRSTLINE})
    list(GET RN_FIRSTLINE_LIST 1 PROJECT_VERSION_STRING)
    message(STATUS ${PROJECT_VERSION_STRING})

    # Get partial versions into a list
    string(REGEX MATCHALL "-.*$|[0-9]+" PARTIAL_VERSION_LIST
      ${PROJECT_VERSION_STRING})

    list(GET PARTIAL_VERSION_LIST 0 _VERSION_MAJOR)
    list(GET PARTIAL_VERSION_LIST 1 _VERSION_MINOR)
    list(GET PARTIAL_VERSION_LIST 2 _VERSION_PATCH)
    
    message(STATUS "M: ${_VERSION_MAJOR}, m: ${_VERSION_MINOR}, p: ${_VERSION_PATCH}")

	  set(${project_version_string} "${PROJECT_VERSION_STRING}" PARENT_SCOPE)
    
  endif()

endfunction()
