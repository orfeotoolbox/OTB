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


function(get_version root_repo_dir project_name project_version_string project_version_string_full project_version project_version_major project_version_minor project_version_patch project_version_tweak project_version_git_sha)
  
  if(EXISTS "${root_repo_dir}/.git")
  
    find_package(Git)
    if(NOT GIT_FOUND)
      message(ERROR "git not found. Make sure git can be found in your PATH")
      return()
    endif()


    message(STATUS "CI_COMMIT_TAG : ${CI_COMMIT_TAG}")
    message(STATUS "CI_COMMIT_REF_NAME : $CI_COMMIT_REF_NAME")
    message(STATUS "CI_COMMIT_SHORT_SHA : ${CI_COMMIT_SHORT_SHA}")



    execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE ${PROJECT_NAME}_COMMIT_STRING
      OUTPUT_STRIP_TRAILING_WHITESPACE)

    message(STATUS "_COMMIT_STRING : ${${PROJECT_NAME}_COMMIT_STRING}")


    # Get last tag from git
    execute_process(COMMAND ${GIT_EXECUTABLE} describe --abbrev=0 --tags
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE ${PROJECT_NAME}_VERSION_STRING
      OUTPUT_STRIP_TRAILING_WHITESPACE)
      
    message(STATUS ${${PROJECT_NAME}_VERSION_STRING})
    
    execute_process(COMMAND ${GIT_EXECUTABLE} describe
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE ${PROJECT_NAME}_VERSION_STRING_2
      OUTPUT_STRIP_TRAILING_WHITESPACE)

    message(STATUS ${${PROJECT_NAME}_VERSION_STRING})
    message(STATUS ${${PROJECT_NAME}_VERSION_STRING_2})
    
    #output string format : <semver_tag>-<commit_distance>-<commit_hash> or <semver_tag> if the HEAD is a tag


    if("${${PROJECT_NAME}_VERSION_STRING_2}" STREQUAL "${${PROJECT_NAME}_VERSION_STRING}")
      set(HEAD_IS_TAG 1)
    else()
      set(HEAD_IS_TAG 0)
    endif()

    
    if(NOT HEAD_IS_TAG)
    
      string(FIND ${${PROJECT_NAME}_VERSION_STRING_2} "-" position REVERSE)
      string(LENGTH ${${PROJECT_NAME}_VERSION_STRING_2} str_length)
      message(STATUS ${position})
      math(EXPR pos2 "${position} + 1" )
      message(STATUS ${str_length})  
      math(EXPR pos_sub_str "${str_length} - ${pos2}")
      message(STATUS ${pos_sub_str})
      string(SUBSTRING ${${PROJECT_NAME}_VERSION_STRING_2} ${pos2} ${pos_sub_str} project_version_git_sha)
      message(STATUS ${project_version_git_sha})

      string(SUBSTRING ${${PROJECT_NAME}_VERSION_STRING_2} 0 ${position} SUB_STRING)
      message(STATUS ${SUB_STRING})
      string(FIND ${SUB_STRING} "-" position2 REVERSE)
      string(LENGTH ${SUB_STRING} str2_length)
      math(EXPR pos3 "${position2} + 1" )
      message(STATUS ${str2_length})  
      math(EXPR pos_sub_str2 "${str2_length} - ${pos3}")
      message(STATUS ${pos_sub_str2})
      string(SUBSTRING ${SUB_STRING} ${pos3} ${pos_sub_str2} ${PROJECT_NAME}_VERSION_AHEAD)
      message(STATUS ${${PROJECT_NAME}_VERSION_AHEAD})
    endif()


    # Get partial versions into a list
    string(REGEX MATCHALL "-.*$|[0-9]+" "${PROJECT_NAME}_PARTIAL_VERSION_LIST" "${${PROJECT_NAME}_VERSION_STRING}")

    # Set the version numbers
    list(GET ${PROJECT_NAME}_PARTIAL_VERSION_LIST 0 ${PROJECT_NAME}_VERSION_MAJOR)
    list(GET ${PROJECT_NAME}_PARTIAL_VERSION_LIST 1 ${PROJECT_NAME}_VERSION_MINOR)
    list(GET ${PROJECT_NAME}_PARTIAL_VERSION_LIST 2 ${PROJECT_NAME}_VERSION_PATCH)

    # The tweak part is optional, so check if the list contains it
    list(LENGTH ${PROJECT_NAME}_PARTIAL_VERSION_LIST ${PROJECT_NAME}_PARTIAL_VERSION_LIST_LEN)
    if (${PROJECT_NAME}_PARTIAL_VERSION_LIST_LEN GREATER 3)
      list(GET ${PROJECT_NAME}_PARTIAL_VERSION_LIST 3 ${PROJECT_NAME}_VERSION_TWEAK)
      string(SUBSTRING ${${PROJECT_NAME}_VERSION_TWEAK} 1 -1 ${PROJECT_NAME}_VERSION_TWEAK)
    endif()

    # Unset the list
    unset(${PROJECT_NAME}_PARTIAL_VERSION_LIST)

    # Set full project version string
    if (NOT HEAD_IS_TAG)
      set(${PROJECT_NAME}_VERSION_STRING_FULL
        ${${PROJECT_NAME}_VERSION_STRING}+${${PROJECT_NAME}_VERSION_AHEAD}.${${PROJECT_NAME}_VERSION_GIT_SHA})
    else()
      set(${PROJECT_NAME}_VERSION_STRING_FULL
        ${${PROJECT_NAME}_VERSION_STRING})
    endif()
     
      set(${project_version_string} ${${PROJECT_NAME}_VERSION_STRING} PARENT_SCOPE)

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

	  set(PROJECT_VERSION_STRING_FULL ${PROJECT_VERSION_STRING})
    
  endif()


  # Set project version (without the preceding 'v')
  set(${PROJECT_NAME}_VERSION ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH})
  if (${PROJECT_NAME}_VERSION_TWEAK)
    set(${PROJECT_NAME}_VERSION ${${PROJECT_NAME}_VERSION}-${${PROJECT_NAME}_VERSION_TWEAK})
  endif()

endfunction()
