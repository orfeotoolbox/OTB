find_package ( GDAL REQUIRED )

mark_as_advanced(GDAL_INCLUDE_DIR)
mark_as_advanced(GDAL_LIBRARY)
mark_as_advanced(GDAL_CONFIG)


if(NOT GDAL_FOUND)
 message(FATAL_ERROR "Cannot find GDAL. Set GDAL_INCLUDE_DIR and GDAL_LIBRARY")
endif()

message(STATUS "Check if Gdal qualifies for Orfeo ToolBox")

#------------------- TESTS ---------------------
# Version of GDAL  
try_run(RUN_RESULT_VERSION COMPILE_RESULT_VERSION ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalVersionTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}" ARGS ${TEMP}/gdalVersion.txt)

# Has OGR
try_compile(GDAL_HAS_OGR ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalOGRTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}")

# Can create geotiff file
try_run(GDAL_CAN_CREATE_GEOTIFF COMPILE_RESULT_CREATE ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalCreateTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}" ARGS GTiff ${TEMP}/testImage.gtif )
if(GDAL_CAN_CREATE_GEOTIFF)

	# Can create jpeg file
	try_run(GDAL_CAN_CREATE_JPEG COMPILE_RESULT_CREATECOPY ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalCreateCopyTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}" ARGS ${TEMP}/testImage.gtif ${TEMP}/testImage.jpeg JPEG)
	# Can create jpeg2000 file
	try_run(GDAL_CAN_CREATE_JPEG2000 COMPILE_RESULT_CREATECOPY ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalCreateCopyTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}" ARGS ${TEMP}/testImage.gtif ${TEMP}/testImage.j2k JPEG2000)

endif()
#------------------- TESTS (END)---------------------


# Warning messages
if (NOT COMPILE_RESULT_VERSION)
	message(WARNING "Modules/ThirdParty/GDAL/gdalVersionTest.cxx did not compile.")
endif()

if (${RUN_RESULT_VERSION} EQUAL 1)
	message(WARNING "Version of GDAL must be >= 1.10")
endif()
	
if (NOT GDAL_HAS_OGR)
	message(WARNING "GDAL doesn't expose OGR library symbols.")
endif()

if (NOT COMPILE_RESULT_CREATE)
	message(WARNING "Modules/ThirdParty/GDAL/gdalCreateTest.cxx did not compile.")
endif()
if (NOT COMPILE_RESULT_CREATECOPY)
	message(WARNING "Modules/ThirdParty/GDAL/gdalCreateCopyTest.cxx did not compile.")
endif()
if (NOT GDAL_CAN_CREATE_GEOTIFF)
	message(WARNING "GDAL can't create tiff files.")
endif()
if (NOT GDAL_CAN_CREATE_JPEG)
	message(WARNING "GDAL can't create jpeg files.")
endif()
if (NOT GDAL_CAN_CREATE_JPEG2000)
	message(WARNING "GDAL can't create jpeg2000 files.")
endif()




#FOR UNIX SYSTEMS ONLY
if(UNIX)
  
  if(GDAL_CAN_CREATE_GEOTIFF)
  
	  if("x${GDAL_CONFIG}" STREQUAL "x")
		message(FATAL_ERROR "Cannot find gdal-config executable. Set GDAL_CONFIG")
	  endif()

		# Prepare bash script
		configure_file(${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalTest.sh.in ${CMAKE_CURRENT_BINARY_DIR}/gdalTest.sh @ONLY)
		execute_process(COMMAND chmod u+x ${CMAKE_CURRENT_BINARY_DIR}/gdalTest.sh)
		execute_process(COMMAND ${CMAKE_CURRENT_BINARY_DIR}/gdalTest.sh)


		#------------------- TESTS ---------------------
		# test libtiff/libgeotiff and test if symbols are renamed (only for internal libtiff/libgeotiff)
		try_run(RUN_RESULT_VAR2 COMPILE_RESULT_VAR2 ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalTest2.cxx ARGS ${TEMP}/testgdal2.txt)

		# bigtiff
		try_run(RUN_RESULT_VAR3 COMPILE_RESULT_VAR3 ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalTest3.cxx ARGS ${TEMP}/testgdal3.txt)

		# formats
		try_run(GDAL_HAS_HDF COMPILE_RESULT_VAR4 ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalTest4.cxx ARGS ${TEMP}/testgdal4.txt hdf)
		try_run(GDAL_HAS_J2K_JG2000 COMPILE_RESULT_VAR4 ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalTest4.cxx ARGS ${TEMP}/testgdal4.txt jpeg2000)
		try_run(GDAL_HAS_J2K_OPJG COMPILE_RESULT_VAR4 ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalTest4.cxx ARGS ${TEMP}/testgdal4.txt openjpeg)
		try_run(GDAL_HAS_J2K_KAK  COMPILE_RESULT_VAR4 ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalTest4.cxx ARGS ${TEMP}/testgdal4.txt jp2kak)
		try_run(GDAL_HAS_J2K_ECW COMPILE_RESULT_VAR4 ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalTest4.cxx ARGS ${TEMP}/testgdal4.txt ecw)

		#------------------- TESTS (END)---------------------


		# Warning messages
		if (NOT COMPILE_RESULT_VAR2)
			message(WARNING "Modules/ThirdParty/GDAL/gdalTest2.cxx did not compile.")
		endif()
		if (NOT COMPILE_RESULT_VAR3)
			message(WARNING "Modules/ThirdParty/GDAL/gdalTest3.cxx did not compile.")
		endif()
		if (NOT COMPILE_RESULT_VAR4)
			message(WARNING "Modules/ThirdParty/GDAL/gdalTest4.cxx did not compile.")
		endif()


		if (${RUN_RESULT_VAR2} EQUAL 1)
			message(WARNING "Internal versions of libtiff/libgeotiff detected without symbol renaming (when configuring GDAL, if options --with-libtiff or --with-geotiff are set to 'internal', then options --with-rename-internal-libtiff-symbols and --with-rename-internal-libgeotiff-symbols should be set to 'yes').")
		endif()
		if (${RUN_RESULT_VAR3} EQUAL 1)
			message(WARNING "No BIGTIFF capatilities.")
		endif()
		if (NOT GDAL_HAS_HDF)
			message(WARNING "No HDF capatilities.")
		endif()
		if (NOT GDAL_HAS_J2K_JG2000 AND NOT GDAL_HAS_J2K_OPJG AND NOT GDAL_HAS_J2K_KAK AND NOT GDAL_HAS_J2K_ECW)
			message(WARNING "No Jpeg2000 driver found (compatible drivers are : OpenJpeg, Kakadu, ECW).")
		endif()
		
	endif() #GDAL_CAN_CREATE_GEOTIFF


endif() #UNIX
