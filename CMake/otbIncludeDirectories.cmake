#-----------------------------------------------------------------------------
# Include directories from the build tree.
set(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_BINARY_DIR})

# These directories are always needed.
set(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
  ${OTB_SOURCE_DIR}/Code/BasicFilters
  ${OTB_SOURCE_DIR}/Code/ContrastEnhancement
  ${OTB_SOURCE_DIR}/Code/ChangeDetection
  ${OTB_SOURCE_DIR}/Code/Common
  ${OTB_SOURCE_DIR}/Code/DisparityMap
  ${OTB_SOURCE_DIR}/Code/FeatureExtraction
  ${OTB_SOURCE_DIR}/Code/Fusion
  ${OTB_SOURCE_DIR}/Code/Fuzzy
  ${OTB_SOURCE_DIR}/Code/GeospatialAnalysis
  ${OTB_SOURCE_DIR}/Code/Hyperspectral
  ${OTB_SOURCE_DIR}/Code/IO
  ${OTB_SOURCE_DIR}/Code/Learning
  ${OTB_SOURCE_DIR}/Code/Markov
  ${OTB_SOURCE_DIR}/Code/MultiScale
  ${OTB_SOURCE_DIR}/Code/MultiTemporal
  ${OTB_SOURCE_DIR}/Code/OBIA
  ${OTB_SOURCE_DIR}/Code/ObjectDetection
  ${OTB_SOURCE_DIR}/Code/Projections
  ${OTB_SOURCE_DIR}/Code/Radiometry
  ${OTB_SOURCE_DIR}/Code/SARPolarimetry
  ${OTB_SOURCE_DIR}/Code/Segmentation
  ${OTB_SOURCE_DIR}/Code/Simulation
  ${OTB_SOURCE_DIR}/Code/SpatialReasoning
  ${OTB_SOURCE_DIR}/Code/Testing
  ${OTB_SOURCE_DIR}/Code/UtilitiesAdapters/CurlAdapters
  ${OTB_SOURCE_DIR}/Code/UtilitiesAdapters/OssimAdapters
  ${OTB_SOURCE_DIR}/Code/UtilitiesAdapters/TinyXMLAdapters
  ${OTB_SOURCE_DIR}/Code/UtilitiesAdapters/OGRAdapters
  ${OTB_SOURCE_DIR}/Code/UtilitiesAdapters/ITKPendingPatches
  ${OTB_SOURCE_DIR}/Code/UtilitiesAdapters
  ${OTB_SOURCE_DIR}/Code/ApplicationEngine
  ${OTB_SOURCE_DIR}/Utilities/otbsvm
  ${OTB_SOURCE_DIR}/Utilities/otbossimplugins
  ${OTB_SOURCE_DIR}/Utilities/otbossimplugins/ossim
  ${OTB_SOURCE_DIR}/Utilities/InsightJournal
  ${OTB_SOURCE_DIR}/Utilities/otb6S
  ${OTB_SOURCE_DIR}/Utilities/otbedison/segm
  ${OTB_SOURCE_DIR}/Utilities/otbedison/prompt
  ${OTB_SOURCE_DIR}/Utilities/otbedison/edge
  ${OTB_SOURCE_DIR}/Utilities/otbconfigfile
)

if(OTB_USE_OPENCV)
  set(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
      ${OTB_SOURCE_DIR}/Code/UtilitiesAdapters/OpenCVAdapters )
endif()

if(OTB_WRAP_QT)
  set(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
    ${OTB_SOURCE_DIR}/Code/Wrappers/QtWidget )
endif()

if(OTB_USE_EXTERNAL_BOOST)
  set(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
    ${Boost_INCLUDE_DIR} )
else()
  set(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
    ${OTB_SOURCE_DIR}/Utilities/BGL )
endif()

set(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
    ${GDAL_INCLUDE_DIRS}
    ${JPEG_INCLUDE_DIRS}
    ${OGR_INCLUDE_DIRS})

if(MSVC)
  set(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
      ${OTB_SOURCE_DIR}/Utilities/otbmsinttypes )
endif()

