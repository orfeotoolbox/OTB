cmake_minimum_required(VERSION 2.8.3)

project(tiff_install)

file(GLOB ${PROJECT_NAME}_HEADERS "${TIFF_BUILD_DIR}/tif*.h" )
file(GLOB ${PROJECT_NAME}_LIBS "${TIFF_BUILD_DIR}/libtiff*.lib" )
file(GLOB ${PROJECT_NAME}_DLLS "${TIFF_BUILD_DIR}/libtiff*.dll" )

foreach(${PROJECT_NAME}_HEADER ${${PROJECT_NAME}_HEADERS})
install(FILES ${${PROJECT_NAME}_HEADER}
  DESTINATION include
  COMPONENT Development)
endforeach()
  
foreach(${PROJECT_NAME}_LIB ${${PROJECT_NAME}_LIBS})
install(FILES ${${PROJECT_NAME}_LIB}
  DESTINATION lib
  COMPONENT Development)
endforeach()

foreach(${PROJECT_NAME}_DLL ${${PROJECT_NAME}_DLLS})
install(FILES ${${PROJECT_NAME}_DLL}
  DESTINATION bin
  COMPONENT Development)
endforeach()