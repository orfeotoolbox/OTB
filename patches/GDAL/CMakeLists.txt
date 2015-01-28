cmake_minimum_required(VERSION 2.8.3)

project(gdal_install)

file(GLOB GDAL_APPS "apps/*.exe")
foreach(GDAL_APP ${GDAL_APPS})
install(FILES ${GDAL_APP} DESTINATION bin/gdal/apps  COMPONENT Development)
endforeach()
        
file(GLOB GDAL_DATAS "data/*.*")
foreach(GDAL_DATA ${GDAL_DATAS})
install(FILES ${GDAL_DATA} DESTINATION bin/gdal-data COMPONENT Development)
endforeach()
    
   
	# if exist $(OUTPUT_DIR)\bin\gdal\plugins del $(OUTPUT_DIR)\bin\gdal\plugins\*.dll
    # if exist $(OUTPUT_DIR)\bin\gdal\plugins-optional del $(OUTPUT_DIR)\bin\gdal\plugins-optional\*.dll
	# if exist $(OUTPUT_DIR)\bin\gdal\plugins-external del $(OUTPUT_DIR)\bin\gdal\plugins-external\*.dll

file(GLOB ${PROJECT_NAME}_HEADERS 
"${GDAL_INCLUDE_DIR}/port/*.h" 
"${GDAL_INCLUDE_DIR}/gcore/*.h"
"${GDAL_INCLUDE_DIR}/alg/*.h"
"${GDAL_INCLUDE_DIR}/ogr/*.h"
"${GDAL_INCLUDE_DIR}/frmts/mem/memdataset.h"
"${GDAL_INCLUDE_DIR}/frmts/raw/rawdataset.h"
"${GDAL_INCLUDE_DIR}/frmts/vrt/*.h"
"${GDAL_INCLUDE_DIR}/ogr/ogrsf_frmts/*.h")

file(GLOB ${PROJECT_NAME}_LIBS  "${GDAL_LIB_DIR}/gdal_i.lib" )

file(GLOB ${PROJECT_NAME}_DLLS "${GDAL_LIB_DIR}/*.dll" )

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