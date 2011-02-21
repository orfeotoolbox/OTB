
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
  otbFullyConstrainedLeastSquareImageFilter.cxx
  otbVectorImageToMatrixImageFilter.cxx
  otbProjectiveProjection.cxx
  otbVCA.cxx
  otbVCAImageFilter.cxx
)

ADD_EXECUTABLE(otbHyperTests1 ${otbHyperTests1_SRC} otbHyperTests1.cxx)
TARGET_LINK_LIBRARIES(otbHyperTests1 OTBCommon OTBIO OTBBasicFilters OTBTesting)

ADD_TEST(vahineVCA1
         ${TESTEXE_DIR}/otbHyperTests1
         vahineVCA
         ${DATA}/AVIRIS/extract128/extract128_goodbands.hdr
         ${TEMP}/extract128_unmix_vahine_vca8endmembers.tif
         8)

ADD_TEST(vahineVCA2
         ${TESTEXE_DIR}/otbHyperTests1
         vahineVCA
         ${DATA}/SYNTHETIC100/hsi_cube.tif
         ${TEMP}/synthetic_unmix_5endmembers.tif
         5)

ADD_TEST(vahineElmVCA
         ${TESTEXE_DIR}/otbHyperTests1
         vahineElmVCA
         ${DATA}/AVIRIS/extract128/extract128_goodbands.hdr
         ${TEMP}/extract128_unmix_vahine_elmvca.tif)




ADD_TEST(bfTvStreamingStatisticsVectorImageFilter
         ${TESTEXE_DIR}/otbHyperTests1
         otbStreamingStatisticsVectorImageFilterTest
         ${DATA}/AVIRIS/extract128/extract128_goodbands.hdr
         ${TEMP}/extract128_stats_otb.txt)

ADD_TEST(bfTuStreamingStatisticsVectorImageFilterNew2
         ${TESTEXE_DIR}/otbHyperTests1
         otbStreamingStatisticsVectorImageFilter2NewTest)

#ADD_TEST(bfTvStreamingStatisticsVectorImageFilter_AVIRIS512
#         ${TESTEXE_DIR}/otbHyperTests1
#         otbStreamingStatisticsVectorImageFilterTest
#         ${DATA}/AVIRIS/extract512/f970619t01p02_r02_sc01_goodbands.hdr
#         ${TEMP}/aviris512_stats_otb.txt)
       
ADD_TEST(bfTvStreamingStatisticsVectorImageFilter2
         ${TESTEXE_DIR}/otbHyperTests1
         otbStreamingStatisticsVectorImageFilter2Test
         ${DATA}/AVIRIS/extract128/extract128_goodbands.hdr
         ${TEMP}/extract128_stats_rt.txt)

ADD_TEST(bfTvStreamingStatisticsVectorImageFilter2_AVIRIS512
         ${TESTEXE_DIR}/otbHyperTests1
         otbStreamingStatisticsVectorImageFilter2Test
         ${DATA}/AVIRIS/extract512/f970619t01p02_r02_sc01_goodbands.hdr
         ${TEMP}/aviris512_stats_rt.txt)

ADD_TEST(bfTuMatrixMultiplyImageFilterNew
         ${TESTEXE_DIR}/otbHyperTests1
         otbMatrixMultiplyImageFilterNewTest)

ADD_TEST(bfTvMatrixMultiplyImageFilter
         ${TESTEXE_DIR}/otbHyperTests1
         otbMatrixMultiplyImageFilterTest)

ADD_TEST(bfTvMatrixMultiplyImageFilterRealImageTest
         ${TESTEXE_DIR}/otbHyperTests1
         otbMatrixMultiplyImageFilterTest
         ${DATA}/SYNTHETIC100/hsi_cube.tif
         ${DATA}/SYNTHETIC100/endmembers.tif
         ${TEMP}/bfMatrixMultiplyImageFilter.tif)



ADD_TEST(bfTuUnConstrainedLeastSquareImageFilterNew
         ${TESTEXE_DIR}/otbHyperTests1
         otbUnConstrainedLeastSquareImageFilterNewTest)

ADD_TEST(bfTvUnConstrainedLeastSquareImageFilter
         ${TESTEXE_DIR}/otbHyperTests1
         otbUnConstrainedLeastSquareImageFilterTest
         ${DATA}/SYNTHETIC100/hsi_cube.tif
         ${DATA}/SYNTHETIC100/endmembers.tif
         ${TEMP}/bfUnConstrainedLeastSquareImageFilter.tif)


