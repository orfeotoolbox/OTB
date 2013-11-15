
# ensure WIN32 definition
if(WIN32)
  add_definitions(-DWIN32)
endif()


# On Visual Studio 8 MS deprecated C. This removes all 1.276E1265 security
# warnings
if(WIN32)
    if(NOT CYGWIN)
      if(NOT MINGW)
          add_definitions(
            -D_CRT_FAR_MAPPINGS_NO_DEPRECATE
            -D_CRT_IS_WCTYPE_NO_DEPRECATE
            -D_CRT_MANAGED_FP_NO_DEPRECATE
            -D_CRT_NONSTDC_NO_DEPRECATE
            -D_CRT_SECURE_NO_DEPRECATE
            -D_CRT_SECURE_NO_DEPRECATE_GLOBALS
            -D_CRT_SETERRORMODE_BEEP_SLEEP_NO_DEPRECATE
            -D_CRT_TIME_FUNCTIONS_NO_DEPRECATE
            -D_CRT_VCCLRIT_NO_DEPRECATE
            -D_SCL_SECURE_NO_DEPRECATE
            )

        if(MSVC)
          add_definitions(-D_CRT_SECURE_NO_WARNINGS)
          add_definitions(-D_CRT_NONSTDC_NO_WARNING)
        endif()

      endif()
    endif()
endif()
