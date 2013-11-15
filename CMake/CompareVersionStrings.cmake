# From  http://www.cmake.org/Wiki/CMakeCompareVersionStrings
# to be able to support cmake 2.6.0 (VERSION_LESS not available yet)
#
# Computes the realtionship between two version strings.  A version
# string is a number delineated by '.'s such as 1.3.2 and 0.99.9.1.
# You can feed version strings with different number of dot versions,
# and the shorter version number will be padded with zeros: 9.2 <
# 9.2.1 will actually compare 9.2.0 < 9.2.1.
#
# Input: a_in - value, not variable
#        b_in - value, not variable
#        result_out - variable with value:
#                         -1 : a_in <  b_in
#                          0 : a_in == b_in
#                          1 : a_in >  b_in
#
# Written by James Bigler.
macro(COMPARE_VERSION_STRINGS a_in b_in result_out)
  # Since SEPARATE_ARGUMENTS using ' ' as the separation token,
  # replace '.' with ' ' to allow easy tokenization of the string.
  string(REPLACE "." " " a ${a_in})
  string(REPLACE "." " " b ${b_in})
  separate_arguments(a)
  separate_arguments(b)

  # Check the size of each list to see if they are equal.
  list(LENGTH a a_length)
  list(LENGTH b b_length)

  # Pad the shorter list with zeros.

  # Note that range needs to be one less than the length as the for
  # loop is inclusive (silly CMake).
  if(a_length LESS b_length)
    # a is shorter
    set(shorter a)
    math(EXPR range "${b_length} - 1")
    math(EXPR pad_range "${b_length} - ${a_length} - 1")
  else()
    # b is shorter
    set(shorter b)
    math(EXPR range "${a_length} - 1")
    math(EXPR pad_range "${a_length} - ${b_length} - 1")
  endif()

  # PAD out if we need to
  if(NOT pad_range LESS 0)
    foreach(pad RANGE ${pad_range})
      # Since shorter is an alias for b, we need to get to it by by dereferencing shorter.
      list(APPEND ${shorter} 0)
    endforeach()
  endif()

  set(result 0)
  foreach(index RANGE ${range})
    if(result EQUAL 0)
      # Only continue to compare things as long as they are equal
      list(GET a ${index} a_version)
      list(GET b ${index} b_version)
      # LESS
      if(a_version LESS b_version)
        set(result -1)
      endif()
      # GREATER
      if(a_version GREATER b_version)
        set(result 1)
      endif()
    endif()
  endforeach()

  # Copy out the return result
  set(${result_out} ${result})
endmacro()
