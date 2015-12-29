find_package ( GDAL REQUIRED )

mark_as_advanced(GDAL_INCLUDE_DIR)
mark_as_advanced(GDAL_LIBRARY)
mark_as_advanced(GDAL_CONFIG)

if(NOT GDAL_FOUND)
 message(FATAL_ERROR "Cannot find GDAL. Set GDAL_INCLUDE_DIR and GDAL_LIBRARY")
endif()

#Check if GDAL is compatible with OTB using a bunch of cmake try_run and try_compile. (Default value is ON.)
set(GDAL_CONFIG_CHECKING ON CACHE BOOL "Tests to check gdal config." FORCE)
mark_as_advanced(GDAL_CONFIG_CHECKING)

if(GDAL_CONFIG_CHECKING)
	set(MIN_MAJOR_VERSION 1)
	set(MIN_MINOR_VERSION 10)

	set(GDAL_QUALIFIES TRUE)
	set(COMPILE_ERRORS FALSE)

	# Ensure that the temporary dir always exists before starting tests
	if(NOT EXISTS ${TEMP})
	  FILE(MAKE_DIRECTORY ${TEMP})
	endif()

	#------------------- TESTS ---------------------
	# Version of GDAL
	try_run(RUN_RESULT_VERSION COMPILE_RESULT_VERSION ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalVersionTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}" ARGS ${TEMP}/gdalVersion.txt ${MIN_MAJOR_VERSION} ${MIN_MINOR_VERSION})

	# Has OGR
	try_compile(GDAL_HAS_OGR ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalOGRTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}")


	# Has formats JPEG2000 & JPEG & GTIF
	# Note : exact format names can be found here http://www.gdal.org/formats_list.html
	try_run(GDAL_HAS_J2K_OPJG COMPILE_RESULT_FORMATS ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalFormatsTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}" ARGS JP2OpenJPEG ${TEMP}/gdalFormats.txt )
	try_run(GDAL_HAS_J2K_KAK COMPILE_RESULT_FORMATS ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalFormatsTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}" ARGS JP2KAK ) # No need to output ${TEMP}/gdalFormats.txt everytime
	try_run(GDAL_HAS_J2K_ECW COMPILE_RESULT_FORMATS ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalFormatsTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}" ARGS JP2ECW )
	try_run(GDAL_HAS_J2K_JG2000 COMPILE_RESULT_FORMATS ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalFormatsTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}" ARGS JPEG2000 )
	try_run(GDAL_HAS_JPEG COMPILE_RESULT_FORMATS ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalFormatsTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}" ARGS JPEG )
	try_run(GDAL_HAS_GTIF COMPILE_RESULT_FORMATS ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalFormatsTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}" ARGS GTiff )
	try_run(GDAL_HAS_HDF5 COMPILE_RESULT_FORMATS ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalFormatsTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}" ARGS HDF5 )

	# Can create geotiff file
	if (GDAL_HAS_GTIF)
		try_run(GDAL_CAN_CREATE_GEOTIFF COMPILE_RESULT_CREATE ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalCreateTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}" ARGS GTiff ${TEMP}/testImage.gtif )
	endif()

	#Can create other format file
	if(COMPILE_RESULT_CREATE AND GDAL_CAN_CREATE_GEOTIFF) #Use the result of the previous try_run

		# Can create bigtiff file
		if (GDAL_HAS_GTIF)
			try_run(GDAL_CAN_CREATE_BIGTIFF COMPILE_RESULT_CREATECOPY ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalCreateCopyTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}" ARGS ${TEMP}/testImage.gtif ${TEMP}/testImage.bigtif GTiff "BIGTIFF=YES")
		endif()

		# Can create jpeg file
		if (GDAL_HAS_JPEG)
			try_run(GDAL_CAN_CREATE_JPEG COMPILE_RESULT_CREATECOPY ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalCreateCopyTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}" ARGS ${TEMP}/testImage.gtif ${TEMP}/testImage.jpeg JPEG)
		endif()

		# Can create jpeg2000 file
		if (GDAL_HAS_J2K_OPJG)
			try_run(GDAL_CAN_CREATE_JPEG2000 COMPILE_RESULT_CREATECOPY ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalCreateCopyTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}" ARGS ${TEMP}/testImage.gtif ${TEMP}/testImage.j2k JP2OpenJPEG)
		endif()

		if (GDAL_HAS_J2K_KAK AND NOT GDAL_CAN_CREATE_JPEG2000)
			try_run(GDAL_CAN_CREATE_JPEG2000 COMPILE_RESULT_CREATECOPY ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalCreateCopyTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}" ARGS ${TEMP}/testImage.gtif ${TEMP}/testImage.j2k JP2KAK)
		endif()

		if (GDAL_HAS_J2K_ECW AND NOT GDAL_CAN_CREATE_JPEG2000)
			try_run(GDAL_CAN_CREATE_JPEG2000 COMPILE_RESULT_CREATECOPY ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalCreateCopyTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}" ARGS ${TEMP}/testImage.gtif ${TEMP}/testImage.j2k JP2ECW)
		endif()

		if (GDAL_HAS_J2K_JG2000 AND NOT GDAL_CAN_CREATE_JPEG2000)
			try_run(GDAL_CAN_CREATE_JPEG2000 COMPILE_RESULT_CREATECOPY ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalCreateCopyTest.cxx CMAKE_FLAGS "-DINCLUDE_DIRECTORIES:PATH=${GDAL_INCLUDE_DIR}" "-DLINK_LIBRARIES:STRING=${GDAL_LIBRARY}" ARGS ${TEMP}/testImage.gtif ${TEMP}/testImage.j2k JPEG2000)
		endif()

	endif()
	#------------------- TESTS (END)---------------------


	# Warning messages
	if (NOT COMPILE_RESULT_VERSION)
		message(WARNING "Modules/ThirdParty/GDAL/gdalVersionTest.cxx did not compile.")
		set(COMPILE_ERRORS TRUE)
	elseif (${RUN_RESULT_VERSION} EQUAL 1)
		file(READ "${TEMP}/gdalVersion.txt" DETECTED_VERSION)
		message(WARNING "Version of GDAL must be >= " ${MIN_MAJOR_VERSION} "." ${MIN_MINOR_VERSION} " : " ${DETECTED_VERSION} " detected.")
		set(GDAL_QUALIFIES FALSE)
  else((${RUN_RESULT_VERSION} EQUAL 1))
    file(READ "${TEMP}/gdalVersion.txt" DETECTED_VERSION)
    string(SUBSTRING ${DETECTED_VERSION} 0 2 VER2)
    if(${VER2} EQUAL "2.")
      message(STATUS "Gdal >= 2.0.0 detected")
      set(OTB_USE_GDAL_20 true CACHE INTERNAL "True if GDAL >= 2.0.0 has been detected" FORCE )
    else(${VER2} EQUAL "2.")
      set(OTB_USE_GDAL_20 false CACHE INTERNAL "True if GDAL >= 2.0.0 has been detected" FORCE )
    endif()
	endif()

	if (NOT GDAL_HAS_OGR)
		message(WARNING "GDAL doesn't expose OGR library symbols.")
		set(GDAL_QUALIFIES FALSE)
	endif()

	if (NOT COMPILE_RESULT_FORMATS)
		message(WARNING "Modules/ThirdParty/GDAL/gdalFormatsTest.cxx did not compile.")
		set(COMPILE_ERRORS TRUE)
	else()

		if (NOT GDAL_HAS_J2K_JG2000 AND NOT GDAL_HAS_J2K_OPJG AND NOT GDAL_HAS_J2K_KAK AND NOT GDAL_HAS_J2K_ECW)
		message(STATUS "No Jpeg2000 driver found (compatible drivers are : OpenJpeg, Kakadu, ECW).")
		#set(GDAL_QUALIFIES FALSE)
		endif()

		if (NOT GDAL_HAS_JPEG)
		message(WARNING "No jpeg driver found.")
		set(GDAL_QUALIFIES FALSE)
		endif()

		if (NOT GDAL_HAS_GTIF)
		message(WARNING "No geotiff driver found.")
		set(GDAL_QUALIFIES FALSE)
		endif()

	endif()

  if (NOT GDAL_HAS_HDF5)
    message(STATUS "GDAL doesn't have HDF5 drivers. (HDF5 tests will be deactivated)")
  endif()

	if (NOT COMPILE_RESULT_CREATE)
		message(WARNING "Modules/ThirdParty/GDAL/gdalCreateTest.cxx did not compile.")
		set(COMPILE_ERRORS TRUE)
	elseif (NOT GDAL_CAN_CREATE_GEOTIFF)
			message(WARNING "GDAL can't create geotiff files.")
			set(GDAL_QUALIFIES FALSE)
	endif()


	if (NOT COMPILE_RESULT_CREATECOPY)
		message(WARNING "Modules/ThirdParty/GDAL/gdalCreateCopyTest.cxx did not compile.")
		set(COMPILE_ERRORS TRUE)

		if (NOT GDAL_CAN_CREATE_BIGTIFF)
			message(WARNING "No BIGTIFF capatilities.")
			set(GDAL_QUALIFIES FALSE)
		endif()

		if (NOT GDAL_CAN_CREATE_JPEG)
			message(WARNING "GDAL can't create jpeg files.")
			set(GDAL_QUALIFIES FALSE)
		endif()

		if (NOT GDAL_CAN_CREATE_JPEG2000)
			message(WARNING "GDAL can't create jpeg2000 files.")
			set(GDAL_QUALIFIES FALSE)
		endif()

	endif()




	#FOR UNIX SYSTEMS ONLY
	if(UNIX)

	  if(GDAL_QUALIFIES)

			# Prepare bash script
			configure_file(${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalTest.sh.in ${CMAKE_CURRENT_BINARY_DIR}/gdalTest.sh @ONLY)
			execute_process(COMMAND chmod u+x ${CMAKE_CURRENT_BINARY_DIR}/gdalTest.sh)
			execute_process(COMMAND ${CMAKE_CURRENT_BINARY_DIR}/gdalTest.sh)


			#------------------- TESTS ---------------------
			# test libtiff/libgeotiff and test if symbols are renamed (only for internal libtiff/libgeotiff)
			try_run(RUN_RESULT_SYMBOLS COMPILE_RESULT_SYMBOLS ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}/Modules/ThirdParty/GDAL/gdalSymbolsTest.cxx ARGS ${TEMP}/gdalSymbols.txt)
			#------------------- TESTS (END)---------------------


			# Warning messages
			if (NOT COMPILE_RESULT_SYMBOLS)
				message(WARNING "Modules/ThirdParty/GDAL/gdalSymbolsTest.cxx did not compile.")
				set(COMPILE_ERRORS TRUE)
			elseif (${RUN_RESULT_SYMBOLS} EQUAL 1)
				message(WARNING "Internal versions of libtiff/libgeotiff detected without symbol renaming (when configuring GDAL, if options --with-libtiff or --with-geotiff are set to 'internal', then options --with-rename-internal-libtiff-symbols and --with-rename-internal-libgeotiff-symbols should be set to 'yes').")
				set(GDAL_QUALIFIES FALSE)
			endif()


		endif() #GDAL_QUALIFIES


	endif() #UNIX


	if(GDAL_QUALIFIES AND NOT COMPILE_ERRORS)
          message(STATUS "Check if Gdal qualifies for Orfeo ToolBox -- yes.")
	else()
          message(STATUS "Check if Gdal qualifies for Orfeo ToolBox -- no.")
	endif()

  if(NOT GDAL_VERSION)
    if(EXISTS "${TEMP}/gdalVersion.txt")
      file(READ "${TEMP}/gdalVersion.txt" GDAL_VERSION)
    endif()
  endif()
  message(STATUS "  Version : ${GDAL_VERSION}")
  message(STATUS "  Drivers for JPEG 2000 : ")
  if(GDAL_HAS_J2K_JG2000)
    message(STATUS "    Jasper (will not be used)")
  endif()
  if(GDAL_HAS_J2K_OPJG)
    message(STATUS "    OpenJPEG")
  endif()
  if(GDAL_HAS_J2K_KAK)
    message(STATUS "    Kakadu")
  endif()
  if(GDAL_HAS_J2K_ECW)
    message(STATUS "    ECW")
  endif()

endif() #GDAL_CONFIG_CHECKING
