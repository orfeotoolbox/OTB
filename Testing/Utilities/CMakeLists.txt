 
IF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/OTB/Images)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB/Files)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
#Images de teledetection (grosses images )
SET(IMAGEDATA ${OTB_DATA_ROOT}/LargeInput )
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

#Tolerance sur diff pixel image
SET(TOL 0.0)
SET(EPSILON 0.00000000001)
SET(EPSILON_10e6 0.000001)


SET(UTILITIES_TESTS ${CXX_TEST_PATH}/otbUtilitiesTests)


# Commentaire parce que trop long. Supprimer commentaire si besoin de tester integration lib Ossim
#ADD_TEST(utOssimIntegrationTest ${UTILITIES_TESTS}
#        ossimIntegrationTest
#        ossimTiffWriter
#        ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
#        ${TEMP}/utOssimIntegration.tif
#)

ADD_TEST(utOssimKeywordlistToulouseQuickBirdTest ${UTILITIES_TESTS}
--compare-ascii ${EPSILON_10e6}  ${BASELINE_FILES}/utOssimKeywordlistToulouseQuickBird.txt
                        ${TEMP}/utOssimKeywordlistToulouseQuickBird.txt
        ossimKeywordlistTest
        ${IMAGEDATA}/TOULOUSE/QuickBird/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
        ${TEMP}/utOssimKeywordlistToulouseQuickBird.txt
        )

ADD_TEST(utOssimKeywordlistBlossevilleIkonosTest ${UTILITIES_TESTS}
--compare-ascii ${TOL}  ${BASELINE_FILES}/utOssimKeywordlistBlossevilleIkonos.txt
                        ${TEMP}/utOssimKeywordlistBlossevilleIkonos.txt
        ossimKeywordlistTest
        ${IMAGEDATA}/IKONOS_BLOSSEVILLE/po_2619900_grn_0000000.tif
        ${TEMP}/utOssimKeywordlistBlossevilleIkonos.txt
        )

ADD_TEST(utOssimElevManagerTest ${UTILITIES_TESTS}
        ossimElevManagerTest
        )


ADD_TEST(utIjBSplineScatteredDataPointSetToImageFilterTest ${UTILITIES_TESTS}
--compare-image ${EPSILON}
		${BASELINE}/ijBSplineScatteredDataPointSetToImageFilterTestOutput.hdr
		${TEMP}/ijBSplineScatteredDataPointSetToImageFilterTestOutput.hdr
		ijBSplineScatteredDataPointSetToImageFilterTest
		${INPUTDATA}/brain.png
		${TEMP}/ijBSplineScatteredDataPointSetToImageFilterTestOutput.hdr
)

# -------            lib otbsvm   ------------------------------

ADD_TEST(utTuSvmKernelFunctorTest ${UTILITIES_TESTS}
                svmGenericKernelFunctor
)

ADD_TEST(utTvSvmTest ${UTILITIES_TESTS}
--compare-ascii ${TOL}  ${INPUTDATA}/svm_model
                ${TEMP}/svm_model_test
                svmTest
                ${INPUTDATA}/svm_model
                ${TEMP}/svm_model_test
)

ADD_TEST(utTvSvmTestNewVersionFileGenericModelTest ${UTILITIES_TESTS}
--compare-ascii ${TOL}  ${BASELINE_FILES}/utTvsvm_model_image_generic_version_test
                ${TEMP}/utTvsvm_model_image_generic_version_test
                svmTest
                ${INPUTDATA}/svm_model_image_generic_version
                ${TEMP}/utTvsvm_model_image_generic_version_test
)

ADD_TEST(utTvSvmTestGenericModelTest ${UTILITIES_TESTS}
--compare-ascii ${TOL}  ${INPUTDATA}/svm_model_image_generic_kernel
                ${TEMP}/svm_model_image_generic_kernel_test
                svmGenericKernelTest
                ${INPUTDATA}/svm_model_image_generic_kernel
                ${TEMP}/svm_model_image_generic_kernel_test
)

ADD_TEST(utTvSvmGenericKernelBasicOperationsTest ${UTILITIES_TESTS}
--compare-ascii ${TOL}  ${BASELINE_FILES}/utTvsvmGenericKernelBasicOperationsTest.txt
                	 ${TEMP}/svmGenericKernelBasicOperationsTest.txt
              svmGenericKernelBasicOperationsTest
		 ${TEMP}/svmGenericKernelBasicOperationsTest.txt
)

