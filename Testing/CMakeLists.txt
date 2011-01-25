
SET(TESTEXE_DIR ${Hyperspectral_BINARY_DIR}/bin)
SET(TEMP ${Hyperspectral_BINARY_DIR}/Testing/Temporary)
SET(DATA ${Hyperspectral_SOURCE_DIR}/Data)

SET( otbHyperTests1_SRC
  vahineElmVCA.cxx
  otbEigenvalueLikelihoodMaximization.cxx
  otbStreamingStatisticsVectorImageFilter.cxx
  otbStreamingStatisticsVectorImageFilter2.cxx
  otbMatrixMultiplyImageFilter.cxx
  otbUnConstrainedLeastSquareImageFilter.cxx
  otbVectorImageToMatrixImageFilter.cxx
)

ADD_EXECUTABLE(otbHyperTests1 ${otbHyperTests1_SRC} otbHyperTests1.cxx)
TARGET_LINK_LIBRARIES(otbHyperTests1 OTBCommon OTBIO OTBBasicFilters OTBTesting )



ADD_TEST(vahineVCA1
         ${TESTEXE_DIR}/otbHyperTests1
         vahineVCA
         ${DATA}/AVIRIS/extract128/extract128_goodbands.hdr
         ${TEMP}/extract128_unmix_vahine_vca8endmembers.tif
         8)

ADD_TEST(vahineVCA2
         ${TESTEXE_DIR}/otbHyperTests1
         vahineVCA
         ${DATA}/SYNTHETIC/hsi_cube.tif
         ${TEMP}/synthetic_unmix_5endmembers.tif
         5)

ADD_TEST(vahineElmVCA
         ${TESTEXE_DIR}/otbHyperTests1
         vahineElmVCA
         ${DATA}/AVIRIS/extract128/extract128_goodbands.hdr
         ${TEMP}/extract128_unmix_vahine_elmvca.tif)




ADD_TEST(bfStreamingStatisticsVectorImageFilter
         ${TESTEXE_DIR}/otbHyperTests1
         otbStreamingStatisticsVectorImageFilterTest
         ${DATA}/AVIRIS/extract128/extract128_goodbands.hdr
         ${TEMP}/extract128_stats_otb.txt)

ADD_TEST(bfStreamingStatisticsVectorImageFilterNew2
         ${TESTEXE_DIR}/otbHyperTests1
         otbStreamingStatisticsVectorImageFilter2NewTest)

ADD_TEST(bfStreamingStatisticsVectorImageFilter2
         ${TESTEXE_DIR}/otbHyperTests1
         otbStreamingStatisticsVectorImageFilter2Test
         ${DATA}/AVIRIS/extract128/extract128_goodbands.hdr
         ${TEMP}/extract128_stats_rt.txt)




ADD_TEST(bfMatrixMultiplyImageFilterNew
         ${TESTEXE_DIR}/otbHyperTests1
         otbMatrixMultiplyImageFilterNewTest)

ADD_TEST(bfMatrixMultiplyImageFilter
         ${TESTEXE_DIR}/otbHyperTests1
         otbMatrixMultiplyImageFilterTest
         ${DATA}/SYNTHETIC/hsi_cube.tif
         ${DATA}/SYNTHETIC/endmembers.tif
         ${TEMP}/bfMatrixMultiplyImageFilter.tif)



ADD_TEST(bfUnConstrainedLeastSquareImageFilterNew
         ${TESTEXE_DIR}/otbHyperTests1
         otbUnConstrainedLeastSquareImageFilterNewTest)

ADD_TEST(bfUnConstrainedLeastSquareImageFilter
         ${TESTEXE_DIR}/otbHyperTests1
         otbUnConstrainedLeastSquareImageFilterTest
         ${DATA}/SYNTHETIC/hsi_cube.tif
         ${DATA}/SYNTHETIC/endmembers.tif
         ${TEMP}/bfUnConstrainedLeastSquareImageFilter.tif)




ADD_TEST(bfVectorImageToMatrixNew
         ${TESTEXE_DIR}/otbHyperTests1
         otbVectorImageToMatrixNewTest)

ADD_TEST(bfVectorImageToMatrix
         ${TESTEXE_DIR}/otbHyperTests1
         otbVectorImageToMatrixTest
         ${DATA}/AVIRIS/extract128/extract128_goodbands.hdr
         ${TEMP}/extract128_matrix.txt)





ADD_TEST(hyEigenvalueLikelihoodMaximizationNew
         ${TESTEXE_DIR}/otbHyperTests1
         otbEigenvalueLikelihoodMaximizationNewTest)         

ADD_TEST(hyEigenvalueLikelihoodMaximization1
         ${TESTEXE_DIR}/otbHyperTests1
         otbEigenvalueLikelihoodMaximizationTest
         ${DATA}/AVIRIS/extract128/extract128_goodbands.hdr
         ${TEMP}/extract128_elm.txt)

ADD_TEST(hyEigenvalueLikelihoodMaximization2
         ${TESTEXE_DIR}/otbHyperTests1
         otbEigenvalueLikelihoodMaximizationTest
         ${DATA}/SYNTHETIC/hsi_cube.tif
         ${TEMP}/synthetic_elm.txt)

         