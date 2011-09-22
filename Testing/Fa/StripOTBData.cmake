
FIND_PROGRAM(Mercurial_HG_EXECUTABLE hg)

IF(NOT Mercurial_HG_EXECUTABLE)
  MESSAGE(FATAL_ERROR "mercurial executable not found")
ENDIF()

EXECUTE_PROCESS(COMMAND ${Mercurial_HG_EXECUTABLE} --config 'extensions.mq=' strip 47f468703345
                WORKING_DIRECTORY ${OTB_DATA_ROOT}
                TIMEOUT 1500
                RESULT_VARIABLE result )


IF(NOT result EQUAL "0")
  MESSAGE(FATAL_ERROR "strip command failed")
ENDIF()