ADD_TEST(utTvSvmComposedKernelFunctorTest ${UTILITIES_TESTS}
--compare-n-ascii ${TOL} 3 
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
#--compare-ascii ${EPSILON_10e6}  ${BASELINE_FILES}/ut6S_function_main_test.txt
--compare-ascii ${TOL}  ${BASELINE_FILES}/ut6S_function_main_test.txt
                ${TEMP}/ut6S_function_main_test.txt
                SIXSFunctionMainOtbTest
                ${INPUTDATA}/in6S_otb
                ${TEMP}/ut6S_function_main_test.txt
)

# -------            lib tinyXMLlib   ------------------------------
ADD_TEST(utTvTinyXMLTest ${UTILITIES_TESTS}
        --compare-ascii ${TOL}  
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


ADD_TEST(utOssimRadarSatSupport ${UTILITIES_TESTS}
        ossimRadarSatSupport
        ${IMAGEDATA}/RADARSAT2/SCENE01/DAT_01.001
        )

# -------            lib Galib   ------------------------------

#                   FOREACH(num 1 12 14 16 17 25 27)
#                   ADD_TEST( utTuGalibTest${num}  ${CXX_TEST_PATH}/otbGalibTests${num}   
#                           galibTests${num}
#                           seed 555 )
#                   ENDFOREACH(num)

#                   FOREACH(num 6 8 19 20)
#                           ADD_TEST( utTvGalibTest${num}  ${CXX_TEST_PATH}/otbGalibTests${num}   
#                           --compare-ascii ${TOL}  
#                                           ${BASELINE_FILES}/utGalibTest${num}_bog.dat 
#                                           ${TEMP}/utGalibTest${num}_bog.dat
#                           galibTests${num}
#                           score_filename ${TEMP}/utGalibTest${num}_bog.dat 
#                           seed 555 )
#                   ENDFOREACH(num)
#                   FOREACH(num 2 4 9 11 15)
#                   ADD_TEST( utTvGalibTest${num}  ${CXX_TEST_PATH}/otbGalibTests${num}   
#                           --compare-ascii ${TOL}  
#                                           ${BASELINE_FILES}/utGalibTest${num}_bog.dat 
#                                           ${TEMP}/utGalibTest${num}_bog.dat
#                           galibTests${num}
#                           ${TEMP}/utGalibTest${num}_bog.dat 
#                           seed 555 )
#                   ENDFOREACH(num)



#                   SET(num 13)
#                   ADD_TEST( utTvGalibTest${num}  ${CXX_TEST_PATH}/otbGalibTests${num}   
#                           --compare-ascii ${TOL}  
#                                           ${BASELINE_FILES}/utGalibTest${num}_bog.dat 
#                                           ${TEMP}/utGalibTest${num}_bog.dat
#                           galibTests${num}
#                           ${INPUTDATA}/galib-smiley.txt
#                           score_filename ${TEMP}/utGalibTest${num}_bog.dat 
#                           seed 555 )

#                   FOREACH(num 3 18)
#                   ADD_TEST( utTvGalibTest${num}  ${CXX_TEST_PATH}/otbGalibTests${num}   
#                           --compare-ascii ${TOL}  
#                                           ${BASELINE_FILES}/utGalibTest${num}_bog.dat 
#                                           ${TEMP}/utGalibTest${num}_bog.dat
#                                   
#                           galibTests${num}
#                           file ${INPUTDATA}/galib-smiley.txt
#                           score_filename ${TEMP}/utGalibTest${num}_bog.dat 
#                           seed 555 )
#                   ENDFOREACH(num)

#                   SET(num 5)
#                   ADD_TEST( utTvGalibTest${num}  ${CXX_TEST_PATH}/otbGalibTests${num}  
#                           --compare-ascii ${TOL}  
#                                           ${BASELINE_FILES}/utGalibTest${num}_bog.dat 
#                                           ${TEMP}/utGalibTest${num}_bog.dat
#                            galibTests${num}
#                            score_filename ${TEMP}/utGalibTest${num}_bog.dat 
#                            graph ${INPUTDATA}/galib-smiley.txt
#                            values ${INPUTDATA}/galib-values.txt
#                            seed 555 )

# Test 7 commented becasue impossible to specific he bog.dat file name in the process ;-(
#SET(num 7)
#        ADD_TEST( utTvGalibTest${num}  ${CXX_TEST_PATH}/otbGalibTests${num}   
#        --compare-ascii ${TOL}  
#                        ${BASELINE_FILES}/utGalibTest${num}_bog.dat 
#                        ${TEMP}/utGalibTest${num}_bog.dat
#        galibTests${num}
#        ${TEMP}/utGalibTest${num}_bog.dat 
#        dfile ${INPUTDATA}/galib-smiley.txt
#        pfile ${INPUTDATA}/galib-settings.txt
#        seed 555 )


#                   SET(num 10)
#                   ADD_TEST( utTvGalibTest${num}  ${CXX_TEST_PATH}/otbGalibTests${num}   
#                           --compare-n-ascii ${TOL} 4 
#                                           ${BASELINE_FILES}/utGalibTest${num}_sinusoid.dat 
#                                           ${TEMP}/utGalibTest${num}_sinusoid.dat 
#                                           ${BASELINE_FILES}/utGalibTest${num}_pop.nospec.dat 
#                                           ${TEMP}/utGalibTest${num}_pop.nospec.dat 
#                                           ${BASELINE_FILES}/utGalibTest${num}_pop.genespec.dat 
#                                           ${TEMP}/utGalibTest${num}_pop.genespec.dat 
#                                           ${BASELINE_FILES}/utGalibTest${num}_pop.phenespec.dat 
#                                           ${TEMP}/utGalibTest${num}_pop.phenespec.dat 
#                           galibTests${num}
#                           ${TEMP}/utGalibTest${num}_sinusoid.dat 
#                           ${TEMP}/utGalibTest${num}_pop.nospec.dat 
#                           ${TEMP}/utGalibTest${num}_pop.genespec.dat 
#                           ${TEMP}/utGalibTest${num}_pop.phenespec.dat 
#                           seed 555 )

#                   SET(num 21)
#                   ADD_TEST( utTvGalibTest${num}  ${CXX_TEST_PATH}/otbGalibTests${num}   
#                           --compare-n-ascii ${TOL} 5
#                                           ${BASELINE_FILES}/utGalibTest${num}_bog1.dat 
#                                           ${TEMP}/utGalibTest${num}_bog1.dat 
#                                           ${BASELINE_FILES}/utGalibTest${num}_bog2.dat 
#                                           ${TEMP}/utGalibTest${num}_bog2.dat 
#                                           ${BASELINE_FILES}/utGalibTest${num}_bog2a.dat 
#                                           ${TEMP}/utGalibTest${num}_bog2a.dat 
#                                           ${BASELINE_FILES}/utGalibTest${num}_bog3.dat 
#                                           ${TEMP}/utGalibTest${num}_bog3.dat 
#                                           ${BASELINE_FILES}/utGalibTest${num}_bog4.dat 
#                                           ${TEMP}/utGalibTest${num}_bog4.dat 
#                           galibTests${num}
#                           ${TEMP}/utGalibTest${num}_bog1.dat 
#                           ${TEMP}/utGalibTest${num}_bog2.dat 
#                           ${TEMP}/utGalibTest${num}_bog2a.dat 
#                           ${TEMP}/utGalibTest${num}_bog3.dat 
#                           ${TEMP}/utGalibTest${num}_bog4.dat 
#                           seed 555 )


#                   SET(num 22)
#                   ADD_TEST( utTvGalibTest${num}  ${CXX_TEST_PATH}/otbGalibTests${num}   
#                           --compare-n-ascii ${TOL} 4
#                                           ${BASELINE_FILES}/utGalibTest${num}_sinusoid.dat 
#                                           ${TEMP}/utGalibTest${num}_sinusoid.dat 
#                                           ${BASELINE_FILES}/utGalibTest${num}_pop.initial.dat 
#                                           ${TEMP}/utGalibTest${num}_pop.initial.dat 
#                                           ${BASELINE_FILES}/utGalibTest${num}_pop.final.dat 
#                                           ${TEMP}/utGalibTest${num}_pop.final.dat 
#                                           ${BASELINE_FILES}/utGalibTest${num}_blog.dat 
#                                           ${TEMP}/utGalibTest${num}_blog.dat 
#                           galibTests${num}
#                           ${TEMP}/utGalibTest${num}_sinusoid.dat 
#                           ${TEMP}/utGalibTest${num}_pop.initial.dat 
#                           ${TEMP}/utGalibTest${num}_pop.final.dat 
#                           score_filename ${TEMP}/utGalibTest${num}_blog.dat 
#                           seed 555 )

#                   SET(num 23)
#                   ADD_TEST( utTvGalibTest${num}  ${CXX_TEST_PATH}/otbGalibTests${num}   
#                           --compare-n-ascii ${TOL} 4
#                                           ${BASELINE_FILES}/utGalibTest${num}_popi.dat 
#                                           ${TEMP}/utGalibTest${num}_popi.dat 
#                                           ${BASELINE_FILES}/utGalibTest${num}_popf.dat 
#                                           ${TEMP}/utGalibTest${num}_popf.dat 
#                                           ${BASELINE_FILES}/utGalibTest${num}_sinusoid.dat 
#                                           ${TEMP}/utGalibTest${num}_sinusoid.dat 
#                                           ${BASELINE_FILES}/utGalibTest${num}_blog.dat 
#                                           ${TEMP}/utGalibTest${num}_blog.dat 
#                           galibTests${num}
#                           ${TEMP}/utGalibTest${num}_popi.dat 
#                           ${TEMP}/utGalibTest${num}_popf.dat 
#                           ${TEMP}/utGalibTest${num}_sinusoid.dat 
#                           score_filename ${TEMP}/utGalibTest${num}_blog.dat 
#                           seed 555 )

#                   SET(num 24)
#                   ADD_TEST( utTvGalibTest${num}  ${CXX_TEST_PATH}/otbGalibTests${num} 
#                           --compare-n-ascii ${TOL} 3
#                                           ${BASELINE_FILES}/utGalibTest${num}_population.dat 
#                                           ${TEMP}/utGalibTest${num}_population.dat 
#                                           ${BASELINE_FILES}/utGalibTest${num}_sinusoid.dat 
#                                           ${TEMP}/utGalibTest${num}_sinusoid.dat 
#                                           ${BASELINE_FILES}/utGalibTest${num}_blog.dat 
#                                           ${TEMP}/utGalibTest${num}_blog.dat 
#                           galibTests${num}
#                           ${INPUTDATA}/galib-settings.txt
#                           ${TEMP}/utGalibTest${num}_population.dat 
#                           ${TEMP}/utGalibTest${num}_sinusoid.dat 
#                           score_filename ${TEMP}/utGalibTest${num}_blog.dat 
#                           seed 555 )

#                   SET(num 26)
#                   ADD_TEST( utTvGalibTest${num}  ${CXX_TEST_PATH}/otbGalibTests${num}   
#                           galibTests${num}
#                           ${INPUTDATA}/galib-tsp_rect_20.txt 
#                           seed 555 )



# -------            lib OpenJpeg   ------------------------------
# Case for release 2.2.1
IF(OTB_COMPILE_JPEG2000)
ADD_TEST(utOpenJpegEncoder ${UTILITIES_TESTS}
	openJpegEncoder)

ADD_TEST(utOpenJpegDecoder ${UTILITIES_TESTS}
	openJpegDecoder)
ENDIF(OTB_COMPILE_JPEG2000)

# -------            lib expat  ------------------------------
IF(NOT OTB_USE_EXTERNAL_EXPAT)
ADD_TEST(utExpatRunTests ${UTILITIES_TESTS}
        expatruntests
        -q)
ENDIF(NOT OTB_USE_EXTERNAL_EXPAT)

# -------            lib OpenThreads  ------------------------------
IF(NOT OTB_USE_EXTERNAL_OPENTHREADS)
  ADD_TEST(utOpenthreadsSimpleThreader ${UTILITIES_TESTS}
	openthreadsSimpleThreader
	2 # [NUM_ELTS] 
	1 # [NUM_THREADS]
	)
  ADD_TEST(utOpenthreadsWorkCrew ${UTILITIES_TESTS}
	openthreadsWorkCrew
	1 # [NUM_THREADS]
	)
ENDIF(NOT OTB_USE_EXTERNAL_OPENTHREADS)

# -------            SIFT   -------------------------------------
ADD_TEST(ut2DSiftImageSquareSyntheticRotate10 ${UTILITIES_TESTS}
	 itk2DScaleInvariantFeatureImageFilterTest
	 ${INPUTDATA}/carre.png
         ${TEMP}/itk2DScaleInvariantFeatureImageFilter_carre1.png
	 ${TEMP}/itk2DScaleInvariantFeatureImageFilter_carre2.png
	 ${TEMP}/itk2DScaleInvariantFeatureImageFilter_carre_keys1.png
	 ${TEMP}/itk2DScaleInvariantFeatureImageFilter_carre_keys2.png
	 1 10 0.5 0  )

ADD_TEST(ut2DSiftImageGridSyntheticRotate10 ${UTILITIES_TESTS}
	 itk2DScaleInvariantFeatureImageFilterTest
	 ${INPUTDATA}/damier.png 
	 ${TEMP}/itk2DScaleInvariantFeatureImageFilter_damier1.png
	 ${TEMP}/itk2DScaleInvariantFeatureImageFilter_damier2.png
	 ${TEMP}/itk2DScaleInvariantFeatureImageFilter_damier_keys1.png
	 ${TEMP}/itk2DScaleInvariantFeatureImageFilter_damier_keys2.png
	 1 10 0.5 0 )

ADD_TEST(ut2DSiftImageSceneSyntheticRotate10 ${UTILITIES_TESTS}
	 itk2DScaleInvariantFeatureImageFilterTest
	 ${INPUTDATA}/scene.png
	 ${TEMP}/itk2DScaleInvariantFeatureImageFilter_scene1.png
	 ${TEMP}/itk2DScaleInvariantFeatureImageFilter_scene2.png
	 ${TEMP}/itk2DScaleInvariantFeatureImageFilter_scene_keys1.png
	 ${TEMP}/itk2DScaleInvariantFeatureImageFilter_scene_keys2.png
	 1 10 0.5 0 )

# -------            lib Kml   ------------------------------
#FOREACH(id_test createkml helloattrs helloenum hellofolder hellogeometry helloregion helloserializer helloworld parsekml      )
#        ADD_TEST( utKml${id_test} ${CXX_TEST_PATH}/otbKmlTests${id_test}   
#        kml${id_test}
#        ${TEMP}/utkml${id_test}.txt
#        )
#ENDFOREACH(id_test)


#ADD_TEST( utKmlhellofeatures  ${CXX_TEST_PATH}/otbKmlTestshellofeatures
#        kmlhellofeatures
#        ${INPUTDATA}/abstractview.kml
#        ${TEMP}/utkmlhellofeatures.txt
#        )

#ADD_TEST( utKmlhellohref  ${CXX_TEST_PATH}/otbKmlTestshellohref
#        kmlhellohref
#        ${INPUTDATA}/abstractview.kml
#        ${TEMP}/utkmlhellohref.txt
#        )
#ADD_TEST( utKmlhellokmz  ${CXX_TEST_PATH}/otbKmlTestshellokmz    
#        kmlhellokmz
#        ${INPUTDATA}/multikml-doc.kmz
#        ${TEMP}/utkmlhellokmz.txt
#        )

#ADD_TEST( utKmlprettykml  ${CXX_TEST_PATH}/otbKmlTestsprettykml
#        kmlprettykml
#        ${INPUTDATA}/polygons2d.kml
#        ${TEMP}/utkmlprettykml.txt
#        )
        
#ADD_TEST( utKmlprintgeometry  ${CXX_TEST_PATH}/otbKmlTestsprintgeometry
#        kmlprintgeometry
#        ${INPUTDATA}/polygons2d.kml
#        ${TEMP}/utkmlprintgeometry.txt
#        )


# -------       Fichiers sources CXX -----------------------------------
SET(UtilitiesTests_SRCS
ossimIntegrationTest.cxx
ossimKeywordlistTest.cxx
ossimElevManagerTest.cxx
ijBSplineScatteredDataPointSetToImageFilterTest.cxx
svmGenericKernelFunctor.cxx
svmTest.cxx
svmGenericKernelTest.cxx
6SFunctionMainTest.cxx
tinyXMLlibTest.cxx
svmGenericKernelBasicOperationsTest.cxx
otbSVMComposedKernelFunctorTest.cxx
ossimRadarSatSupport.cxx
itkScaleInvariantFeatureImageFilterTest.cxx
)

IF(OTB_COMPILE_JPEG2000)
  SET(UtilitiesTests_SRCS ${UtilitiesTests_SRCS}  openJpegEncoder.cxx openJpegDecoder.cxx)
ENDIF(OTB_COMPILE_JPEG2000)

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
  IF(CMAKE_COMPILER_IS_GNUCXX)
        SET_SOURCE_FILES_PROPERTIES( expatchardata.cxx expatminicheck.cxx expatruntests.cxx PROPERTIES COMPILE_FLAGS -w )
  ENDIF(CMAKE_COMPILER_IS_GNUCXX)

ENDIF(NOT OTB_USE_EXTERNAL_EXPAT)


INCLUDE_DIRECTORIES("${OTBTesting_BINARY_DIR}")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}")