if(OTB_USE_SIFTFAST)
  set(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
      ${OTB_SOURCE_DIR}/Utilities/otbsiftfast)
endif()

if(OTB_USE_EXTERNAL_TINYXML)
  set(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
      ${TINYXML_INCLUDE_DIRS})
else()
  set(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
      ${OTB_SOURCE_DIR}/Utilities/tinyXMLlib)
endif()

if(OTB_USE_EXTERNAL_MUPARSER)
  set(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
      ${MUPARSER_INCLUDE_DIR})
else()
  set(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
      ${OTB_SOURCE_DIR}/Utilities/otbmuparser)
endif()

if(OTB_USE_EXTERNAL_MUPARSERX)
  set(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
      ${MUPARSERX_INCLUDE_DIR})
else()
  set(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
      ${OTB_SOURCE_DIR}/Utilities/otbmuparserx)
endif()

if(OTB_USE_EXTERNAL_LIBKML)
  set(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
      ${LIBKML_INCLUDE_DIR})
else()
  set(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
      ${OTB_SOURCE_DIR}/Utilities/otbkml/src)
endif()

if(OTB_USE_MAPNIK)
  set(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
  	${MAPNIK_INCLUDE_DIR} ${FREETYPE2_INCLUDE_DIR})
endif()

if(OTB_USE_OPENCV)
   set(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
     ${OpenCV_INCLUDE_DIRS})
endif()

if(OTB_USE_EXTERNAL_ITK)
  set(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
  	${ITK_INCLUDE_DIRS})
else()
   # We add the ITK include dirs in the main CMakeLists.txt
   # after configuring ITK.
   # We cannot do it here since ITK is not yet configured
   # so ITK_INCLUDE_DIRS is empty
endif()

#-----------------------------------------------------------------------------
# Include directories needed for .cxx files in OTB.  These include
# directories will NOT be available to user projects.
set(OTB_INCLUDE_DIRS_BUILD_TREE_CXX)

set(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX} )

#-----------------------------------------------------------------------------
# Include directories from libraries build tree.


#For GLU header file
set(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
  		${OTB_GLU_INCLUDE_PATH} )

#For EXPAT header file
if(OTB_USE_EXTERNAL_EXPAT)
        set(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
  	        ${EXPAT_INCLUDE_DIR} )
else()
        set(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
                ${OTB_SOURCE_DIR}/Utilities/otbexpat
                ${OTB_SOURCE_DIR}/Utilities/otbexpat/lib)
endif()

# For OpenThreads
if(NOT OTB_USE_EXTERNAL_OPENTHREADS)
        set(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
                ${OTB_SOURCE_DIR}/Utilities/otbopenthreads/OpenThreads/include
	)
endif()

#For ITK header file
if(OTB_USE_EXTERNAL_ITK)
	set(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
   			${ITK_INCLUDE_DIRS} )
#   			${ITK_INCLUDE_DIRS_INSTALL_TREE} 	)
else()
	set(OTB_INCLUDE_DIRS_BUILD_TREE_CXX ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
   			${ITK_INCLUDE_DIRS_BUILD_TREE}
   			${ITK_INCLUDE_DIRS_BUILD_TREE_CXX} 	)
endif()







#-----------------------------------------------------------------------------
# Include directories from the install tree.
# Alls install dirs must been RELATIVE PATH, except for the external libraries, specify by the ABSOLUE PATH
# set(OTB_INSTALL_INCLUDE_PATH "${CMAKE_INSTALL_PREFIX}${OTB_INSTALL_INCLUDE_DIR}")
#           set(OTB_INCLUDE_RELATIVE_DIRS ${OTB_INCLUDE_RELATIVE_DIRS} )

#           set(OTB_INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib/otb")
#           set(OTB_INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin")
#           set(OTB_INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include/otb")

set(OTB_INCLUDE_RELATIVE_DIRS )
set(OTB_INCLUDE_ABSOLUE_DIRS )


