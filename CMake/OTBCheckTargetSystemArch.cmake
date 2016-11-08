macro(OTB_CHECK_TARGET_SYSTEM_ARCH _RESULT1 _RESULT2)
  set(${_RESULT1} "x64")
  set(${_RESULT2} TRUE)

  if(MSVC)
    execute_process(
      COMMAND ${CMAKE_C_COMPILER}
      ERROR_VARIABLE ev
      OUTPUT_VARIABLE ov
      OUTPUT_QUIET
      OUTPUT_STRIP_TRAILING_WHITESPACE
      )

    if("${ev}" MATCHES "x86")
      set(${_RESULT1} "x86")
      set(${_RESULT2} FALSE)
    endif()
  endif() #MSVC

  if(UNIX)
    execute_process(
      COMMAND uname -m
      ERROR_VARIABLE ev
      OUTPUT_VARIABLE ov
      OUTPUT_STRIP_TRAILING_WHITESPACE
      )
    
    set(${_RESULT1} "${ov}")
    set(${_RESULT2} FALSE)
    if("${ov}" STREQUAL "x86_64")
      set(${_RESULT2} TRUE)
    endif()
  endif()

  if(WIN32 AND NOT MSVC)
    if(MXE_TARGET_DIR)
      if(MXE_TARGET_DIR MATCHES "i686")
	set(${_RESULT1} "i686")
	set(${_RESULT2} FALSE)
      elseif(MXE_TARGET_DIR MATCHES "x86_64")
	set(${_RESULT1} "x86_64")
	set(${_RESULT2} TRUE)
      endif()
    else()
      message(STATUS "MXE_TARGET_DIR not set!")
    endif()
  endif()
endmacro()

#call macro
OTB_CHECK_TARGET_SYSTEM_ARCH(OTB_TARGET_SYSTEM_ARCH OTB_TARGET_SYSTEM_ARCH_IS_X64)
