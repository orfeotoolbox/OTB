
PROJECT(IOExamples)

ADD_EXECUTABLE(ImageReadWrite ImageReadWrite.cxx )
TARGET_LINK_LIBRARIES(ImageReadWrite OTBCommon OTBIO)

ADD_EXECUTABLE(StreamingImageReadWrite StreamingImageReadWrite.cxx )
TARGET_LINK_LIBRARIES(StreamingImageReadWrite OTBCommon OTBIO)

ADD_EXECUTABLE(ExplicitStreamingExample ExplicitStreamingExample.cxx )
TARGET_LINK_LIBRARIES(ExplicitStreamingExample OTBCommon OTBIO)


ADD_EXECUTABLE(ImageReadCastWrite ImageReadCastWrite.cxx )
TARGET_LINK_LIBRARIES(ImageReadCastWrite OTBCommon OTBIO)

#ADD_EXECUTABLE(ImageReadExtractWrite ImageReadExtractWrite.cxx )
#TARGET_LINK_LIBRARIES(ImageReadExtractWrite OTBCommon OTBIO)

ADD_EXECUTABLE(ImageReadRegionOfInterestWrite ImageReadRegionOfInterestWrite.cxx )
TARGET_LINK_LIBRARIES(ImageReadRegionOfInterestWrite OTBCommon OTBIO)
ADD_EXECUTABLE(RGBImageReadWrite RGBImageReadWrite.cxx )
TARGET_LINK_LIBRARIES(RGBImageReadWrite OTBCommon OTBIO)

#ADD_EXECUTABLE(CovariantVectorImageWrite CovariantVectorImageWrite.cxx )
#TARGET_LINK_LIBRARIES(CovariantVectorImageWrite)

#ADD_EXECUTABLE(CovariantVectorImageRead CovariantVectorImageRead.cxx )
#TARGET_LINK_LIBRARIES(CovariantVectorImageRead)

#ADD_EXECUTABLE(CovariantVectorImageExtractComponent CovariantVectorImageExtractComponent.cxx )
#TARGET_LINK_LIBRARIES(CovariantVectorImageExtractComponent)

#ADD_EXECUTABLE(VectorImageReadWrite VectorImageReadWrite.cxx )
#TARGET_LINK_LIBRARIES(VectorImageReadWrite)

ADD_EXECUTABLE(ComplexImageReadWrite ComplexImageReadWrite.cxx )
TARGET_LINK_LIBRARIES(ComplexImageReadWrite OTBCommon OTBIO)

ADD_EXECUTABLE(MultibandImageReadWrite MultibandImageReadWrite.cxx )
TARGET_LINK_LIBRARIES(MultibandImageReadWrite OTBCommon OTBIO)

ADD_EXECUTABLE(ExtractROI ExtractROI.cxx )
TARGET_LINK_LIBRARIES(ExtractROI OTBCommon OTBIO)

ADD_EXECUTABLE(ImageSeriesIOExample ImageSeriesIOExample.cxx )
TARGET_LINK_LIBRARIES(ImageSeriesIOExample OTBCommon OTBIO)


ADD_EXECUTABLE(MetadataExample MetadataExample.cxx )
TARGET_LINK_LIBRARIES(MetadataExample OTBCommon OTBIO)

ADD_EXECUTABLE(DXFReaderExample DXFReaderExample.cxx )
TARGET_LINK_LIBRARIES(DXFReaderExample OTBCommon OTBIO)

ADD_EXECUTABLE(VectorDataIOExample VectorDataIOExample.cxx )
TARGET_LINK_LIBRARIES(VectorDataIOExample OTBCommon OTBIO)

ADD_EXECUTABLE(DEMToImageGenerator DEMToImageGenerator.cxx )
TARGET_LINK_LIBRARIES(DEMToImageGenerator OTBProjections OTBIO OTBCommon )

ADD_EXECUTABLE(DEMToOrthoImageGenerator DEMToOrthoImageGenerator.cxx )
TARGET_LINK_LIBRARIES(DEMToOrthoImageGenerator OTBProjections OTBIO OTBCommon )

IF(OTB_USE_LIBLAS)
  ADD_EXECUTABLE(LidarReaderExample LidarReaderExample.cxx )
  TARGET_LINK_LIBRARIES(LidarReaderExample OTBIO OTBCommon )

  IF(ITK_USE_REVIEW)
    ADD_EXECUTABLE(LidarToImageExample LidarToImageExample.cxx )
    TARGET_LINK_LIBRARIES(LidarToImageExample OTBIO OTBCommon )
  ENDIF(ITK_USE_REVIEW)
ENDIF(OTB_USE_LIBLAS)

IF( OTB_USE_CURL )
  ADD_EXECUTABLE(TileMapImageIOExample TileMapImageIOExample.cxx)
  TARGET_LINK_LIBRARIES(TileMapImageIOExample OTBIO OTBCommon)
ENDIF( OTB_USE_CURL )