set(OTB_INCLUDE_RELATIVE_DIRS ${OTB_INCLUDE_RELATIVE_DIRS}
  Common
  BasicFilters
  IO
  ChangeDetection
  FeatureExtraction
  Learning
  MultiScale
  MultiTemporal
  SpatialReasoning
  DisparityMap
  Projections
  Radiometry
  SARPolarimetry
  Segmentation
  Markov
  Fusion
  GeospatialAnalysis
  Hyperspectral
  Simulation
  Testing
  UtilitiesAdapters/CurlAdapters
  UtilitiesAdapters/OssimAdapters
  UtilitiesAdapters/TinyXMLAdapters
  UtilitiesAdapters/OGRAdapters
  UtilitiesAdapters/OpenCVAdapters
  UtilitiesAdapters/ITKPendingPatches
  UtilitiesAdapters
  OBIA
  Fuzzy
  ObjectDetection
  ApplicationEngine
  Utilities
  Utilities/otbsvm
  Utilities/InsightJournal
  Utilities/otb6S
  Utilities/otbedison
  Utilities/otbconfigfile
)

if(OTB_USE_OPENCV)
  set(OTB_INCLUDE_RELATIVE_DIRS ${OTB_INCLUDE_RELATIVE_DIRS}
      UtilitiesAdapters/OpenCVAdapters )
endif()

if(OTB_WRAP_QT)
  set(OTB_INCLUDE_RELATIVE_DIRS ${OTB_INCLUDE_RELATIVE_DIRS}
    Wrappers
    Wrappers/QtWidget )
endif()

#For BOOST header file
if(OTB_USE_EXTERNAL_BOOST)
  set(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
      ${Boost_INCLUDE_DIR} )
else()
  set(OTB_INCLUDE_RELATIVE_DIRS ${OTB_INCLUDE_RELATIVE_DIRS}
      Utilities/BGL)
endif()

if(WIN32)
  set(OTB_INCLUDE_RELATIVE_DIRS ${OTB_INCLUDE_RELATIVE_DIRS}
      Utilities/otbmsinttypes)
endif()

#For GDAL header file
set(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
    ${GDAL_INCLUDE_DIRS}
    ${JPEG_INCLUDE_DIRS}
    ${OGR_INCLUDE_DIRS})

#For EXPAT header file
if(OTB_USE_EXTERNAL_EXPAT)
        set(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
  	        ${EXPAT_INCLUDE_DIR} )
else()
endif()

#For MAPNIK header file
if(OTB_USE_MAPNIK)
  set(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
  	${MAPNIK_INCLUDE_DIR} )
endif()

if(OTB_USE_SIFTFAST)
  set(OTB_INCLUDE_RELATIVE_DIRS ${OTB_INCLUDE_RELATIVE_DIRS}
      Utilities/otbsiftfast)
endif()

#For TinyXML header file
if(OTB_USE_EXTERNAL_TINYXML)
        set(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
                ${TINYXML_INCLUDE_DIRS})
else()
        set(OTB_INCLUDE_RELATIVE_DIRS ${OTB_INCLUDE_RELATIVE_DIRS}
                Utilities/tinyXMLlib)
endif()

#For MuParser header file
if(OTB_USE_EXTERNAL_MUPARSER)
        set(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
                ${MUPARSER_INCLUDE_DIR})
else()
        set(OTB_INCLUDE_RELATIVE_DIRS ${OTB_INCLUDE_RELATIVE_DIRS}
                Utilities/otbmuparser)
endif()

#For LibKML header file
if(OTB_USE_EXTERNAL_LIBKML)
        set(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
                ${LIBKML_INCLUDE_DIR})
else()
        set(OTB_INCLUDE_RELATIVE_DIRS ${OTB_INCLUDE_RELATIVE_DIRS}
                Utilities/otbkml)
endif()

#For GLU header file
set(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
  		${OTB_GLU_INCLUDE_PATH} )

if(OTB_USE_OPENCV)
   set(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
     ${OpenCV_INCLUDE_DIRS})
endif()

#For ITK header file
if(OTB_USE_EXTERNAL_ITK)
  set(OTB_INCLUDE_ABSOLUE_DIRS ${OTB_INCLUDE_ABSOLUE_DIRS}
        ${ITK_INCLUDE_DIRS} )
else()
endif()