IF(NOT BUILD_SHARED_LIBS)
  ADD_DEFINITIONS(-DOPJ_STATIC)
ENDIF(NOT BUILD_SHARED_LIBS)

ADD_EXECUTABLE(otbUtilitiesTests otbUtilitiesTests.cxx ${UtilitiesTests_SRCS})
TARGET_LINK_LIBRARIES(otbUtilitiesTests OTBIO OTBCommon gdal ITKIO ITKAlgorithms ITKStatistics ITKCommon otbossim otbsvm otb6S tinyXML )

IF(OTB_COMPILE_JPEG2000)
  TARGET_LINK_LIBRARIES(otbUtilitiesTests otbopenjpeg)
ENDIF(OTB_COMPILE_JPEG2000)

IF(NOT OTB_USE_EXTERNAL_OPENTHREADS)
  TARGET_LINK_LIBRARIES(otbUtilitiesTests OpenThreads)
ENDIF(NOT OTB_USE_EXTERNAL_OPENTHREADS)

IF(NOT OTB_USE_EXTERNAL_EXPAT)
  TARGET_LINK_LIBRARIES(otbUtilitiesTests otbexpat)
ENDIF(NOT OTB_USE_EXTERNAL_EXPAT)

IF(UNIX)
    TARGET_LINK_LIBRARIES (otbUtilitiesTests m)