ADD_TEST(bfTuFullyConstrainedLeastSquareImageFilterNew
         ${TESTEXE_DIR}/otbHyperTests1
         otbFullyConstrainedLeastSquareImageFilterNewTest)

ADD_TEST(bfTvFullyConstrainedLeastSquareImageFilter_SYNTHETIC
         ${TESTEXE_DIR}/otbHyperTests1
         otbFullyConstrainedLeastSquareImageFilterTest
         ${DATA}/SYNTHETIC100/hsi_cube.tif
         ${DATA}/SYNTHETIC100/endmembers.tif
         ${TEMP}/bfFullyConstrainedLeastSquareImageFilter_SYNTHETIC.tif)


ADD_TEST(bfTvFullyConstrainedLeastSquareImageFilter_AVIRIS512
         ${TESTEXE_DIR}/otbHyperTests1
         otbFullyConstrainedLeastSquareImageFilterTest
         ${DATA}/AVIRIS/extract512/f970619t01p02_r02_sc01_goodbands.hdr
         ${TEMP}/aviris_vcaimagefilter_13.hdr
         ${TEMP}/bfFullyConstrainedLeastSquareImageFilter_AVIRIS512.tif)



ADD_TEST(bfTuVectorImageToMatrixNew
         ${TESTEXE_DIR}/otbHyperTests1
         otbVectorImageToMatrixNewTest)

ADD_TEST(bfTvVectorImageToMatrix
         ${TESTEXE_DIR}/otbHyperTests1
         otbVectorImageToMatrixTest)





ADD_TEST(hyTuEigenvalueLikelihoodMaximizationNew
         ${TESTEXE_DIR}/otbHyperTests1
         otbEigenvalueLikelihoodMaximizationNewTest)         

ADD_TEST(hyTvEigenvalueLikelihoodMaximization_AVIRIS1
         ${TESTEXE_DIR}/otbHyperTests1
         otbEigenvalueLikelihoodMaximizationTest
         ${DATA}/AVIRIS/extract128/extract128_goodbands.hdr
         ${TEMP}/extract128_elm.txt)


ADD_TEST(hyTvEigenvalueLikelihoodMaximization_AVIRIS2
         ${TESTEXE_DIR}/otbHyperTests1
         otbEigenvalueLikelihoodMaximizationTest
         ${DATA}/AVIRIS/extract512/f970619t01p02_r02_sc01_goodbands.hdr
         ${TEMP}/aviris512_elm.txt)

ADD_TEST(hyTvEigenvalueLikelihoodMaximization2
         ${TESTEXE_DIR}/otbHyperTests1
         otbEigenvalueLikelihoodMaximizationTest
         ${DATA}/SYNTHETIC100/hsi_cube.tif
         ${TEMP}/synthetic_elm.txt)

ADD_TEST(testProjectiveProjection
         ${TESTEXE_DIR}/otbHyperTests1
         otbProjectiveProjectionTestHighSNR
         ${DATA}/SYNTHETIC100/hsi_cube.tif
         8
         ${TEMP}/hsi_cube_projectiveprojected_8.tif )


ADD_TEST(hyTvVCA_Synthetic
         ${TESTEXE_DIR}/otbHyperTests1
         otbVCATestHighSNR
         ${DATA}/SYNTHETIC100/hsi_cube.tif
         ${TEMP}/synthetic_vca_5.hdr
         5 )


ADD_TEST(hyTvVCA_AVIRIS
         ${TESTEXE_DIR}/otbHyperTests1
         otbVCATestHighSNR
         ${DATA}/AVIRIS/extract512/f970619t01p02_r02_sc01_goodbands.hdr
         ${TEMP}/aviris_vca_13.hdr
         13 )


ADD_TEST(hyTvVCAImageFilter_Synthetic
         ${TESTEXE_DIR}/otbHyperTests1
         otbVCAImageFilterTestHighSNR
         ${DATA}/SYNTHETIC100/hsi_cube.tif
         ${TEMP}/synthetic_vcaimagefilter_5.hdr
         5 )


ADD_TEST(hyTvVCAImageFilter_AVIRIS
         ${TESTEXE_DIR}/otbHyperTests1
         otbVCAImageFilterTestHighSNR
         ${DATA}/AVIRIS/extract512/f970619t01p02_r02_sc01_goodbands.hdr
         ${TEMP}/aviris_vcaimagefilter_13.hdr
         13 )
         
###################
## Cublas
###################
IF( OTB_USE_CUBLAS )