#ADD_EXECUTABLE(ImageReadExportVTK ImageReadExportVTK.cxx )
#TARGET_LINK_LIBRARIES(ImageReadExportVTK)

#ADD_EXECUTABLE(RGBImageSeriesReadWrite RGBImageSeriesReadWrite.cxx )
#TARGET_LINK_LIBRARIES(RGBImageSeriesReadWrite)

#ADD_EXECUTABLE(ImageSeriesReadWrite ImageSeriesReadWrite.cxx )
#TARGET_LINK_LIBRARIES(ImageSeriesReadWrite)

#ADD_EXECUTABLE(ImageReadImageSeriesWrite ImageReadImageSeriesWrite.cxx )
#TARGET_LINK_LIBRARIES(ImageReadImageSeriesWrite)

#ADD_EXECUTABLE(DicomSeriesReadImageWrite DicomSeriesReadImageWrite.cxx )
#TARGET_LINK_LIBRARIES(DicomSeriesReadImageWrite)

#ADD_EXECUTABLE(DicomSeriesReadImageWrite2 DicomSeriesReadImageWrite2.cxx )
#TARGET_LINK_LIBRARIES(DicomSeriesReadImageWrite2)

#ADD_EXECUTABLE(DicomSeriesReadPrintTags DicomSeriesReadPrintTags.cxx )
#TARGET_LINK_LIBRARIES(DicomSeriesReadPrintTags)

#ADD_EXECUTABLE(DicomImageReadPrintTags DicomImageReadPrintTags.cxx )
#TARGET_LINK_LIBRARIES(DicomImageReadPrintTags)

#ADD_EXECUTABLE(DicomImageReadWrite DicomImageReadWrite.cxx )
#TARGET_LINK_LIBRARIES(DicomImageReadWrite)

#ADD_EXECUTABLE(DicomImageReadChangeHeaderWrite DicomImageReadChangeHeaderWrite.cxx )
#TARGET_LINK_LIBRARIES(DicomImageReadChangeHeaderWrite)

#ADD_EXECUTABLE(DicomSeriesReadSeriesWrite DicomSeriesReadSeriesWrite.cxx )
#TARGET_LINK_LIBRARIES(DicomSeriesReadSeriesWrite)

#ADD_EXECUTABLE(DicomPrintPatientInformation DicomPrintPatientInformation.cxx )
#TARGET_LINK_LIBRARIES(DicomPrintPatientInformation)

#ADD_EXECUTABLE(TransformReadWrite TransformReadWrite.cxx )
#TARGET_LINK_LIBRARIES(TransformReadWrite)

ADD_EXECUTABLE(ImageToKmzAndMapFileProductExample ImageToKmzAndMapFileProductExample.cxx)
TARGET_LINK_LIBRARIES(ImageToKmzAndMapFileProductExample OTBIO)

IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/IO)
SET(DATA ${OTB_DATA_ROOT}/Input)

SET(INPUTDATA ${OTB_DATA_ROOT}/Examples)
SET(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)


SET(EXE_TESTS1  ${CXX_TEST_PATH}/otbIOExamplesTests1)
SET(EXE_TESTS2  ${CXX_TEST_PATH}/otbIOExamplesTests2) # Need curl
SET(EXE_TESTS3  ${CXX_TEST_PATH}/otbIOExamplesTests3) # Need liblas

SET(NOTOL 0.0)
SET(EPSILON_5 0.00001)
SET(HIGHTOL 10.0)

# ------- ExtractROITest----------

ADD_TEST(ioTeExtractROITest ${EXE_TESTS1}
	--compare-n-images ${NOTOL} 2
	${BASELINE}/ROI_IMAGERY_RGB.png
	${TEMP}/ROI_IMAGERY_RGB.png

	${BASELINE}/ROI_IMAGERY_MIR.png
	${TEMP}/ROI_IMAGERY_MIR.png
	ExtractROITest
	${INPUTDATA}/IMAGERY_SSECH.tif
	${TEMP}/ROI_IMAGERY_RGB.png
	${TEMP}/ROI_IMAGERY_MIR.png
    0 0 100 100
)

ADD_TEST(ioTeDEMToImageGeneratorTest ${EXE_TESTS1}
--compare-image ${EPSILON_5}  ${BASELINE}/DEMToImageGenerator.tif
                        ${TEMP}/DEMToImageGenerator.tif
        DEMToImageGeneratorTest
         ${TEMP}/DEMToImageGenerator.tif
         ${TEMP}/pretty_DEMToImageGenerator.png
         6.5
         45.5
         500
         500
         0.002
         -0.002
          ${INPUTDATA}/DEM_srtm
         )

