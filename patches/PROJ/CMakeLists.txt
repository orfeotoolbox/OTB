cmake_minimum_required(VERSION 2.8.3)

project(proj_install)

file(GLOB ${PROJECT_NAME}_HEADERS "${PROJ_INCLUDE_DIR}/projects.h" "${PROJ_INCLUDE_DIR}/proj_api.h" "${PROJ_INCLUDE_DIR}/org_proj4_Projections.h" )
file(GLOB ${PROJECT_NAME}_LIBS "${PROJ_LIB_DIR}/*.lib" )
file(GLOB ${PROJECT_NAME}_DLLS "${PROJ_LIB_DIR}/*.dll" )

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