ENDIF(UNIX)


# -------       CXX and EXECUTABLES files for GALIB Tests -----------------------------------
#FOREACH(loop_var RANGE 1 27 1)
#        ADD_EXECUTABLE(otbGalibTests${loop_var} galibTests${loop_var}.cxx)
#        IF(UNIX)
#                TARGET_LINK_LIBRARIES (otbGalibTests${loop_var} m)
#        ENDIF(UNIX)
#        TARGET_LINK_LIBRARIES(otbGalibTests${loop_var} otbgalib OTBIO OTBCommon)
#ENDFOREACH(loop_var)


# ROMAIN
# -------       CXX and EXECUTABLES files for KML Tests -----------------------------------
# hellofeatures
#FOREACH(filename        createkml helloattrs helloenum hellofolder
#                        hellogeometry hellohref hellokmz helloregion helloserializer
#                        helloworld parsekml prettykml printgeometry
#        )
#        ADD_EXECUTABLE(otbKmlTests${filename} kml${filename}.cc )
#        TARGET_LINK_LIBRARIES(otbKmlTests${filename} otbkml OTBIO OTBCommon)
#        IF( OTB_USE_EXTERNAL_EXPAT )
#                TARGET_LINK_LIBRARIES(otbKmlTests${filename} expat)
#        ELSE( OTB_USE_EXTERNAL_EXPAT  )
#                TARGET_LINK_LIBRARIES(otbKmlTests${filename} otbexpat)
#        ENDIF( OTB_USE_EXTERNAL_EXPAT )
#        IF(UNIX)
#                TARGET_LINK_LIBRARIES (otbKmlTests${filename} m)
#        ENDIF(UNIX)
#ENDFOREACH(filename)


# Original 6S main executable in C
#     ADD_EXECUTABLE(otb6SMain 6SMainTest.cxx )
#     TARGET_LINK_LIBRARIES(otb6SMain OTBCommon ITKCommon otb6S)
#     IF(UNIX)
#         TARGET_LINK_LIBRARIES (otb6SMain m)
#     ENDIF(UNIX)

# OTB 6S main executable in C (call 6S main function.
#     ADD_EXECUTABLE(otb6SMainWithCallFunction 6SMainOtbWithCallFunctionTest.cxx )
#     TARGET_LINK_LIBRARIES(otb6SMainWithCallFunction OTBCommon ITKCommon otb6S)
#     IF(UNIX)
#         TARGET_LINK_LIBRARIES (otb6SMainWithCallFunction m)
#     ENDIF(UNIX)

ENDIF( NOT OTB_DISABLE_CXX_TESTING AND BUILD_TESTING )