# Testing the chain of creating a kmz and MapFilefrom 
# a non georeferenced image
ADD_TEST(ioImageToKmzAndMapFileProductExample ${EXE_TESTS1}
         ImageToKmzAndMapFileProductExample
         ${DATA}/Haiti1.jpeg
         ${TEMP}/ioTeHaitiKmzProduct.kmz
         ${TEMP}/mapserverExample/ioTeHaitiMapFileProduct.map
         ${TEMP}/mapserverExample/shapeindex
         http://localhost/cgi-bin/mapserv
         1980.000000 1986.000000 -72.351257 18.555075 0.000000 
         1963.000000 1944.000000 -72.352051 18.556946 0.000000
         2078.000000 1566.000000 -72.346130 18.574358 0.000000
         2154.000000 1430.000000 -72.342377 18.580441 0.000000
         2432.000000 1408.000000 -72.328964 18.581335 0.000000
         2558.000000 934.000000 -72.322571 18.602974 0.000000
         3492.000000 1397.000000 -72.278343 18.580887 0.000000
         2890.000000 1439.000000 -72.306908 18.579464 0.000000
         2098.000000 2469.000000 -72.345741 18.532839 0.000000
         1941.000000 2473.000000 -72.353317 18.532595 0.00000 
         1644.000000 2409.000000 -72.367691 18.535810 0.000000
         1440.000000 2448.000000 -72.377457 18.534243 0.000000
         687.000000 2005.000000 -72.413315 18.554974 0.000000
         741.000000 2366.000000 -72.410934 18.538496 0.000000
         855.000000 2802.000000 -72.405739 18.518456 0.000000
         1936.000000 2855.000000 -72.353920 18.515240 0.000000
         3338.000000 2824.000000 -72.286240 18.515160 0.000000
         3715.000000 1772.000000 -72.267570 18.563681 0.000000
         3676.000000 1580.000000 -72.269203 18.572529 0.000000
         3395.000000 931.000000 -72.282402 18.602425 0.000000
         3426.000000 856.000000 -72.280792 18.605862 0.000000
         2822.000000 707.000000 -72.309692 18.613203 0.000000
         1891.000000 360.000000 -72.354301 18.629776 0.000000
         3989.000000 2639.000000 -72.255020 18.523664 0.000000
         417.000000 2317.000000 -72.426552 18.540876 0.000000
         )
       

IF( OTB_USE_CURL )
ADD_TEST(ioTeTileMapImageIOExampleTest ${EXE_TESTS2}
--compare-image ${HIGHTOL}  ${BASELINE}/openStreetMap.png
                        ${TEMP}/openStreetMap.png
         TileMapImageIOExampleTest
         ${INPUTDATA}/osmfile.otb
         ${TEMP}/openStreetMap.png
         ${TEMP}
         1.4835345
         43.55968261
         12
         )
         
 ADD_TEST(ioTeTileMapImageIOExample2Test ${EXE_TESTS2}
--compare-image ${HIGHTOL}  ${BASELINE}/nearMap.jpg
                        ${TEMP}/nearMap.jpg
         TileMapImageIOExampleTest
         ${INPUTDATA}/nearmapfile.otb
         ${TEMP}/nearMap.jpg
         ${TEMP}
         1.4835345
         43.55968261
         8
         )

ENDIF( OTB_USE_CURL )


IF(OTB_USE_LIBLAS)
IF(ITK_USE_REVIEW)
ADD_TEST(ioTeLidarToImageExampleTest ${EXE_TESTS3}
--compare-image ${NOTOL}  ${BASELINE}/lidar-image-4.hdr
                        ${TEMP}/lidar-image-4.hdr
         LidarToImageExampleTest
         ${DATA}/TO_core_last_zoom.las
         ${TEMP}/lidar-image-4.hdr
         ${TEMP}/lidar-image-4.png
         1.0
         5
         4
         )
ADD_TEST(ioTeLidarToImageExample2Test ${EXE_TESTS3}
--compare-image ${NOTOL}  ${BASELINE}/lidar-image-8.hdr
                        ${TEMP}/lidar-image-8.hdr
         LidarToImageExampleTest
         ${DATA}/TO_core_last_zoom.las
         ${TEMP}/lidar-image-8.hdr
         ${TEMP}/lidar-image-8.png
         1.0
         5
         8
         )
ENDIF(ITK_USE_REVIEW)
ENDIF(OTB_USE_LIBLAS)



INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)
ADD_EXECUTABLE(otbIOExamplesTests1 otbIOExamplesTests1.cxx)
TARGET_LINK_LIBRARIES(otbIOExamplesTests1 OTBIO OTBCommon OTBTesting)

IF( OTB_USE_CURL )
  ADD_EXECUTABLE(otbIOExamplesTests2 otbIOExamplesTests2.cxx)
  TARGET_LINK_LIBRARIES(otbIOExamplesTests2 OTBCommon OTBIO ${CURL_LIBRARY}  OTBTesting)
ENDIF( OTB_USE_CURL )

IF(OTB_USE_LIBLAS)
IF(ITK_USE_REVIEW)
  ADD_EXECUTABLE(otbIOExamplesTests3 otbIOExamplesTests3.cxx)
  TARGET_LINK_LIBRARIES(otbIOExamplesTests3 OTBCommon OTBIO OTBTesting)
ENDIF(ITK_USE_REVIEW)
ENDIF(OTB_USE_LIBLAS)




ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
