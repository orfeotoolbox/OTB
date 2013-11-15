# - Extract information from a mercurial working copy

# Example usage:
#  find_package(Mercurial)
#  if(Mercurial_FOUND)
#    Mercurial_WC_IDENTIFY(${PROJECT_SOURCE_DIR} Project)
#    message("Current revision is ${Project_WC_REVISION}")
#    message("Mercurial version is ${Mercurial_VERSION_HG}")
#  endif()


set(Mercurial_FOUND FALSE)
set(Mercurial_HG_FOUND FALSE)

find_program(Mercurial_HG_EXECUTABLE hg
  DOC "mercurial command line client")
mark_as_advanced(Mercurial_HG_EXECUTABLE)

if(Mercurial_HG_EXECUTABLE)
  set(Mercurial_HG_FOUND TRUE)
  set(Mercurial_FOUND TRUE)

  macro(Mercurial_WC_IDENTIFY dir prefix)
    # the subversion commands should be executed with the C locale, otherwise
    # the message (which are parsed) may be translated, Alex
    set(_Mercurial_SAVED_LC_ALL "$ENV{LC_ALL}")
    set(ENV{LC_ALL} C)
    execute_process(COMMAND ${Mercurial_HG_EXECUTABLE} version
      WORKING_DIRECTORY ${dir}
      OUTPUT_VARIABLE Mercurial_VERSION_HG
      OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)

    execute_process(COMMAND ${Mercurial_HG_EXECUTABLE} identify ${dir}
      OUTPUT_VARIABLE ${prefix}_WC_IDENTIFY
#      ERROR_VARIABLE Mercurial_hg_identify_error
#      RESULT_VARIABLE Mercurial_hg_identify_result
      OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)

    execute_process(COMMAND ${Mercurial_HG_EXECUTABLE} status -m ${dir}
      OUTPUT_VARIABLE ${prefix}_WC_STATUS
#      ERROR_VARIABLE Mercurial_hg_status_error
#      RESULT_VARIABLE Mercurial_hg_status_result
      OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)

    if(NOT ${Mercurial_hg_identify_result} EQUAL 0)
      message(SEND_ERROR "Command \"${Mercurial_HG_EXECUTABLE} identify ${dir}\" failed with output:\n${Mercurial_hg_identify_error}")
    else()

      string(REGEX REPLACE "^(.*\n)?Mercurial Distributed SCM [(]version ([.0-9]+).*"
        "\\2" Mercurial_VERSION_HG "${Mercurial_VERSION_HG}")
      string(REGEX REPLACE "^(.*\n)?([0-9a-f]+).*"
        "\\2" ${prefix}_WC_REVISION "${${prefix}_WC_IDENTIFY}")

    endif()

    # restore the previous LC_ALL
    set(ENV{LC_ALL} ${_Mercurial_SAVED_LC_ALL})

  endmacro()



endif()



if(NOT Mercurial_FOUND)
  if(NOT Mercurial_FIND_QUIETLY)
    message(STATUS "Mercurial was not found.")
  else()
    if(Mercurial_FIND_REQUIRED)
      message(FATAL_ERROR "Mercurial was not found.")
    endif()
  endif()
endif()
