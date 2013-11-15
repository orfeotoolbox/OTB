find_program(NM nm REQUIRED)

execute_process(COMMAND ${NM} ${OPJ_TARGET_PATH}
                OUTPUT_VARIABLE NM_OUTPUT)

# get a list, one item for each line
string(REPLACE "\n" ";" NM_OUTPUT ${NM_OUTPUT})

set(mangling_ok true)

# define the regex to match the nm output
set(regex "[a-z0-9]+ T ([a-zA-Z0-9_]+)")

foreach( NM_SYMBOL ${NM_OUTPUT})
  if (NM_SYMBOL MATCHES ${regex})
    # extract the symbol part
    string (REGEX REPLACE "${regex}" "\\1" symbol "${NM_SYMBOL}" )
    # check that the symbol begins with "${OPJ_MANGLE_PREFIX}_"
    if (NOT ${symbol} MATCHES "^${OPJ_MANGLE_PREFIX}_")
      # two additional symbols _fini and _init are added for the shared libraries
      if (NOT ${symbol} MATCHES "(_fini|_init)")
        message("Found unmangled symbol : ${symbol}")
        # test fails as soon as there is one unmangled symbol
        set(mangling_ok false)
      endif()
    endif()
  endif()
endforeach()

if (NOT mangling_ok)
  # outputing a fatal error will make the test fail
  message(FATAL_ERROR "Not all symbols are properly mangled")
endif()
