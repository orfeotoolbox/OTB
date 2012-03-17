
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Remote sensing images (large images )
IF(OTB_DATA_USE_LARGEINPUT)
  SET(IMAGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)

SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Tolerance sur diff pixel image
SET(NOTOL 0.0)
SET(EPSILON_6 0.000001)
SET(EPSILON_4 0.0001)

SET(UTILITIES_TESTS ${CXX_TEST_PATH}/otbUtilitiesTests)


# Commentaire parce que trop long. Supprimer commentaire si besoin de tester integration lib Ossim
#ADD_TEST(utTvOssimIntegrationTest ${UTILITIES_TESTS}
#        ossimIntegrationTest
#        ossimTiffWriter
#        ${IMAGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
#        ${TEMP}/utOssimIntegration.tif
#)

IF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(utTvOssimKeywordlistToulouseQuickBirdTest ${UTILITIES_TESTS}
--ignore-order --compare-ascii ${EPSILON_6}
               ${BASELINE_FILES}/utOssimKeywordlistToulouseQuickBird.txt
               ${TEMP}/utOssimKeywordlistToulouseQuickBird.txt
        ossimKeywordlistTest
        ${IMAGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/utOssimKeywordlistToulouseQuickBird.txt
        )

ADD_TEST(utTvOssimKeywordlistBlossevilleIkonosTest ${UTILITIES_TESTS}
--ignore-order --compare-ascii ${NOTOL}
                ${BASELINE_FILES}/utOssimKeywordlistBlossevilleIkonos.txt
                ${TEMP}/utOssimKeywordlistBlossevilleIkonos.txt
        ossimKeywordlistTest
        ${IMAGEDATA}/IKONOS/BLOSSEVILLE/po_2619900_grn_0000000.tif
        ${TEMP}/utOssimKeywordlistBlossevilleIkonos.txt
        )
ENDIF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(utTuOssimElevManagerTest ${UTILITIES_TESTS}
        ossimElevManagerTest
        )

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(utTvOssimXmlDocumentTest ${UTILITIES_TESTS}
        ossimXmlDocumentTest
        ${IMAGEDATA}/TERRASARX/TORONTO/TSX1_SAR__SSC/TSX1_SAR__SSC.xml
        )
ENDIF(OTB_DATA_USE_LARGEINPUT)

ADD_TEST(utTvOssimTileMapModelTestToulouse ${UTILITIES_TESTS}
        --compare-ascii ${NOTOL}
                ${BASELINE_FILES}/utOssimTileMapModelToulouse.txt
                ${TEMP}/utOssimTileMapModelToulouse.txt
        ossimTileMapModelTest
        43.6044
        1.44295
        ${TEMP}/utOssimTileMapModelToulouse.txt
        )
        
ADD_TEST(utTvOssimpluginsHermiteInterpolationTest ${UTILITIES_TESTS}
     ossimpluginsHermiteInterpolationTest)

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(utTvOssimpluginsHermiteInterpolationPlateformPositionRadarSat2Test ${UTILITIES_TESTS}
        ossimpluginsHermiteInterpolationPlateformPositionTest
        ${IMAGEDATA}/RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/product.xml
    )
ENDIF(OTB_DATA_USE_LARGEINPUT)


# -------            lib otbsvm   ------------------------------

ADD_TEST(utTuSvmKernelFunctorTest ${UTILITIES_TESTS}
                svmGenericKernelFunctor
)

ADD_TEST(utTvSvmTest ${UTILITIES_TESTS}
--compare-ascii ${NOTOL}  ${INPUTDATA}/svm_model
                ${TEMP}/svm_model_test
                svmTest
                ${INPUTDATA}/svm_model
                ${TEMP}/svm_model_test
)

ADD_TEST(utTvSvmTestNewVersionFileGenericModelTest ${UTILITIES_TESTS}
--compare-ascii ${NOTOL}  ${BASELINE_FILES}/utTvsvm_model_image_generic_version_test
                ${TEMP}/utTvsvm_model_image_generic_version_test
                svmTest
                ${INPUTDATA}/svm_model_image_generic_version
                ${TEMP}/utTvsvm_model_image_generic_version_test
)

ADD_TEST(utTvSvmTestGenericModelTest ${UTILITIES_TESTS}
--compare-ascii ${NOTOL}  ${INPUTDATA}/svm_model_image_generic_kernel
                ${TEMP}/svm_model_image_generic_kernel_test
                svmGenericKernelTest
                ${INPUTDATA}/svm_model_image_generic_kernel
                ${TEMP}/svm_model_image_generic_kernel_test
)

ADD_TEST(utTvSvmGenericKernelBasicOperationsTest ${UTILITIES_TESTS}
--compare-ascii ${NOTOL}  ${BASELINE_FILES}/utTvsvmGenericKernelBasicOperationsTest.txt
                         ${TEMP}/svmGenericKernelBasicOperationsTest.txt
              svmGenericKernelBasicOperationsTest
                 ${TEMP}/svmGenericKernelBasicOperationsTest.txt
)

ADD_TEST(utTvSvmComposedKernelFunctorTest ${UTILITIES_TESTS}
--compare-n-ascii ${NOTOL} 3
                  ${BASELINE_FILES}/utTvSVMComposedKernelFunctorResTest.txt
                  ${TEMP}/SVMComposedKernelFunctorTestRes.txt
                  ${BASELINE_FILES}/utTvSVMComposedKernelFunctorTest.txt
                  ${TEMP}/SVMComposedKernelFunctorTestOutput.txt
                  ${BASELINE_FILES}/utTvSVMComposedKernelFunctorTest.txt
                  ${TEMP}/SVMComposedKernelFunctorTestOutputBis.txt

               otbSVMComposedKernelFunctorTest
                   ${TEMP}/SVMComposedKernelFunctorTestRes.txt
                   ${TEMP}/SVMComposedKernelFunctorTestOutput.txt
                   ${TEMP}/SVMComposedKernelFunctorTestOutputBis.txt
)

# -------            lib otb6S   ------------------------------

# The file is not read. It a screen user scan. main.exe < 132.149.107.66.wloip20967.INP
#ADD_TEST(utTv6SMainTest ${UTILITIES_TESTS}
#                SIXSMainTest
#                ${INPUTDATA}/132.149.107.66.wloip20967.INP
#)

ADD_TEST(utTv6SFunctionMainTest ${UTILITIES_TESTS}
--compare-ascii ${EPSILON_4}  ${BASELINE_FILES}/ut6S_function_main_test.txt
                ${TEMP}/ut6S_function_main_test.txt
                SIXSFunctionMainOtbTest
                ${INPUTDATA}/in6S_otb
                ${TEMP}/ut6S_function_main_test.txt
)

# -------            lib tinyXMLlib   ------------------------------
ADD_TEST(utTvTinyXMLTest ${UTILITIES_TESTS}
        --compare-ascii ${NOTOL}
                        ${BASELINE_FILES}/utTinyXML_testStdout.txt
                        ${TEMP}/utTinyXML_testStdout.txt
                tinyXMLlibTest
                ${INPUTDATA}/utf8test.xml
                ${INPUTDATA}/utf8testverify.xml
                ${TEMP}/demotest.xml
                ${TEMP}/utf8testout.xml
                ${TEMP}/passages.xml
                ${TEMP}/test5.xml
                ${TEMP}/test6.xml
                ${TEMP}/test7.xml
                ${TEMP}/textfile.txt
                ${TEMP}/utTinyXML_testStdout.txt
)

IF(OTB_DATA_USE_LARGEINPUT)
ADD_TEST(utTvOssimRadarSatSupport ${UTILITIES_TESTS}
        ossimRadarSatSupport
        ${IMAGEDATA}/RADARSAT1/GOMA2/SCENE01/DAT_01.001
        )
ENDIF(OTB_DATA_USE_LARGEINPUT)


# -------            lib OpenJpeg   ------------------------------
# Case for release 2.2.1
#IF(OTB_USE_JPEG2000)
#ADD_TEST(utTvOpenJpegEncoder ${UTILITIES_TESTS}
#        openJpegEncoder
#        ${TEMP}/testjpeg.j2k
#)
#
#ADD_TEST(utTvOpenJpegDecoder ${UTILITIES_TESTS}
#        openJpegDecoder
#        ${TEMP}/testjpeg.j2k
#)
#SET_TESTS_PROPERTIES(utTvOpenJpegDecoder PROPERTIES DEPENDS utTvOpenJpegEncoder)
#ENDIF(OTB_USE_JPEG2000)

# -------            lib expat  ------------------------------
IF(NOT OTB_USE_EXTERNAL_EXPAT)
ADD_TEST(utTvExpatRunTests ${UTILITIES_TESTS}
        expatruntests
        -q)
ENDIF(NOT OTB_USE_EXTERNAL_EXPAT)

# -------            lib OpenThreads  ------------------------------
IF(NOT OTB_USE_EXTERNAL_OPENTHREADS)
  ADD_TEST(utTvOpenthreadsSimpleThreader ${UTILITIES_TESTS}
        openthreadsSimpleThreader
        2 # [NUM_ELTS]
        1 # [NUM_THREADS]
        )
  ADD_TEST(utTvOpenthreadsWorkCrew ${UTILITIES_TESTS}
        openthreadsWorkCrew
        1 # [NUM_THREADS]
        )
ENDIF(NOT OTB_USE_EXTERNAL_OPENTHREADS)

# -------            SIFT   -------------------------------------
ADD_TEST(utTv2DSiftImageSquareSyntheticRotate10 ${UTILITIES_TESTS}
         itk2DScaleInvariantFeatureImageFilterTest
         ${INPUTDATA}/carre.png
         ${TEMP}/itk2DScaleInvariantFeatureImageFilter_carre1.png
         ${TEMP}/itk2DScaleInvariantFeatureImageFilter_carre2.png
         ${TEMP}/itk2DScaleInvariantFeatureImageFilter_carre_keys1.png
         ${TEMP}/itk2DScaleInvariantFeatureImageFilter_carre_keys2.png
         1 10 0.5 0  )

ADD_TEST(utTv2DSiftImageGridSyntheticRotate10 ${UTILITIES_TESTS}
         itk2DScaleInvariantFeatureImageFilterTest
         ${INPUTDATA}/damier.png
         ${TEMP}/itk2DScaleInvariantFeatureImageFilter_damier1.png
         ${TEMP}/itk2DScaleInvariantFeatureImageFilter_damier2.png
         ${TEMP}/itk2DScaleInvariantFeatureImageFilter_damier_keys1.png
         ${TEMP}/itk2DScaleInvariantFeatureImageFilter_damier_keys2.png
         1 10 0.5 0 )

ADD_TEST(utTv2DSiftImageSceneSyntheticRotate10 ${UTILITIES_TESTS}
         itk2DScaleInvariantFeatureImageFilterTest
         ${INPUTDATA}/scene.png
         ${TEMP}/itk2DScaleInvariantFeatureImageFilter_scene1.png
         ${TEMP}/itk2DScaleInvariantFeatureImageFilter_scene2.png
         ${TEMP}/itk2DScaleInvariantFeatureImageFilter_scene_keys1.png
         ${TEMP}/itk2DScaleInvariantFeatureImageFilter_scene_keys2.png
         1 10 0.5 0 )

# -------            lib Kml   ------------------------------
FOREACH(id_test createkml helloattrs helloenum hellofolder hellogeometry helloregion helloworld parsekml )
        ADD_TEST(utTuKml${id_test} ${UTILITIES_TESTS}
        kml${id_test}
        ${TEMP}/utkml${id_test}.txt
        )
ENDFOREACH(id_test)


ADD_TEST(utTvKmlhellofeatures   ${UTILITIES_TESTS}
        kmlhellofeatures
        ${INPUTDATA}/MODIS20040214212340.kml #abstractview.kml
#        ${TEMP}/utkmlhellofeatures.txt
        )

ADD_TEST(utTvKmlhellohref   ${UTILITIES_TESTS}
        kmlhellohref
        ${INPUTDATA}/abstractview.kml
#        ${TEMP}/utkmlhellohref.txt
        )
ADD_TEST(utTvKmlhellokmz   ${UTILITIES_TESTS}
        kmlhellokmz
        ${INPUTDATA}/multikml-doc.kmz
#        ${TEMP}/utkmlhellokmz.txt
        )

ADD_TEST(utTvKmlprettykml   ${UTILITIES_TESTS}
        --compare-ascii ${NOTOL}
                        ${BASELINE_FILES}/utkmlprettykml.txt
                        ${TEMP}/utkmlprettykml.txt
        kmlprettykml
        ${INPUTDATA}/polygons2d.kml
        ${TEMP}/utkmlprettykml.txt
        )

ADD_TEST(utTvKmlprintgeometry   ${UTILITIES_TESTS}
        kmlprintgeometry
        ${INPUTDATA}/polygons2d.kml
#        ${TEMP}/utkmlprintgeometry.txt
        )

# FIXME set inputs; this test has never been finished...
# ADD_TEST(ijObjectLabelAttributeValuesTest ${UTILITIES_TESTS}
#                 ijObjectLabelAttributeValuesTest
# )


# ------- GreyLevelRunLengthMatrixTextureCoefficientsCalculator from Insight Journal  --------------------
ADD_TEST(ijTuGreyLevelRunLengthMatrixTextureCoefficientsCalculator ${UTILITIES_TESTS}
          ijGreyLevelRunLengthMatrixTextureCoefficientsCalculatorTest
          ${INPUTDATA}/Textures/1.1.02.png )
          


# -------       Fichiers sources CXX -----------------------------------
SET(UtilitiesTests_SRCS
ossimIntegrationTest.cxx
ossimKeywordlistTest.cxx
ossimElevManagerTest.cxx
ossimXmlDocumentTest.cxx
ossimTileMapModelTest.cxx
ossimpluginsHermiteInterpolation.cxx
ossimpluginsHermiteInterpolationPlateformPosition.cxx
svmGenericKernelFunctor.cxx
svmTest.cxx
svmGenericKernelTest.cxx
6SFunctionMainTest.cxx
tinyXMLlibTest.cxx
svmGenericKernelBasicOperationsTest.cxx
otbSVMComposedKernelFunctorTest.cxx
ossimRadarSatSupport.cxx
itkScaleInvariantFeatureImageFilterTest.cxx
kmlcreatekml.cc
kmlhelloattrs.cc
kmlhelloenum.cc
kmlhellofolder.cc
kmlhellogeometry.cc
kmlhellohref.cc
kmlhellokmz.cc
kmlhelloregion.cc
kmlhelloworld.cc
kmlparsekml.cc
kmlprettykml.cc
kmlprintgeometry.cc
kmlhellofeatures.cc
kmlprint.cc
kmlchecklinks.cc
kmlcirclegen.cc
# kmlcountkml.cc
# kmlparsens.cc
kmlsharedstyles.cc
kmlsimplifylines.cc
kmlsortplacemarks.cc
ijObjectLabelAttributeValuesTest.cxx
ijGreyLevelRunLengthMatrixTextureCoefficientsCalculatorTest.cxx
)

IF(OTB_USE_SIFTFAST)
  SET(UtilitiesTests_SRCS ${UtilitiesTests_SRCS} siftfast.cpp)
ENDIF(OTB_USE_SIFTFAST)

IF(OTB_USE_JPEG2000)
#  SET(UtilitiesTests_SRCS ${UtilitiesTests_SRCS}  openJpegEncoder.cxx openJpegDecoder.cxx)

  IF (UNIX AND OPJ_MANGLE_PREFIX)
  # Rely on "nm" for this test, so enable it only on Unix platform  
  ADD_TEST(NAME utTvOpenJpegMangling
           COMMAND ${CMAKE_COMMAND}
                   -DOPJ_TARGET_PATH=$<TARGET_FILE:otbopenjpeg>
                   -DOPJ_MANGLE_PREFIX=${OPJ_MANGLE_PREFIX}
                   -P ${CMAKE_CURRENT_SOURCE_DIR}/TestOpenJPEGMangling.cmake)
  
  ENDIF()

ENDIF(OTB_USE_JPEG2000)

IF(NOT OTB_USE_EXTERNAL_OPENTHREADS)
  SET(UtilitiesTests_SRCS ${UtilitiesTests_SRCS}
        openthreadsThreadObserver.cpp
        openthreadsThreadReporter.cpp
        openthreadsWorkCrew.cpp
        openthreadsSimpleThreader.cpp )
ENDIF(NOT OTB_USE_EXTERNAL_OPENTHREADS)

IF(NOT OTB_USE_EXTERNAL_EXPAT)
  SET(UtilitiesTests_SRCS ${UtilitiesTests_SRCS}
        expatchardata.cxx
        expatminicheck.cxx
        expatruntests.cxx )
ENDIF(NOT OTB_USE_EXTERNAL_EXPAT)


# -------       Select sources files suppress warning  -----------------------------------
SET(UtilitiesTests_DisableWarning_SRCS
    openthreadsWorkCrew.cpp
    expatchardata.cxx
    expatminicheck.cxx
    expatruntests.cxx
)

IF(OTB_USE_SIFTFAST)
  SET(UtilitiesTests_DisableWarning_SRCS ${UtilitiesTests_DisableWarning_SRCS} siftfast.cpp)
ENDIF(OTB_USE_SIFTFAST)

IF(CMAKE_COMPILER_IS_GNUCXX)
      SET_SOURCE_FILES_PROPERTIES( ${UtilitiesTests_DisableWarning_SRCS} PROPERTIES COMPILE_FLAGS -w)
ELSE(CMAKE_COMPILER_IS_GNUCXX)
  IF(WIN32)
    IF (CMAKE_CXX_COMPILER MATCHES "^cl$")
        SET_SOURCE_FILES_PROPERTIES( ${UtilitiesTests_DisableWarning_SRCS} PROPERTIES COMPILE_FLAGS "/W0" )
    ENDIF (CMAKE_CXX_COMPILER MATCHES "^cl$")
  ENDIF(WIN32)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}")

