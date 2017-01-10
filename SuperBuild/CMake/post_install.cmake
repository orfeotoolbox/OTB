string(REPLACE "|" ";" P_DIRS ${P_DIRS})

foreach( p_dir ${P_DIRS} )
set(file_list)
file( GLOB file_list "${p_dir}/*.cmake" )
foreach( cmake_file ${file_list} )
  file(STRINGS "${cmake_file}" MATCH_FOUND REGEX "${MATCH}")
  if(MATCH_FOUND)
    file(STRINGS "${cmake_file}" cmake_file_CONTENTS NEWLINE_CONSUME)
    string(REPLACE "${P_MATCH}" "$${}{${P_REPLACE}}" cmake_file_CONTENTS ${cmake_file_CONTENTS})
    file(WRITE "${cmake_file}"  "# This file is modified by OTB after installation.
      \n${cmake_file_CONTENTS}")
  endif()
endforeach() # foreach( cmake_file
endforeach() # foreach( p_dir
