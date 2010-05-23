# - Extract information from a mercurial working copy

# Example usage:
#  FIND_PACKAGE(Mercurial)
#  IF(Mercurial_FOUND)
#    Mercurial_WC_IDENTIFY(${PROJECT_SOURCE_DIR} Project)
#    MESSAGE("Current revision is ${Project_WC_REVISION}")
#    MESSAGE("Mercurial version is ${Mercurial_VERSION_HG}")
#  ENDIF(Mercurial_FOUND)


SET(Mercurial_FOUND FALSE)
SET(Mercurial_HG_FOUND FALSE)

FIND_PROGRAM(Mercurial_HG_EXECUTABLE hg
  DOC "mercurial command line client")
MARK_AS_ADVANCED(Mercurial_HG_EXECUTABLE)

IF(Mercurial_HG_EXECUTABLE)
  SET(Mercurial_HG_FOUND TRUE)
  SET(Mercurial_FOUND TRUE)

  MACRO(Mercurial_WC_IDENTIFY dir prefix)
    # the subversion commands should be executed with the C locale, otherwise
    # the message (which are parsed) may be translated, Alex
    SET(_Mercurial_SAVED_LC_ALL "$ENV{LC_ALL}")
    SET(ENV{LC_ALL} C)
    EXECUTE_PROCESS(COMMAND ${Mercurial_HG_EXECUTABLE} version
      WORKING_DIRECTORY ${dir}
      OUTPUT_VARIABLE Mercurial_VERSION_HG
      OUTPUT_STRIP_TRAILING_WHITESPACE)

    EXECUTE_PROCESS(COMMAND ${Mercurial_HG_EXECUTABLE} identify ${dir}
      OUTPUT_VARIABLE ${prefix}_WC_IDENTIFY
      ERROR_VARIABLE Mercurial_hg_identify_error
      RESULT_VARIABLE Mercurial_hg_identify_result
      OUTPUT_STRIP_TRAILING_WHITESPACE)

    EXECUTE_PROCESS(COMMAND ${Mercurial_HG_EXECUTABLE} status -m ${dir}
      OUTPUT_VARIABLE ${prefix}_WC_STATUS
      ERROR_VARIABLE Mercurial_hg_status_error
      RESULT_VARIABLE Mercurial_hg_status_result
      OUTPUT_STRIP_TRAILING_WHITESPACE)

    IF(NOT ${Mercurial_hg_identify_result} EQUAL 0)
      MESSAGE(SEND_ERROR "Command \"${Mercurial_HG_EXECUTABLE} identify ${dir}\" failed with output:\n${Mercurial_hg_identify_error}")
    ELSE(NOT ${Mercurial_hg_identify_result} EQUAL 0)

      STRING(REGEX REPLACE "^(.*\n)?Mercurial Distributed SCM [(]version ([.0-9]+).*"
        "\\2" Mercurial_VERSION_HG "${Mercurial_VERSION_HG}")
      STRING(REGEX REPLACE "^(.*\n)?([0-9a-f]+).*"
        "\\2" ${prefix}_WC_REVISION "${${prefix}_WC_IDENTIFY}")

    ENDIF(NOT ${Mercurial_hg_identify_result} EQUAL 0)

    # restore the previous LC_ALL
    SET(ENV{LC_ALL} ${_Mercurial_SAVED_LC_ALL})

  ENDMACRO(Mercurial_WC_IDENTIFY)



ENDIF(Mercurial_HG_EXECUTABLE)



IF(NOT Mercurial_FOUND)
  IF(NOT Mercurial_FIND_QUIETLY)
    MESSAGE(STATUS "Mercurial was not found.")
  ELSE(NOT Mercurial_FIND_QUIETLY)
    IF(Mercurial_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Mercurial was not found.")
    ENDIF(Mercurial_FIND_REQUIRED)
  ENDIF(NOT Mercurial_FIND_QUIETLY)
ENDIF(NOT Mercurial_FOUND)
