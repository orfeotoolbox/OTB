find_package ( GDAL REQUIRED )

mark_as_advanced(GDAL_INCLUDE_DIR)
mark_as_advanced(GDAL_LIBRARY)
mark_as_advanced(GDAL_CONFIG)


if(NOT GDAL_FOUND)
 message(FATAL_ERROR "Cannot find GDAL. Set GDAL_INCLUDE_DIR and GDAL_LIBRARY")
endif()

if(UNIX)

	message(STATUS "Check if Gdal qualifies for Orfeo ToolBox")
  
  if("x${GDAL_CONFIG}" STREQUAL "x")
    message(FATAL_ERROR "Cannot find gdal-config executable. Set GDAL_CONFIG")
  endif()

	# Prepare bash script
	configure_file(${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalTest.sh.in ${CMAKE_CURRENT_BINARY_DIR}/gdalTest.sh @ONLY)
	execute_process(COMMAND chmod u+x ${CMAKE_CURRENT_BINARY_DIR}/gdalTest.sh)
	execute_process(COMMAND ${CMAKE_CURRENT_BINARY_DIR}/gdalTest.sh)


	#------------------- TESTS ---------------------
	# Version of GDAL  
	try_run(RUN_RESULT_VAR1 COMPILE_RESULT_VAR1 ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalTest1.cxx ARGS ${TEMP}/testgdal1.txt)

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

	# Has OGR
	try_compile(GDAL_HAS_OGR ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalOGRTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}")

	# Can open tiff file
	try_run(GDAL_CAN_OPEN_TIFF COMPILE_RESULT_OPEN ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalOpenTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}" ARGS ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/otb_logo.tif)

	# Can open jpeg file
	try_run(GDAL_CAN_OPEN_JPEG COMPILE_RESULT_OPEN ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalOpenTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}" ARGS ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/otb_logo.jpeg)

	# Can open jpeg2000 file
	try_run(GDAL_CAN_OPEN_JPEG2000 COMPILE_RESULT_OPEN ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalOpenTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}" ARGS ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/QB_multicomp_small.jpx)

	# Can open geotiff file
	#try_run(GDAL_CAN_OPEN_GEOTIFF COMPILE_RESULT_OPEN ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalOpenTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}" ARGS ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/GEOTIFF)

	#------------------- TESTS (END)---------------------


	# Compilations checking
	if (NOT COMPILE_RESULT_VAR1)
		message(WARNING "Modules/ThirdParty/GDAL/gdalTest1.cxx did not compile.")
	endif()
	if (NOT COMPILE_RESULT_VAR2)
		message(WARNING "Modules/ThirdParty/GDAL/gdalTest2.cxx did not compile.")
	endif()
	if (NOT COMPILE_RESULT_VAR3)
		message(WARNING "Modules/ThirdParty/GDAL/gdalTest3.cxx did not compile.")
	endif()
	if (NOT COMPILE_RESULT_VAR4)
		message(WARNING "Modules/ThirdParty/GDAL/gdalTest4.cxx did not compile.")
	endif()
	if (NOT COMPILE_RESULT_OPEN)
		message(WARNING "Modules/ThirdParty/GDAL/gdalOpenTest.cxx did not compile.")
	endif()

	# Warning messages
	if (${RUN_RESULT_VAR1} EQUAL 1)
		message(WARNING "Version of GDAL must be >= 1.10")
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
	if (NOT GDAL_HAS_OGR)
		message(WARNING "GDAL doesn't expose OGR library symbols.")
	endif()
	if (NOT GDAL_CAN_OPEN_TIFF)
		message(WARNING "GDAL can't open tiff files.")
	endif()
	if (NOT GDAL_CAN_OPEN_JPEG)
		message(WARNING "GDAL can't open jpeg files.")
	endif()
	if (NOT GDAL_CAN_OPEN_JPEG2000)
		message(WARNING "GDAL can't open jpeg files.")
	endif()
	#if (NOT GDAL_CAN_OPEN_GEOTIFF)
	#	message(WARNING "GDAL can't open geotiff files.")
	#endif()

endif() #UNIX

