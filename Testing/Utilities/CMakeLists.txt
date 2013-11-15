
if( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

set(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
set(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
set(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Remote sensing images (large images )
if(OTB_DATA_USE_LARGEINPUT)
  set(IMAGEDATA ${OTB_DATA_LARGEINPUT_ROOT} )
endif()

set(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Tolerance sur diff pixel image
set(NOTOL 0.0)
set(EPSILON_6 0.000001)
set(EPSILON_4 0.0001)

set(UTILITIES_TESTS ${CXX_TEST_PATH}/otbUtilitiesTests)


# Commentaire parce que trop long. Supprimer commentaire si besoin de tester integration lib Ossim
#add_test(utTvOssimIntegrationTest ${UTILITIES_TESTS}
#        ossimIntegrationTest
#        ossimTiffWriter
#        ${IMAGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
#        ${TEMP}/utOssimIntegration.tif
#)

if(OTB_DATA_USE_LARGEINPUT)

add_test(utTvOssimKeywordlistToulouseQuickBirdTest ${UTILITIES_TESTS}
--ignore-order --compare-ascii ${EPSILON_6}
               ${BASELINE_FILES}/utOssimKeywordlistToulouseQuickBird.txt
               ${TEMP}/utOssimKeywordlistToulouseQuickBird.txt
        ossimKeywordlistTest
        ${IMAGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/utOssimKeywordlistToulouseQuickBird.txt
        )

add_test(utTvOssimKeywordlistBlossevilleIkonosTest ${UTILITIES_TESTS}
--ignore-order --compare-ascii ${NOTOL}
                ${BASELINE_FILES}/utOssimKeywordlistBlossevilleIkonos.txt
                ${TEMP}/utOssimKeywordlistBlossevilleIkonos.txt
        ossimKeywordlistTest
        ${IMAGEDATA}/IKONOS/BLOSSEVILLE/po_2619900_grn_0000000.tif
        ${TEMP}/utOssimKeywordlistBlossevilleIkonos.txt
        )
endif()

add_test(utTuOssimElevManagerTest ${UTILITIES_TESTS}
        ossimElevManagerTest
        )

if(OTB_DATA_USE_LARGEINPUT)
add_test(utTvOssimXmlDocumentTest ${UTILITIES_TESTS}
        ossimXmlDocumentTest
        ${IMAGEDATA}/TERRASARX/TORONTO/TSX1_SAR__SSC/TSX1_SAR__SSC.xml
        )
endif()

add_test(utTvOssimTileMapModelTestToulouse ${UTILITIES_TESTS}
        --compare-ascii ${NOTOL}
                ${BASELINE_FILES}/utOssimTileMapModelToulouse.txt
                ${TEMP}/utOssimTileMapModelToulouse.txt
        ossimTileMapModelTest
        43.6044
        1.44295
        ${TEMP}/utOssimTileMapModelToulouse.txt
        )

add_test(utTvOssimpluginsHermiteInterpolationTest ${UTILITIES_TESTS}
     ossimpluginsHermiteInterpolationTest)

if(OTB_DATA_USE_LARGEINPUT)
add_test(utTvOssimpluginsHermiteInterpolationPlateformPositionRadarSat2Test ${UTILITIES_TESTS}
        ossimpluginsHermiteInterpolationPlateformPositionTest
        ${IMAGEDATA}/RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/product.xml
    )
endif()


# -------            lib otbsvm   ------------------------------

add_test(utTuSvmKernelFunctorTest ${UTILITIES_TESTS}
                svmGenericKernelFunctor
)

add_test(utTvSvmTest ${UTILITIES_TESTS}
--compare-ascii ${NOTOL}  ${INPUTDATA}/svm_model
                ${TEMP}/svm_model_test
                svmTest
                ${INPUTDATA}/svm_model
                ${TEMP}/svm_model_test
)

add_test(utTvSvmTestNewVersionFileGenericModelTest ${UTILITIES_TESTS}
--compare-ascii ${NOTOL}  ${BASELINE_FILES}/utTvsvm_model_image_generic_version_test
                ${TEMP}/utTvsvm_model_image_generic_version_test
                svmTest
                ${INPUTDATA}/svm_model_image_generic_version
                ${TEMP}/utTvsvm_model_image_generic_version_test
)

add_test(utTvSvmTestGenericModelTest ${UTILITIES_TESTS}
--compare-ascii ${NOTOL}  ${INPUTDATA}/svm_model_image_generic_kernel
                ${TEMP}/svm_model_image_generic_kernel_test
                svmGenericKernelTest
                ${INPUTDATA}/svm_model_image_generic_kernel
                ${TEMP}/svm_model_image_generic_kernel_test
)

add_test(utTvSvmGenericKernelBasicOperationsTest ${UTILITIES_TESTS}
--compare-ascii ${NOTOL}  ${BASELINE_FILES}/utTvsvmGenericKernelBasicOperationsTest.txt
                         ${TEMP}/svmGenericKernelBasicOperationsTest.txt
              svmGenericKernelBasicOperationsTest
                 ${TEMP}/svmGenericKernelBasicOperationsTest.txt
)

add_test(utTvSvmComposedKernelFunctorTest ${UTILITIES_TESTS}
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
#add_test(utTv6SMainTest ${UTILITIES_TESTS}
#                SIXSMainTest
#                ${INPUTDATA}/132.149.107.66.wloip20967.INP
#)

add_test(utTv6SFunctionMainTest ${UTILITIES_TESTS}
--compare-ascii ${EPSILON_4}  ${BASELINE_FILES}/ut6S_function_main_test.txt
                ${TEMP}/ut6S_function_main_test.txt
                SIXSFunctionMainOtbTest
                ${INPUTDATA}/in6S_otb
                ${TEMP}/ut6S_function_main_test.txt
)

# -------            lib tinyXMLlib   ------------------------------
add_test(utTvTinyXMLTest ${UTILITIES_TESTS}
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

if(OTB_DATA_USE_LARGEINPUT)
add_test(utTvOssimRadarSatSupport ${UTILITIES_TESTS}
        ossimRadarSatSupport
        ${IMAGEDATA}/RADARSAT1/GOMA2/SCENE01/DAT_01.001
        )
endif()


# -------            lib OpenJpeg   ------------------------------
# Case for release 2.2.1
#if(OTB_USE_JPEG2000)
#add_test(utTvOpenJpegEncoder ${UTILITIES_TESTS}
#        openJpegEncoder
#        ${TEMP}/testjpeg.j2k
#)
#
#add_test(utTvOpenJpegDecoder ${UTILITIES_TESTS}
#        openJpegDecoder
#        ${TEMP}/testjpeg.j2k
#)
#set_tests_properties(utTvOpenJpegDecoder PROPERTIES DEPENDS utTvOpenJpegEncoder)
#endif()

# -------            lib expat  ------------------------------
if(NOT OTB_USE_EXTERNAL_EXPAT)
add_test(utTvExpatRunTests ${UTILITIES_TESTS}
        expatruntests
        -q)
endif()

# -------            lib OpenThreads  ------------------------------
if(NOT OTB_USE_EXTERNAL_OPENTHREADS)
  add_test(utTvOpenthreadsSimpleThreader ${UTILITIES_TESTS}
        openthreadsSimpleThreader
        2 # [NUM_ELTS]
        1 # [NUM_THREADS]
        )
  add_test(utTvOpenthreadsWorkCrew ${UTILITIES_TESTS}
        openthreadsWorkCrew
        1 # [NUM_THREADS]
        )
endif()

# -------            SIFT   -------------------------------------
add_test(utTv2DSiftImageSquareSyntheticRotate10 ${UTILITIES_TESTS}
         itk2DScaleInvariantFeatureImageFilterTest
         ${INPUTDATA}/carre.png
         ${TEMP}/itk2DScaleInvariantFeatureImageFilter_carre1.png
         ${TEMP}/itk2DScaleInvariantFeatureImageFilter_carre2.png
         ${TEMP}/itk2DScaleInvariantFeatureImageFilter_carre_keys1.png
         ${TEMP}/itk2DScaleInvariantFeatureImageFilter_carre_keys2.png
         1 10 0.5 0  )

add_test(utTv2DSiftImageGridSyntheticRotate10 ${UTILITIES_TESTS}
         itk2DScaleInvariantFeatureImageFilterTest
         ${INPUTDATA}/damier.png
         ${TEMP}/itk2DScaleInvariantFeatureImageFilter_damier1.png
         ${TEMP}/itk2DScaleInvariantFeatureImageFilter_damier2.png
         ${TEMP}/itk2DScaleInvariantFeatureImageFilter_damier_keys1.png
         ${TEMP}/itk2DScaleInvariantFeatureImageFilter_damier_keys2.png
         1 10 0.5 0 )

add_test(utTv2DSiftImageSceneSyntheticRotate10 ${UTILITIES_TESTS}
         itk2DScaleInvariantFeatureImageFilterTest
         ${INPUTDATA}/scene.png
         ${TEMP}/itk2DScaleInvariantFeatureImageFilter_scene1.png
         ${TEMP}/itk2DScaleInvariantFeatureImageFilter_scene2.png
         ${TEMP}/itk2DScaleInvariantFeatureImageFilter_scene_keys1.png
         ${TEMP}/itk2DScaleInvariantFeatureImageFilter_scene_keys2.png
         1 10 0.5 0 )

# -------            lib Kml   ------------------------------
foreach(id_test createkml helloattrs helloenum hellofolder hellogeometry helloregion helloworld parsekml )
        add_test(utTuKml${id_test} ${UTILITIES_TESTS}
        kml${id_test}
        ${TEMP}/utkml${id_test}.txt
        )
endforeach()


add_test(utTvKmlhellofeatures   ${UTILITIES_TESTS}
        kmlhellofeatures
        ${INPUTDATA}/MODIS20040214212340.kml #abstractview.kml
#        ${TEMP}/utkmlhellofeatures.txt
        )

add_test(utTvKmlhellohref   ${UTILITIES_TESTS}
        kmlhellohref
        ${INPUTDATA}/abstractview.kml
#        ${TEMP}/utkmlhellohref.txt
        )
add_test(utTvKmlhellokmz   ${UTILITIES_TESTS}
        kmlhellokmz
        ${INPUTDATA}/multikml-doc.kmz
#        ${TEMP}/utkmlhellokmz.txt
        )

add_test(utTvKmlprettykml   ${UTILITIES_TESTS}
        --compare-ascii ${NOTOL}
                        ${BASELINE_FILES}/utkmlprettykml.txt
                        ${TEMP}/utkmlprettykml.txt
        kmlprettykml
        ${INPUTDATA}/polygons2d.kml
        ${TEMP}/utkmlprettykml.txt
        )

add_test(utTvKmlprintgeometry   ${UTILITIES_TESTS}
        kmlprintgeometry
        ${INPUTDATA}/polygons2d.kml
#        ${TEMP}/utkmlprintgeometry.txt
        )

# FIXME set inputs; this test has never been finished...
# add_test(ijObjectLabelAttributeValuesTest ${UTILITIES_TESTS}
#                 ijObjectLabelAttributeValuesTest
# )


# -------       Fichiers sources CXX -----------------------------------
set(UtilitiesTests_SRCS
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
#FIXME itkv4
#ijObjectLabelAttributeValuesTest.cxx
)

if(OTB_USE_SIFTFAST)
  set(UtilitiesTests_SRCS ${UtilitiesTests_SRCS} siftfast.cpp)
endif()

if(OTB_USE_JPEG2000)
#  set(UtilitiesTests_SRCS ${UtilitiesTests_SRCS}  openJpegEncoder.cxx openJpegDecoder.cxx)

  if(UNIX AND OPJ_MANGLE_PREFIX)
  # Rely on "nm" for this test, so enable it only on Unix platform
  add_test(NAME utTvOpenJpegMangling
           COMMAND ${CMAKE_COMMAND}
                   -DOPJ_TARGET_PATH=$<TARGET_FILE:otbopenjpeg>
                   -DOPJ_MANGLE_PREFIX=${OPJ_MANGLE_PREFIX}
                   -P ${CMAKE_CURRENT_SOURCE_DIR}/TestOpenJPEGMangling.cmake)

  endif()

endif()

if(NOT OTB_USE_EXTERNAL_OPENTHREADS)
  set(UtilitiesTests_SRCS ${UtilitiesTests_SRCS}
        openthreadsThreadObserver.cpp
        openthreadsThreadReporter.cpp
        openthreadsWorkCrew.cpp
        openthreadsSimpleThreader.cpp )
endif()

if(NOT OTB_USE_EXTERNAL_EXPAT)
  set(UtilitiesTests_SRCS ${UtilitiesTests_SRCS}
        expatchardata.cxx
        expatminicheck.cxx
        expatruntests.cxx )
endif()


# -------       Select sources files suppress warning  -----------------------------------
set(UtilitiesTests_DisableWarning_SRCS
    openthreadsWorkCrew.cpp
    expatchardata.cxx
    expatminicheck.cxx
    expatruntests.cxx
)

if(OTB_USE_SIFTFAST)
  set(UtilitiesTests_DisableWarning_SRCS ${UtilitiesTests_DisableWarning_SRCS} siftfast.cpp)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
      set_source_files_properties( ${UtilitiesTests_DisableWarning_SRCS} PROPERTIES COMPILE_FLAGS -w)
else()
  if(WIN32)
    if(CMAKE_CXX_COMPILER MATCHES "^cl$")
        set_source_files_properties( ${UtilitiesTests_DisableWarning_SRCS} PROPERTIES COMPILE_FLAGS "/W0" )
    endif()
  endif()
endif()

include_directories("${OTBTesting_BINARY_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

if(OTB_USE_EXTERNAL_OSSIM)
  include_directories( ${OSSIM_INCLUDE_DIR} )
else()
  include_directories( ${OTB_SOURCE_DIR}/Utilities/otbossim/include
                       ${OTB_BINARY_DIR}/Utilities/otbossim/include )
endif()

if(NOT BUILD_SHARED_LIBS)
  add_definitions(-DOPJ_STATIC)
endif()

add_executable(otbUtilitiesTests otbUtilitiesTests.cxx ${UtilitiesTests_SRCS})
target_link_libraries(otbUtilitiesTests OTBIO OTBCommon OTBTesting otbsvm otb6S ${TINYXML_LIBRARIES} ${LIBKML_LIBRARIES})

if(OTB_USE_SIFTFAST)
  target_link_libraries(otbUtilitiesTests otbsiftfast)
endif()

if(OTB_USE_JPEG2000)
  target_link_libraries(otbUtilitiesTests otbopenjpeg)
endif()

if(NOT OTB_USE_EXTERNAL_OPENTHREADS)
  target_link_libraries(otbUtilitiesTests OpenThreads)
endif()

if(NOT OTB_USE_EXTERNAL_EXPAT)
  target_link_libraries(otbUtilitiesTests ${EXPAT_LIBRARIES})
endif()

if(UNIX)
    target_link_libraries(otbUtilitiesTests m)
endif()

# Original 6S main executable in C
#     add_executable(otb6SMain 6SMainTest.cxx )
#     target_link_libraries(otb6SMain OTBCommon otb6S)
#     if(UNIX)
#         target_link_libraries(otb6SMain m)
#     endif()

# OTB 6S main executable in C (call 6S main function.
#     add_executable(otb6SMainWithCallFunction 6SMainOtbWithCallFunctionTest.cxx )
#     target_link_libraries(otb6SMainWithCallFunction OTBCommon otb6S)
#     if(UNIX)
#         target_link_libraries(otb6SMainWithCallFunction m)
#     endif()

endif()
