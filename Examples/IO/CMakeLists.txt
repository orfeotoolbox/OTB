
project(IOExamples)

add_executable(ImageReadWrite ImageReadWrite.cxx )
target_link_libraries(ImageReadWrite OTBCommon OTBIO)

add_executable(StreamingImageReadWrite StreamingImageReadWrite.cxx )
target_link_libraries(StreamingImageReadWrite OTBCommon OTBIO)

add_executable(ExplicitStreamingExample ExplicitStreamingExample.cxx )
target_link_libraries(ExplicitStreamingExample OTBCommon OTBIO)


add_executable(ImageReadCastWrite ImageReadCastWrite.cxx )
target_link_libraries(ImageReadCastWrite OTBCommon OTBIO)

#add_executable(ImageReadExtractWrite ImageReadExtractWrite.cxx )
#target_link_libraries(ImageReadExtractWrite OTBCommon OTBIO)

add_executable(ImageReadRegionOfInterestWrite ImageReadRegionOfInterestWrite.cxx )
target_link_libraries(ImageReadRegionOfInterestWrite OTBCommon OTBIO)
add_executable(RGBImageReadWrite RGBImageReadWrite.cxx )
target_link_libraries(RGBImageReadWrite OTBCommon OTBIO)

#add_executable(CovariantVectorImageWrite CovariantVectorImageWrite.cxx )
#target_link_libraries(CovariantVectorImageWrite)

#add_executable(CovariantVectorImageRead CovariantVectorImageRead.cxx )
#target_link_libraries(CovariantVectorImageRead)

#add_executable(CovariantVectorImageExtractComponent CovariantVectorImageExtractComponent.cxx )
#target_link_libraries(CovariantVectorImageExtractComponent)

#add_executable(VectorImageReadWrite VectorImageReadWrite.cxx )
#target_link_libraries(VectorImageReadWrite)

add_executable(ComplexImageReadWrite ComplexImageReadWrite.cxx )
target_link_libraries(ComplexImageReadWrite OTBCommon OTBIO)

add_executable(MultibandImageReadWrite MultibandImageReadWrite.cxx )
target_link_libraries(MultibandImageReadWrite OTBCommon OTBIO)

add_executable(ExtractROI ExtractROI.cxx )
target_link_libraries(ExtractROI OTBCommon OTBIO)

add_executable(ImageSeriesIOExample ImageSeriesIOExample.cxx )
target_link_libraries(ImageSeriesIOExample OTBCommon OTBIO)


add_executable(MetadataExample MetadataExample.cxx )
target_link_libraries(MetadataExample OTBCommon OTBIO)

add_executable(VectorDataIOExample VectorDataIOExample.cxx )
target_link_libraries(VectorDataIOExample OTBCommon OTBIO)

add_executable(OGRWrappersExample OGRWrappersExample.cxx )
target_link_libraries(OGRWrappersExample OTBCommon OTBOGRAdapters)

add_executable(DEMHandlerExample DEMHandlerExample.cxx )
target_link_libraries(DEMHandlerExample OTBOssimAdapters OTBCommon OTBIO)

add_executable(DEMToImageGenerator DEMToImageGenerator.cxx )
target_link_libraries(DEMToImageGenerator OTBProjections OTBIO OTBCommon )

if( OTB_USE_CURL )
  add_executable(TileMapImageIOExample TileMapImageIOExample.cxx)
  target_link_libraries(TileMapImageIOExample OTBIO OTBCommon)
endif()

add_executable(ImageToKmzAndMapFileProductExample ImageToKmzAndMapFileProductExample.cxx)
target_link_libraries(ImageToKmzAndMapFileProductExample OTBIO)

add_executable(HDFReaderExample HDFReaderExample.cxx)
target_link_libraries(HDFReaderExample OTBIO)

if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )


set(BASELINE ${OTB_DATA_ROOT}/Baseline/Examples/IO)
set(DATA ${OTB_DATA_ROOT}/Input)

set(INPUTDATA ${OTB_DATA_ROOT}/Examples)
set(TEMP ${OTB_BINARY_DIR}/Testing/Temporary)


set(EXE_TESTS1  ${CXX_TEST_PATH}/otbIOExamplesTests1)
set(EXE_TESTS2  ${CXX_TEST_PATH}/otbIOExamplesTests2) # Need curl
set(EXE_TESTS3  ${CXX_TEST_PATH}/otbIOExamplesTests3)

set(NOTOL 0.0)
set(EPSILON_5 0.00001)
set(HIGHTOL 10.0)

# ------- ExtractROITest----------

add_test(ioTeExtractROITest ${EXE_TESTS1}
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

add_test(ioTeDEMToImageGeneratorTest ${EXE_TESTS1}
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

add_test(prTeDEMHandlerExampleTest ${EXE_TESTS1}
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
add_test(ioTeImageToKmzAndMapFileProductExample ${EXE_TESTS1}
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


if( OTB_USE_CURL )
add_test(ioTeTileMapImageIOExampleTest ${EXE_TESTS2}
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
# add_test(ioTeTileMapImageIOExample2Test ${EXE_TESTS2}
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

endif()




include_directories(${OTB_SOURCE_DIR}/Testing/Code)
add_executable(otbIOExamplesTests1 otbIOExamplesTests1.cxx)
target_link_libraries(otbIOExamplesTests1 OTBIO OTBCommon OTBOssimAdapters OTBTesting)

if( OTB_USE_CURL )
  add_executable(otbIOExamplesTests2 otbIOExamplesTests2.cxx)
  target_link_libraries(otbIOExamplesTests2 OTBCommon OTBIO OTBTesting)
endif()


if(ITK_USE_REVIEW)
  add_executable(otbIOExamplesTests3 otbIOExamplesTests3.cxx)
  target_link_libraries(otbIOExamplesTests3 OTBCommon OTBIO OTBTesting)
endif()




endif()
