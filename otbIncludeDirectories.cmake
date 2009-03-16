# $Id$
#-----------------------------------------------------------------------------
# Include directories for other projects installed on the system.
SET(OTB_INCLUDE_DIRS_SYSTEM "")

#-----------------------------------------------------------------------------
# Include directories from the build tree.
SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_BINARY_DIR})

# These directories are always needed.
SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
  ${OTB_SOURCE_DIR}/Code/Common
  ${OTB_SOURCE_DIR}/Code/BasicFilters
  ${OTB_SOURCE_DIR}/Code/IO
  ${OTB_SOURCE_DIR}/Code/ChangeDetection
  ${OTB_SOURCE_DIR}/Code/FeatureExtraction
  ${OTB_SOURCE_DIR}/Code/Learning
  ${OTB_SOURCE_DIR}/Code/MultiScale
  ${OTB_SOURCE_DIR}/Code/SpatialReasoning
  ${OTB_SOURCE_DIR}/Code/DisparityMap
  ${OTB_SOURCE_DIR}/Code/Visu
  ${OTB_SOURCE_DIR}/Code/Visualization
  ${OTB_SOURCE_DIR}/Code/Gui
  ${OTB_SOURCE_DIR}/Code/Fusion
  ${OTB_SOURCE_DIR}/Code/Projections
  ${OTB_SOURCE_DIR}/Code/Radiometry
  ${OTB_SOURCE_DIR}/Code/SARPolarimetry
  ${OTB_SOURCE_DIR}/Code/Markov
  ${OTB_SOURCE_DIR}/Utilities/BGL
  ${OTB_SOURCE_DIR}/Utilities/BGL/boost
  ${OTB_SOURCE_DIR}/Utilities/otbsvm
  ${OTB_SOURCE_DIR}/Utilities/otbossim
  ${OTB_SOURCE_DIR}/Utilities/otbossim/include
  ${OTB_SOURCE_DIR}/Utilities/otbossim/include/ossim
  ${OTB_SOURCE_DIR}/Utilities/otbossimplugins
  ${OTB_SOURCE_DIR}/Utilities/dxflib
  ${OTB_SOURCE_DIR}/Utilities/InsightJournal
  ${OTB_SOURCE_DIR}/Utilities/otb6S
  ${OTB_SOURCE_DIR}/Utilities/tinyXMLlib
#  ${OTB_SOURCE_DIR}/Utilities/otbgalib
  ${OTB_SOURCE_DIR}/Utilities/otbkml
  ${OTB_SOURCE_DIR}/Utilities/otbkml/src
  ${OTB_SOURCE_DIR}/Utilities/otbkml/third_party
  ${OTB_SOURCE_DIR}/Utilities/otbkml/third_party/boost_1_34_1
#  ${OTB_SOURCE_DIR}/Utilities/otbkml/third_party/zlib-1.2.3
#  ${OTB_SOURCE_DIR}/Utilities/otbkml/third_party/zlib-1.2.3/contrib
  ${OTB_SOURCE_DIR}/Utilities/otbliblas/include
  ${OTB_SOURCE_DIR}/Utilities/otbedison/segm
  ${OTB_SOURCE_DIR}/Utilities/otbedison/prompt
  ${OTB_SOURCE_DIR}/Utilities/otbedison/edge
  ${OTB_SOURCE_DIR}/Utilities/otbsiftfast		

)
IF(OTB_COMPILE_JPEG2000)
  SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
        ${OTB_SOURCE_DIR}/Utilities/otbopenjpeg
        ${OTB_SOURCE_DIR}/Utilities/otbopenjpeg/libopenjpeg)
ENDIF(OTB_COMPILE_JPEG2000)

#-----------------------------------------------------------------------------
# Include directories from the ossim build tree
SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
        ${OTB_BINARY_DIR}/Utilities/otbossim/include
)


#-----------------------------------------------------------------------------
# Include directories from the GDAL build tree.
SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
  	${GDAL_INCLUDE_DIRS} )
#-----------------------------------------------------------------------------
# Include directories from the CURL build tree.
IF(OTB_USE_CURL)
  SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
  	${CURL_INCLUDE_DIR} )
ENDIF(OTB_USE_CURL)

#-----------------------------------------------------------------------------
# Include directories from the ITK build tree.
IF(OTB_USE_EXTERNAL_ITK)
#        INCLUDE(${ITK_SOURCE_DIR}/Utilities/itkThirdParty.cmake)
# 	INCLUDE(${ITK_SOURCE_DIR}/itkIncludeDirectories.cmake)
#	SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
#	${ITK_SOURCE_DIR}/Utilities/vxl/v3p/netlib
# 	${ITK_SOURCE_DIR}/Utilities/vxl/v3p/netlib/opt)
	SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
   			 ${ITK_INCLUDE_DIRS} )
