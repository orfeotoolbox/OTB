cmake_minimum_required(VERSION 2.8.3)

project(jpeg_install)


file(GLOB ${PROJECT_NAME}_HEADERS 
           "${JPEG_BUILD_DIR}/jconfig.h" 
           "${JPEG_BUILD_DIR}/jerror.h"
           "${JPEG_BUILD_DIR}/jmorecfg.h"
           "${JPEG_BUILD_DIR}/jpegint.h"
           "${JPEG_BUILD_DIR}/jpeglib.h" )

#libjpeg is static
file(GLOB ${PROJECT_NAME}_LIBS "${JPEG_BUILD_DIR}/*.lib" )
#file(GLOB ${PROJECT_NAME}_DLLS "${JPEG_INCLUDE_DIR}/jpeg*.dll" )


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

# foreach(${PROJECT_NAME}_DLL ${${PROJECT_NAME}_DLLS})
 # install(FILES ${${PROJECT_NAME}_DLL}
  # DESTINATION bin
  # COMPONENT Development)
# endforeach()