IF(OTB_USE_EXTERNAL_OSSIM)
  INCLUDE_DIRECTORIES( ${OSSIM_INCLUDE_DIR} )
ELSE(OTB_USE_EXTERNAL_OSSIM)
  INCLUDE_DIRECTORIES( ${OTB_SOURCE_DIR}/Utilities/otbossim/include
                       ${OTB_BINARY_DIR}/Utilities/otbossim/include )
ENDIF(OTB_USE_EXTERNAL_OSSIM)

IF(NOT BUILD_SHARED_LIBS)
  ADD_DEFINITIONS(-DOPJ_STATIC)
ENDIF(NOT BUILD_SHARED_LIBS)

ADD_EXECUTABLE(otbUtilitiesTests otbUtilitiesTests.cxx ${UtilitiesTests_SRCS})
TARGET_LINK_LIBRARIES(otbUtilitiesTests OTBIO OTBCommon OTBTesting otbsvm otb6S ${TINYXML_LIBRARIES} ${LIBKML_LIBRARIES})

IF(OTB_USE_SIFTFAST)
  TARGET_LINK_LIBRARIES(otbUtilitiesTests otbsiftfast)
ENDIF(OTB_USE_SIFTFAST)

IF(OTB_USE_JPEG2000)
  TARGET_LINK_LIBRARIES(otbUtilitiesTests otbopenjpeg)