ELSE(OTB_USE_EXTERNAL_ITK)
	IF(NOT ITK_INSTALL_INCLUDE_DIR)
  		SET(ITK_INSTALL_INCLUDE_DIR "/include/otb/Utilities/ITK")
	ENDIF(NOT ITK_INSTALL_INCLUDE_DIR)
        INCLUDE(${OTB_SOURCE_DIR}/Utilities/ITK/Utilities/itkThirdParty.cmake)
 	INCLUDE(${OTB_SOURCE_DIR}/Utilities/ITK/itkIncludeDirectories.cmake)
	SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
# 2008/12/18: Added ITK include dir
        ${ITK_INCLUDE_DIRS_BUILD_TREE}
        ${ITK_INCLUDE_DIRS_BUILD_TREE_CXX}
        ${ITK_INCLUDE_DIRS_SYSTEM}
	${OTB_SOURCE_DIR}/Utilities/ITK/Utilities/vxl/v3p/netlib
	${OTB_SOURCE_DIR}/Utilities/ITK/Utilities/vxl/v3p/netlib/opt)
ENDIF(OTB_USE_EXTERNAL_ITK)


#-----------------------------------------------------------------------------
# Include directories needed for .cxx files in OTB.  These include
# directories will NOT be available to user projects.
SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX)

SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX} )

#-----------------------------------------------------------------------------
# Include FLTK variables.
#IF(NOT OTB_USE_EXTERNAL_FLTK)
#        IF(NOT EXISTS "${FLTK_BINARY_DIR}/FLTKConfig.cmake")
#                MESSAGE(FATAL_ERROR "Impossible to find the file ${FLTK_BINARY_DIR}/FLTKConfig.cmake. You must generate FLTK with cmake process.")
#        ELSE(NOT EXISTS "${FLTK_BINARY_DIR}/FLTKConfig.cmake")
#                INCLUDE(${FLTK_BINARY_DIR}/FLTKConfig.cmake)
#        ENDIF(NOT EXISTS "${FLTK_BINARY_DIR}/FLTKConfig.cmake")
#ELSE(OTB_USE_EXTERNAL_FLTK)
#        IF(EXISTS "${OTB_BINARY_DIR}/Utilities/FLTK/FLTKConfig.cmake")
#                INCLUDE(${OTB_BINARY_DIR}/Utilities/FLTK/FLTKConfig.cmake)
#        ENDIF(EXISTS "${OTB_BINARY_DIR}/Utilities/FLTK/FLTKConfig.cmake")
#ENDIF(NOT OTB_USE_EXTERNAL_FLTK)


#-----------------------------------------------------------------------------
# Include directories from libraries build tree.


#For FLTK header file
IF(OTB_USE_EXTERNAL_FLTK)
        SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
  		${FLTK_INCLUDE_DIRS} )
ELSE(OTB_USE_EXTERNAL_FLTK)
        SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
                ${OTB_SOURCE_DIR}/Utilities/FLTK
  	        ${OTB_SOURCE_DIR}/Utilities/FLTK/src
  	        ${OTB_BINARY_DIR}/Utilities/FLTK )
ENDIF(OTB_USE_EXTERNAL_FLTK)

#For GLU header file
SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
  		${OTB_GLU_INCLUDE_PATH} )

#For GDAL header file
SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
  		${GDAL_INCLUDE_DIRS} )

#For EXPAT header file
IF(OTB_USE_EXTERNAL_EXPAT)
        SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
  	        ${EXPAT_INCLUDE_DIR} )
ELSE(OTB_USE_EXTERNAL_EXPAT)
        SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
                ${OTB_SOURCE_DIR}/Utilities/otbexpat)
ENDIF(OTB_USE_EXTERNAL_EXPAT)

# For OpenThreads
IF(NOT OTB_USE_EXTERNAL_OPENTHREADS)
        SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
                ${OTB_SOURCE_DIR}/Utilities/otbopenthreads/OpenThreads/include
	)
ENDIF(NOT OTB_USE_EXTERNAL_OPENTHREADS)

#For ITK header file
IF(OTB_USE_EXTERNAL_ITK)
	SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
   			${ITK_INCLUDE_DIRS} )
#   			${ITK_INCLUDE_DIRS_INSTALL_TREE} 	)
ELSE(OTB_USE_EXTERNAL_ITK)
	SET(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
   			${ITK_INCLUDE_DIRS_BUILD_TREE}
   			${ITK_INCLUDE_DIRS_BUILD_TREE_CXX} 	)
ENDIF(OTB_USE_EXTERNAL_ITK)

