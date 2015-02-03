cmake_minimum_required(VERSION 2.8.3)

project(gdal_install)

file(GLOB GDAL_APPS "${GDAL_BUILD_DIR}/apps/*.exe")
foreach(GDAL_APP ${GDAL_APPS})
install(FILES ${GDAL_APP} DESTINATION bin/gdal/apps  COMPONENT Development)
endforeach()
        
file(GLOB GDAL_DATAS "${GDAL_BUILD_DIR}/data/*.*")
message(STATUS ${GDAL_DATAS})
foreach(GDAL_DATA ${GDAL_DATAS})
install(FILES ${GDAL_DATA} DESTINATION bin/gdal-data COMPONENT Development)
endforeach()
    
   
	# if exist $(OUTPUT_DIR)\bin\gdal\plugins del $(OUTPUT_DIR)\bin\gdal\plugins\*.dll
    # if exist $(OUTPUT_DIR)\bin\gdal\plugins-optional del $(OUTPUT_DIR)\bin\gdal\plugins-optional\*.dll
	# if exist $(OUTPUT_DIR)\bin\gdal\plugins-external del $(OUTPUT_DIR)\bin\gdal\plugins-external\*.dll

file(GLOB ${PROJECT_NAME}_HEADERS 
"${GDAL_BUILD_DIR}/port/*.h" 
"${GDAL_BUILD_DIR}/gcore/*.h"
"${GDAL_BUILD_DIR}/alg/*.h"
"${GDAL_BUILD_DIR}/ogr/*.h"
"${GDAL_BUILD_DIR}/frmts/mem/memdataset.h"
"${GDAL_BUILD_DIR}/frmts/raw/rawdataset.h"
"${GDAL_BUILD_DIR}/frmts/vrt/*.h"
"${GDAL_BUILD_DIR}/ogr/ogrsf_frmts/*.h")

file(GLOB ${PROJECT_NAME}_LIBS  "${GDAL_BUILD_DIR}/*.lib" )

file(GLOB ${PROJECT_NAME}_DLLS "${GDAL_BUILD_DIR}/*.dll" )

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