ENDIF(OTB_USE_JPEG2000)

IF(NOT OTB_USE_EXTERNAL_OPENTHREADS)
  TARGET_LINK_LIBRARIES(otbUtilitiesTests OpenThreads)
ENDIF(NOT OTB_USE_EXTERNAL_OPENTHREADS)

IF(NOT OTB_USE_EXTERNAL_EXPAT)
  TARGET_LINK_LIBRARIES(otbUtilitiesTests ${EXPAT_LIBRARIES})
ENDIF(NOT OTB_USE_EXTERNAL_EXPAT)

IF(UNIX)
    TARGET_LINK_LIBRARIES (otbUtilitiesTests m)
ENDIF(UNIX)

# Original 6S main executable in C
#     ADD_EXECUTABLE(otb6SMain 6SMainTest.cxx )
#     TARGET_LINK_LIBRARIES(otb6SMain OTBCommon otb6S)
#     IF(UNIX)
#         TARGET_LINK_LIBRARIES (otb6SMain m)
#     ENDIF(UNIX)

# OTB 6S main executable in C (call 6S main function.
#     ADD_EXECUTABLE(otb6SMainWithCallFunction 6SMainOtbWithCallFunctionTest.cxx )
#     TARGET_LINK_LIBRARIES(otb6SMainWithCallFunction OTBCommon otb6S)
#     IF(UNIX)
#         TARGET_LINK_LIBRARIES (otb6SMainWithCallFunction m)
#     ENDIF(UNIX)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
