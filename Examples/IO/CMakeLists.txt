
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

ADD_EXECUTABLE(VectorDataIOExample VectorDataIOExample.cxx )
TARGET_LINK_LIBRARIES(VectorDataIOExample OTBCommon OTBIO)

ADD_EXECUTABLE(OGRWrappersExample OGRWrappersExample.cxx )
TARGET_LINK_LIBRARIES(OGRWrappersExample OTBCommon OTBOGRAdapters)

ADD_EXECUTABLE(DEMHandlerExample DEMHandlerExample.cxx )
TARGET_LINK_LIBRARIES(DEMHandlerExample OTBOssimAdapters OTBCommon OTBIO)

ADD_EXECUTABLE(DEMToImageGenerator DEMToImageGenerator.cxx )
TARGET_LINK_LIBRARIES(DEMToImageGenerator OTBProjections OTBIO OTBCommon )

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

ADD_EXECUTABLE(ImageToKmzAndMapFileProductExample ImageToKmzAndMapFileProductExample.cxx)
TARGET_LINK_LIBRARIES(ImageToKmzAndMapFileProductExample OTBIO)

ADD_EXECUTABLE(HDFReaderExample HDFReaderExample.cxx)
TARGET_LINK_LIBRARIES(HDFReaderExample OTBIO)

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

ADD_TEST(prTeDEMHandlerExampleTest ${EXE_TESTS1}
  DEMHandlerExampleTest
  ${DATA}/DEM/srtm_directory/
  ${DATA}/DEM/egm96.grd
  40
  8.434583
  44.647083
  383.580313671
  0.001
)

# Testing the chain of creating a kmz and MapFilefrom
# a non georeferenced image
ADD_TEST(ioTeImageToKmzAndMapFileProductExample ${EXE_TESTS1}
         ImageToKmzAndMapFileProductExample
         ${DATA}/Haiti1_sr10.jpeg
         ${TEMP}/ioTeHaitiKmzProduct.kmz
         ${TEMP}/mapserverExample/ioTeHaitiMapFileProduct.map
         ${TEMP}/mapserverExample/shapeindex
         http://localhost/cgi-bin/mapserv
         ${DATA}/DEM/srtm_directory
         198.000000 199.000000 -72.351257 18.555075 0.000000
         196.000000 194.000000 -72.352051 18.556946 0.000000
         208.000000 157.000000 -72.346130 18.574358 0.000000
         215.000000 143.000000 -72.342377 18.580441 0.000000
         243.000000 141.000000 -72.328964 18.581335 0.000000
         256.000000 93.000000  -72.322571 18.602974 0.000000
         349.000000 140.000000 -72.278343 18.580887 0.000000
         289.000000 144.000000 -72.306908 18.579464 0.000000
         210.000000 247.000000 -72.345741 18.532839 0.000000
         194.000000 247.000000 -72.353317 18.532595 0.00000
         164.000000 241.000000 -72.367691 18.535810 0.000000
         144.000000 245.000000 -72.377457 18.534243 0.000000
         69.000000  201.000000 -72.413315 18.554974 0.000000
         74.000000  237.000000 -72.410934 18.538496 0.000000
         86.000000  280.000000 -72.405739 18.518456 0.000000
         193.000000 286.000000 -72.353920 18.515240 0.000000
         334.000000 282.000000 -72.286240 18.515160 0.000000
         373.000000 177.000000 -72.267570 18.563681 0.000000
         368.000000 158.000000 -72.269203 18.572529 0.000000
         350.000000 93.000000  -72.282402 18.602425 0.000000
         343.000000 86.000000  -72.280792 18.605862 0.000000
         282.000000 71.000000  -72.309692 18.613203 0.000000
         189.000000 36.000000  -72.354301 18.629776 0.000000
         399.000000 264.000000 -72.255020 18.523664 0.000000
         42.000000  232.000000 -72.426552 18.540876 0.000000
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

# Access to NearMap is subject to an "agreement" checkbox ...
# ADD_TEST(ioTeTileMapImageIOExample2Test ${EXE_TESTS2}
# --compare-image ${HIGHTOL}  ${BASELINE}/nearMap.jpg
#                         ${TEMP}/nearMap.jpg
#          TileMapImageIOExampleTest
#          ${INPUTDATA}/nearmapfile.otb
#          ${TEMP}/nearMap.jpg
#          ${TEMP}
#          1.4835345
#          43.55968261
#          8
#          )

ENDIF( OTB_USE_CURL )


IF(OTB_USE_LIBLAS)
IF(ITK_USE_REVIEW)
ADD_TEST(ioTeLidarToImageExampleTest ${EXE_TESTS3}
--compare-image ${NOTOL}  ${BASELINE}/lidar-image-4.hdr
                        ${TEMP}/lidar-image-4.hdr
         LidarToImageExampleTest
         ${DATA}/srs.las
         ${TEMP}/lidar-image-4.hdr
         ${TEMP}/lidar-image-4.png
         0.2
         5
         4
         )
ADD_TEST(ioTeLidarToImageExample2Test ${EXE_TESTS3}
--compare-image ${NOTOL}  ${BASELINE}/lidar-image-8.hdr
                        ${TEMP}/lidar-image-8.hdr
         LidarToImageExampleTest
         ${DATA}/srs.las
         ${TEMP}/lidar-image-8.hdr
         ${TEMP}/lidar-image-8.png
         0.2
         5
         8
         )
ENDIF(ITK_USE_REVIEW)
ENDIF(OTB_USE_LIBLAS)



INCLUDE_DIRECTORIES(${OTB_SOURCE_DIR}/Testing/Code)
ADD_EXECUTABLE(otbIOExamplesTests1 otbIOExamplesTests1.cxx)
TARGET_LINK_LIBRARIES(otbIOExamplesTests1 OTBIO OTBCommon OTBOssimAdapters OTBTesting)

IF( OTB_USE_CURL )
  ADD_EXECUTABLE(otbIOExamplesTests2 otbIOExamplesTests2.cxx)
  TARGET_LINK_LIBRARIES(otbIOExamplesTests2 OTBCommon OTBIO OTBTesting)
ENDIF( OTB_USE_CURL )

IF(OTB_USE_LIBLAS)
IF(ITK_USE_REVIEW)
  ADD_EXECUTABLE(otbIOExamplesTests3 otbIOExamplesTests3.cxx)
  TARGET_LINK_LIBRARIES(otbIOExamplesTests3 OTBCommon OTBIO OTBTesting)
ENDIF(ITK_USE_REVIEW)
ENDIF(OTB_USE_LIBLAS)




ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