#-----------------------------------------------------------------------------
# Include directories from the install tree.
SET(OTB_INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib/otb")
SET(OTB_INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin")
SET(OTB_INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include/otb")
SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
  ${OTB_INSTALL_INCLUDE_DIR}
  ${OTB_INSTALL_INCLUDE_DIR}/Common
  ${OTB_INSTALL_INCLUDE_DIR}/BasicFilters
  ${OTB_INSTALL_INCLUDE_DIR}/IO
  ${OTB_INSTALL_INCLUDE_DIR}/ChangeDetection
  ${OTB_INSTALL_INCLUDE_DIR}/FeatureExtraction
  ${OTB_INSTALL_INCLUDE_DIR}/Learning
  ${OTB_INSTALL_INCLUDE_DIR}/MultiScale
  ${OTB_INSTALL_INCLUDE_DIR}/SpatialReasoning
  ${OTB_INSTALL_INCLUDE_DIR}/DisparityMap
  ${OTB_INSTALL_INCLUDE_DIR}/Visu
  ${OTB_INSTALL_INCLUDE_DIR}/Visualization
  ${OTB_INSTALL_INCLUDE_DIR}/Gui
  ${OTB_INSTALL_INCLUDE_DIR}/Projections
  ${OTB_INSTALL_INCLUDE_DIR}/Radiometry
  ${OTB_INSTALL_INCLUDE_DIR}/SARPolarimetry
  ${OTB_INSTALL_INCLUDE_DIR}/Markov
  ${OTB_INSTALL_INCLUDE_DIR}/Fusion
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/ITK
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/BGL
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/BGL/boost
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbsvm
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbossim
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbossim/include
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbossim/include/ossim
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbossimplugins
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/InsightJournal
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otb6S
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/tinyXMLlib
#  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbgalib
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbkml
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbkml/src
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbkml/third_party
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbkml/third_party/boost_1_34_1
# ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbkml/third_party/zlib-1.2.3
# ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbkml/third_party/zlib-1.2.3/contrib
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbliblas/include
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbedison
  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbsiftfast
)

IF(OTB_COMPILE_JPEG2000)
  SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
        ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbopenjpeg
        ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbopenjpeg/libopenjpeg)
ENDIF(OTB_COMPILE_JPEG2000)

#For GDAL header file
SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
  		${GDAL_INCLUDE_DIRS} )

#For EXPAT header file
IF(OTB_USE_EXTERNAL_EXPAT)
        SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
  	        ${EXPAT_INCLUDE_DIR} )
ELSE(OTB_USE_EXTERNAL_EXPAT)
        SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
                ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbexpat)
ENDIF(OTB_USE_EXTERNAL_EXPAT)

#For CURL header file
IF(OTB_USE_CURL)
  SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
  	${CURL_INCLUDE_DIR} )
ENDIF(OTB_USE_CURL)

# For OpentTreads  header file
IF(NOT OTB_USE_EXTERNAL_OPENTHREADS)
        SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
                ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbopenthreads/OpenThreads/include)
ENDIF(NOT OTB_USE_EXTERNAL_OPENTHREADS)

#For FLTK header file
IF(OTB_USE_EXTERNAL_FLTK)
        SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
  		        ${FLTK_INCLUDE_DIRS})
ELSE(OTB_USE_EXTERNAL_FLTK)
        SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
  		        ${OTB_INSTALL_INCLUDE_DIR}/Utilities/FLTK)
ENDIF(OTB_USE_EXTERNAL_FLTK)

#For GLU header file
SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
  		${OTB_GLU_INCLUDE_PATH} )

#For ITK header file
IF(OTB_USE_EXTERNAL_ITK)
	SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
   			${ITK_INCLUDE_DIRS} )
#   			${ITK_INCLUDE_DIRS_INSTALL_TREE} 	)
ELSE(OTB_USE_EXTERNAL_ITK)

# JULIEN Seems that ITK_INCLUDE_DIRS_CONFIG replaces ITK_INCLUDE_DIRS_INSTALL_TREE
SET(ITK_INCLUDE_DIRS  ${OTB_INSTALL_INCLUDE_DIR}/Utilities/ITK/)
FOREACH(DIR ${ITK_INCLUDE_RELATIVE_DIRS})
  LIST(APPEND ITK_INCLUDE_DIRS ${OTB_INSTALL_INCLUDE_DIR}/Utilities/ITK/${DIR})
ENDFOREACH(DIR)
IF(ITK_INCLUDE_DIRS_SYSTEM)
  LIST(APPEND ITK_INCLUDE_DIR ${ITK_INCLUDE_DIRS_SYSTEM})
ENDIF(ITK_INCLUDE_DIRS_SYSTEM)


#THOMAS
	SET(OTB_INCLUDE_DIRS_INSTALL_TREE ${OTB_INCLUDE_DIRS_INSTALL_TREE}
			${ITK_INCLUDE_DIRS_INSTALL_TREE}
#JULIEN
			${ITK_INCLUDE_DIRS}
			 )
#			${ITK_INCLUDE_DIRS_BUILD_TREE}
#			${ITK_INCLUDE_DIRS_BUILD_TREE_CXX} )
ENDIF(OTB_USE_EXTERNAL_ITK)