SET( otbHyperCublasTests1_SRC
  otbCublasStreamingStatisticsVectorImageFilter.cxx
  otbCudaFCLSImageFilter.cxx
)

ADD_EXECUTABLE(otbHyperCublasTests1 ${otbHyperCublasTests1_SRC} otbHyperCublasTests1.cxx)
TARGET_LINK_LIBRARIES(otbHyperCublasTests1 OTBCommon OTBIO OTBBasicFilters OTBTesting OTBBasicFiltersGPUCuda OTBHyperspectralGPUCuda )

ADD_TEST(bfTuCublasStreamingStatisticsVectorImageFilterNew
         ${TESTEXE_DIR}/otbHyperCublasTests1
         otbCublasStreamingStatisticsVectorImageFilterNewTest)

ADD_TEST(bfTvCublasStreamingStatisticsVectorImageFilter
         ${TESTEXE_DIR}/otbHyperCublasTests1
         otbCublasStreamingStatisticsVectorImageFilterTest
         ${DATA}/AVIRIS/extract128/extract128_goodbands.hdr
         ${TEMP}/extract128_stats_cublas.txt)

ADD_TEST(bfTvCublasStreamingStatisticsVectorImageFilter_AVIRIS512
         ${TESTEXE_DIR}/otbHyperCublasTests1
         otbCublasStreamingStatisticsVectorImageFilterTest
         ${DATA}/AVIRIS/extract512/f970619t01p02_r02_sc01_goodbands.hdr
         ${TEMP}/aviris512_stats_cublas.txt)


ADD_TEST(bfTuCudaFullyConstrainedLeastSquareImageFilterNew
         ${TESTEXE_DIR}/otbHyperCublasTests1
         otbCudaFullyConstrainedLeastSquareImageFilterNewTest)

ADD_TEST(bfTvCudaFullyConstrainedLeastSquareImageFilter_SYNTHETIC
         ${TESTEXE_DIR}/otbHyperCublasTests1
         otbCudaFullyConstrainedLeastSquareImageFilterTest
         ${DATA}/SYNTHETIC100/hsi_cube.tif
         ${DATA}/SYNTHETIC100/endmembers.tif
         ${TEMP}/bfCudaFullyConstrainedLeastSquareImageFilter_SYNTHETIC.hdr
         500 # maxiter
         512 # GPU block size
         1 # stream size in MB
         )

ADD_TEST(bfTvCudaFullyConstrainedLeastSquareImageFilter_AVIRIS512_block512
         ${TESTEXE_DIR}/otbHyperCublasTests1
         otbCudaFullyConstrainedLeastSquareImageFilterTest
         ${DATA}/AVIRIS/extract512/f970619t01p02_r02_sc01_goodbands.hdr
         ${TEMP}/aviris_vcaimagefilter_13.hdr
         ${TEMP}/bfCudaFullyConstrainedLeastSquareImageFilter_AVIRIS512.hdr
         500 # maxiter
         512 # GPU block size
         1 # stream size in MB
         )

ENDIF( OTB_USE_CUBLAS )

###################
## Anomaly detection
###################

SET( otbAnomalyTests1_SRC
     LocalRxDetectorTest.cxx
     LocalRxDetectorRoiTest.cxx
     #LocalRxDetectorNonTrhreadedTest.cxx
     #LocalRxDetectorRoiNonThreadedTest.cxx
)

ADD_EXECUTABLE(otbAnomalyTests1 ${otbAnomalyTests1_SRC} otbAnomalyTests1.cxx)
TARGET_LINK_LIBRARIES(otbAnomalyTests1 OTBCommon OTBIO OTBBasicFilters OTBTesting )


ADD_TEST(tuLocalRXDetectorNew
         ${TESTEXE_DIR}/otbAnomalyTests1
         LocalRXDetectorNewTest
         )
         
ADD_TEST(tvLocalRXDetector
         ${TESTEXE_DIR}/otbAnomalyTests1
         LocalRXDetectorTest
         ${DATA}/CupriteSubHsi/cupriteSubHsi.tif
         ${TEMP}/localRXCupriteSubHsi.tif
         7 
         1
         )

ADD_TEST(tvLocalRXDetectorROI
         ${TESTEXE_DIR}/otbAnomalyTests1
         LocalRXDetectorROITest
         ${DATA}/CupriteSubHsi/cupriteSubHsi.tif
         ${TEMP}/localRXROICupriteSubHsi.tif
         7 
         20
         20
         1
         )
