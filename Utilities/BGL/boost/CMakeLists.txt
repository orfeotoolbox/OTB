
IF(NOT OTB_INSTALL_NO_DEVELOPMENT)

set(BOOST_FOLDER_LIST "bfunctional;bind;butility;config;detail;function;graph;iterator;mpl;optional;pending;preprocessor;spirit;tuple;type_traits")
  
  FILE(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  FILE(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")
  FILE(GLOB __files3 "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
  INSTALL(FILES ${__files1} ${__files2} ${__files3}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/Utilities/BGL/boost
    COMPONENT Development)
    
  foreach(folder ${BOOST_FOLDER_LIST})
    FILE(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/${folder}/*.h")
    FILE(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/${folder}/*.txx")
    FILE(GLOB __files3 "${CMAKE_CURRENT_SOURCE_DIR}/${folder}/*.hpp")
    INSTALL(FILES ${__files1} ${__files2} ${__files3}
      DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/Utilities/BGL/boost/${folder}
      COMPONENT Development)
  endforeach(folder)    
ENDIF(NOT OTB_INSTALL_NO_DEVELOPMENT)
