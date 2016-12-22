set(file_list)
file(GLOB file_list "${PATCH_DIR}/*.cmake")
foreach(cmake_file ${file_list})
  file(STRINGS "${cmake_file}" MATCH_FOUND REGEX "${MATCH}")
  if(MATCH_FOUND)
    file(STRINGS "${cmake_file}" cmake_file_CONTENTS NEWLINE_CONSUME)
    set(CODE_SNIPPET "get_filename_component(DEPS_INSTALL_DIR \"$${}{CMAKE_CURRENT_LIST_FILE}\" PATH)")
    if (PATH_STRIP_COUNT LESS 1)
      set(PATH_STRIP_COUNT 2)
    endif()
    foreach(loop_var RANGE 1 ${PATH_STRIP_COUNT})
      set(CODE_SNIPPET "${CODE_SNIPPET} \nget_filename_component(DEPS_INSTALL_DIR \"$${}{DEPS_INSTALL_DIR}\" PATH)" )
    endforeach()
    string(REPLACE "${MATCH}" "$${}{DEPS_INSTALL_DIR}" cmake_file_CONTENTS_NEW ${cmake_file_CONTENTS})
    file(WRITE "${cmake_file}" 
      "# This file is modified by OTB after installation.\n"
      "# For example, see POST_INSTALL_* step in OTB/SuperBuild/CMake/External_itk.cmake \n"
      "# BEGIN CODE BLOCK FROM OTB SUPERBUILD\n"
      "${CODE_SNIPPET}\n"
      "# END CODE BLOCK FROM OTB SUPERBUILD\n"
      "${cmake_file_CONTENTS_NEW}\n")
  endif()
endforeach() #foreach(cmake_file
