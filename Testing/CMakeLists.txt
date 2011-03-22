
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
  otbISRAUnmixingImageFilter.cxx
  otbFCLSUnmixingImageFilter.cxx
  otbCLSPSTOUnmixingImageFilter.cxx
  otbNCLSUnmixingImageFilter.cxx
  otbFullyConstrainedLeastSquareImageFilter.cxx
  otbVectorImageToMatrixImageFilter.cxx
  otbProjectiveProjection.cxx
  otbVCA.cxx
  otbVCAImageFilter.cxx
  otbHorizontalSobelVectorImageFilter.cxx
  otbVerticalSobelVectorImageFilter.cxx
  otbSobelVectorImageFilter.cxx
  otbLocalGradientVectorImageFilter.cxx
  otbLocalActivityVectorImageFilter.cxx
  otbNormalizeVectorImageFilter.cxx
  otbPCAImageFilter.cxx
  otbMNFImageFilter.cxx
  otbNAPCAImageFilter.cxx
  otbFastICAImageFilter.cxx
  otbSparseWvltToAngleMapperListFilter.cxx
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

ADD_TEST(bfTvUnConstrainedLeastSquareImageFilter_SYNTHETIC100
         ${TESTEXE_DIR}/otbHyperTests1
         otbUnConstrainedLeastSquareImageFilterTest
         ${DATA}/SYNTHETIC100/hsi_cube.tif
         ${DATA}/SYNTHETIC100/endmembers.tif
         ${TEMP}/bfUnConstrainedLeastSquareImageFilter_SYNTHETIC100.hdr)
ADD_TEST(NAME bfTvUnConstrainedLeastSquareImageFilter_SYNTHETIC100_Recompose
         COMMAND ${TESTEXE_DIR}/otbHyperspectralRecompose
         -in ${TEMP}/bfUnConstrainedLeastSquareImageFilter_SYNTHETIC100.hdr
         -out ${TEMP}/bfUnConstrainedLeastSquareImageFilter_SYNTHETIC100_Recomposed.hdr
         -e ${DATA}/SYNTHETIC100/endmembers.tif)
SET_TESTS_PROPERTIES(bfTvUnConstrainedLeastSquareImageFilter_SYNTHETIC100_Recompose
                     PROPERTIES DEPENDS bfTvUnConstrainedLeastSquareImageFilter_SYNTHETIC100)
ADD_TEST(NAME bfTvUnConstrainedLeastSquareImageFilter_SYNTHETIC100_Compare
         COMMAND ${TESTEXE_DIR}/otbHyperspectralCompare
         -in1 ${DATA}/SYNTHETIC100/hsi_cube.tif
         -in2 ${TEMP}/bfUnConstrainedLeastSquareImageFilter_SYNTHETIC100_Recomposed.hdr
         -out ${TEMP}/bfUnConstrainedLeastSquareImageFilter_SYNTHETIC100_SpectralAngle.hdr
         -s ${TEMP}/bfUnConstrainedLeastSquareImageFilter_SYNTHETIC100_SpectralAngle_Stats.txt )
SET_TESTS_PROPERTIES(bfTvUnConstrainedLeastSquareImageFilter_SYNTHETIC100_Compare
                     PROPERTIES DEPENDS bfTvUnConstrainedLeastSquareImageFilter_SYNTHETIC100_Recompose)


ADD_TEST(bfTvUnConstrainedLeastSquareImageFilter_AVIRIS512
         ${TESTEXE_DIR}/otbHyperTests1
         otbUnConstrainedLeastSquareImageFilterTest
         ${DATA}/AVIRIS/extract512/f970619t01p02_r02_sc01_goodbands.hdr
         ${TEMP}/aviris_vcaimagefilter_13_2.hdr
         ${TEMP}/bfUnConstrainedLeastSquareImageFilter_AVIRIS512.hdr)
ADD_TEST(NAME bfTvUnConstrainedLeastSquareImageFilter_AVIRIS512_Recompose
         COMMAND ${TESTEXE_DIR}/otbHyperspectralRecompose
         -in ${TEMP}/bfUnConstrainedLeastSquareImageFilter_AVIRIS512.hdr
         -out ${TEMP}/bfUnConstrainedLeastSquareImageFilter_AVIRIS512_Recomposed.hdr
         -e ${TEMP}/aviris_vcaimagefilter_13_2.hdr)
SET_TESTS_PROPERTIES(bfTvUnConstrainedLeastSquareImageFilter_AVIRIS512_Recompose
                     PROPERTIES DEPENDS bfTvUnConstrainedLeastSquareImageFilter_AVIRIS512)
ADD_TEST(NAME bfTvUnConstrainedLeastSquareImageFilter_AVIRIS512_Compare
         COMMAND ${TESTEXE_DIR}/otbHyperspectralCompare
         -in1 ${DATA}/AVIRIS/extract512/f970619t01p02_r02_sc01_goodbands.hdr
         -in2 ${TEMP}/bfUnConstrainedLeastSquareImageFilter_AVIRIS512_Recomposed.hdr
         -out ${TEMP}/bfUnConstrainedLeastSquareImageFilter_AVIRIS512_SpectralAngle.hdr
         -s ${TEMP}/bfUnConstrainedLeastSquareImageFilter_AVIRIS512_SpectralAngle_Stats.txt )
SET_TESTS_PROPERTIES(bfTvUnConstrainedLeastSquareImageFilter_AVIRIS512_Compare
                     PROPERTIES DEPENDS bfTvUnConstrainedLeastSquareImageFilter_AVIRIS512_Recompose)


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
         ${TEMP}/aviris_vcaimagefilter_13_2.hdr
         ${TEMP}/bfFullyConstrainedLeastSquareImageFilter_AVIRIS512.tif)


ADD_TEST(bfTuISRAUnmixingImageFilterNew
         ${TESTEXE_DIR}/otbHyperTests1
         otbISRAUnmixingImageFilterNewTest)


ADD_TEST(bfTvISRAUnmixingImageFilter_SYNTHETIC100
         ${TESTEXE_DIR}/otbHyperTests1
         otbISRAUnmixingImageFilterTest
         ${DATA}/SYNTHETIC100/hsi_cube.tif
         ${DATA}/SYNTHETIC100/endmembers.tif
         ${TEMP}/bfISRAUnmixingImageFilter_SYNTHETIC100.hdr
         10)
ADD_TEST(NAME bfTvISRAUnmixingImageFilter_SYNTHETIC100_Recompose
         COMMAND ${TESTEXE_DIR}/otbHyperspectralRecompose
         -in ${TEMP}/bfISRAUnmixingImageFilter_SYNTHETIC100.hdr
         -out ${TEMP}/bfISRAUnmixingImageFilter_SYNTHETIC100_Recomposed.hdr
         -e ${DATA}/SYNTHETIC100/endmembers.tif)
SET_TESTS_PROPERTIES(bfTvISRAUnmixingImageFilter_SYNTHETIC100_Recompose
                     PROPERTIES DEPENDS bfTvISRAUnmixingImageFilter_SYNTHETIC100)
ADD_TEST(NAME bfTvISRAUnmixingImageFilter_SYNTHETIC100_Compare
         COMMAND ${TESTEXE_DIR}/otbHyperspectralCompare
         -in1 ${DATA}/SYNTHETIC100/hsi_cube.tif
         -in2 ${TEMP}/bfISRAUnmixingImageFilter_SYNTHETIC100_Recomposed.hdr
         -out ${TEMP}/bfISRAUnmixingImageFilter_SYNTHETIC100_SpectralAngle.hdr
         -s ${TEMP}/bfISRAUnmixingImageFilter_SYNTHETIC100_SpectralAngle_Stats.txt )
SET_TESTS_PROPERTIES(bfTvISRAUnmixingImageFilter_SYNTHETIC100_Compare
                     PROPERTIES DEPENDS bfTvISRAUnmixingImageFilter_SYNTHETIC100_Recompose)


ADD_TEST(bfTvISRAUnmixingImageFilter_AVIRIS512
         ${TESTEXE_DIR}/otbHyperTests1
         otbISRAUnmixingImageFilterTest
         ${DATA}/AVIRIS/extract512/f970619t01p02_r02_sc01_goodbands.hdr
         ${TEMP}/aviris_vcaimagefilter_13_2.hdr
         ${TEMP}/bfISRAUnmixingImageFilter_AVIRIS512.hdr
         10)
ADD_TEST(NAME bfTvISRAUnmixingImageFilter_AVIRIS512_Recompose
         COMMAND ${TESTEXE_DIR}/otbHyperspectralRecompose
         -in ${TEMP}/bfISRAUnmixingImageFilter_AVIRIS512.hdr
         -out ${TEMP}/bfISRAUnmixingImageFilter_AVIRIS512_Recomposed.hdr
         -e ${TEMP}/aviris_vcaimagefilter_13_2.hdr)
SET_TESTS_PROPERTIES(bfTvISRAUnmixingImageFilter_AVIRIS512_Recompose
                     PROPERTIES DEPENDS bfTvISRAUnmixingImageFilter_AVIRIS512)
ADD_TEST(NAME bfTvISRAUnmixingImageFilter_AVIRIS512_Compare
         COMMAND ${TESTEXE_DIR}/otbHyperspectralCompare
         -in1 ${DATA}/AVIRIS/extract512/f970619t01p02_r02_sc01_goodbands.hdr
         -in2 ${TEMP}/bfISRAUnmixingImageFilter_AVIRIS512_Recomposed.hdr
         -out ${TEMP}/bfISRAUnmixingImageFilter_AVIRIS512_SpectralAngle.hdr
         -s ${TEMP}/bfISRAUnmixingImageFilter_AVIRIS512_SpectralAngle_Stats.txt )
SET_TESTS_PROPERTIES(bfTvISRAUnmixingImageFilter_AVIRIS512_Compare
                     PROPERTIES DEPENDS bfTvISRAUnmixingImageFilter_AVIRIS512_Recompose)

ADD_TEST(bfTvISRAUnmixingImageFilter_AVIRIS512_iter100
         ${TESTEXE_DIR}/otbHyperTests1
         otbISRAUnmixingImageFilterTest
         ${DATA}/AVIRIS/extract512/f970619t01p02_r02_sc01_goodbands.hdr
         ${TEMP}/aviris_vcaimagefilter_13_2.hdr
         ${TEMP}/bfISRAUnmixingImageFilter_AVIRIS512_iter100.hdr
         10)
ADD_TEST(NAME bfTvISRAUnmixingImageFilter_AVIRIS512_iter100_Recompose
         COMMAND ${TESTEXE_DIR}/otbHyperspectralRecompose
         -in ${TEMP}/bfISRAUnmixingImageFilter_AVIRIS512_iter100.hdr
         -out ${TEMP}/bfISRAUnmixingImageFilter_AVIRIS512_iter100_Recomposed.hdr
         -e ${TEMP}/aviris_vcaimagefilter_13_2.hdr)
SET_TESTS_PROPERTIES(bfTvISRAUnmixingImageFilter_AVIRIS512_iter100_Recompose
                     PROPERTIES DEPENDS bfTvISRAUnmixingImageFilter_AVIRIS512_iter100)
ADD_TEST(NAME bfTvISRAUnmixingImageFilter_AVIRIS512_iter100_Compare
         COMMAND ${TESTEXE_DIR}/otbHyperspectralCompare
         -in1 ${DATA}/AVIRIS/extract512/f970619t01p02_r02_sc01_goodbands.hdr
         -in2 ${TEMP}/bfISRAUnmixingImageFilter_AVIRIS512_iter100_Recomposed.hdr
         -out ${TEMP}/bfISRAUnmixingImageFilter_AVIRIS512_iter100_SpectralAngle.hdr
         -s ${TEMP}/bfISRAUnmixingImageFilter_AVIRIS512_iter100_SpectralAngle_Stats.txt )
SET_TESTS_PROPERTIES(bfTvISRAUnmixingImageFilter_AVIRIS512_iter100_Compare
                     PROPERTIES DEPENDS bfTvISRAUnmixingImageFilter_AVIRIS512_iter100_Recompose)



ADD_TEST(bfTvNCLSUnmixingImageFilter_SYNTHETIC100
         ${TESTEXE_DIR}/otbHyperTests1
         otbNCLSUnmixingImageFilterTest
         ${DATA}/SYNTHETIC100/hsi_cube.tif
         ${DATA}/SYNTHETIC100/endmembers.tif
         ${TEMP}/bfNCLSUnmixingImageFilter_SYNTHETIC100.hdr
         10)
ADD_TEST(NAME bfTvNCLSUnmixingImageFilter_SYNTHETIC100_Recompose
         COMMAND ${TESTEXE_DIR}/otbHyperspectralRecompose
         -in ${TEMP}/bfNCLSUnmixingImageFilter_SYNTHETIC100.hdr
         -out ${TEMP}/bfNCLSUnmixingImageFilter_SYNTHETIC100_Recomposed.hdr
         -e ${DATA}/SYNTHETIC100/endmembers.tif)
SET_TESTS_PROPERTIES(bfTvNCLSUnmixingImageFilter_SYNTHETIC100_Recompose
                     PROPERTIES DEPENDS bfTvNCLSUnmixingImageFilter_SYNTHETIC100)
ADD_TEST(NAME bfTvNCLSUnmixingImageFilter_SYNTHETIC100_Compare
         COMMAND ${TESTEXE_DIR}/otbHyperspectralCompare
         -in1 ${DATA}/SYNTHETIC100/hsi_cube.tif
         -in2 ${TEMP}/bfNCLSUnmixingImageFilter_SYNTHETIC100_Recomposed.hdr
         -out ${TEMP}/bfNCLSUnmixingImageFilter_SYNTHETIC100_SpectralAngle.hdr
         -s ${TEMP}/bfNCLSUnmixingImageFilter_SYNTHETIC100_SpectralAngle_Stats.txt )
SET_TESTS_PROPERTIES(bfTvNCLSUnmixingImageFilter_SYNTHETIC100_Compare
                     PROPERTIES DEPENDS bfTvNCLSUnmixingImageFilter_SYNTHETIC100_Recompose)


ADD_TEST(bfTvNCLSUnmixingImageFilter_AVIRIS512_iter100
         ${TESTEXE_DIR}/otbHyperTests1
         otbNCLSUnmixingImageFilterTest
         ${DATA}/AVIRIS/extract512/f970619t01p02_r02_sc01_goodbands.hdr
         ${TEMP}/aviris_vcaimagefilter_13_2.hdr
         ${TEMP}/bfNCLSUnmixingImageFilter_AVIRIS512_iter100.hdr
         10)
ADD_TEST(NAME bfTvNCLSUnmixingImageFilter_AVIRIS512_iter100_Recompose
         COMMAND ${TESTEXE_DIR}/otbHyperspectralRecompose
         -in ${TEMP}/bfNCLSUnmixingImageFilter_AVIRIS512_iter100.hdr
         -out ${TEMP}/bfNCLSUnmixingImageFilter_AVIRIS512_iter100_Recomposed.hdr
         -e ${TEMP}/aviris_vcaimagefilter_13_2.hdr)
SET_TESTS_PROPERTIES(bfTvNCLSUnmixingImageFilter_AVIRIS512_iter100_Recompose
                     PROPERTIES DEPENDS bfTvNCLSUnmixingImageFilter_AVIRIS512_iter100)
ADD_TEST(NAME bfTvNCLSUnmixingImageFilter_AVIRIS512_iter100_Compare
         COMMAND ${TESTEXE_DIR}/otbHyperspectralCompare
         -in1 ${DATA}/AVIRIS/extract512/f970619t01p02_r02_sc01_goodbands.hdr
         -in2 ${TEMP}/bfNCLSUnmixingImageFilter_AVIRIS512_iter100_Recomposed.hdr
         -out ${TEMP}/bfNCLSUnmixingImageFilter_AVIRIS512_iter100_SpectralAngle.hdr
         -s ${TEMP}/bfNCLSUnmixingImageFilter_AVIRIS512_iter100_SpectralAngle_Stats.txt )
SET_TESTS_PROPERTIES(bfTvNCLSUnmixingImageFilter_AVIRIS512_iter100_Compare
                     PROPERTIES DEPENDS bfTvNCLSUnmixingImageFilter_AVIRIS512_iter100_Recompose)


ADD_TEST(bfTvFCLSUnmixingImageFilter_AVIRIS512_iter100
         ${TESTEXE_DIR}/otbHyperTests1
         otbFCLSUnmixingImageFilterTest
         ${DATA}/AVIRIS/extract512/f970619t01p02_r02_sc01_goodbands.hdr
         ${TEMP}/aviris_vcaimagefilter_13_2.hdr
         ${TEMP}/bfFCLSUnmixingImageFilter_AVIRIS512_iter100.hdr
         10)
ADD_TEST(NAME bfTvFCLSUnmixingImageFilter_AVIRIS512_iter100_Recompose
         COMMAND ${TESTEXE_DIR}/otbHyperspectralRecompose
         -in ${TEMP}/bfFCLSUnmixingImageFilter_AVIRIS512_iter100.hdr
         -out ${TEMP}/bfFCLSUnmixingImageFilter_AVIRIS512_iter100_Recomposed.hdr
         -e ${TEMP}/aviris_vcaimagefilter_13_2.hdr)
SET_TESTS_PROPERTIES(bfTvFCLSUnmixingImageFilter_AVIRIS512_iter100_Recompose
                     PROPERTIES DEPENDS bfTvFCLSUnmixingImageFilter_AVIRIS512_iter100)
ADD_TEST(NAME bfTvFCLSUnmixingImageFilter_AVIRIS512_iter100_Compare
         COMMAND ${TESTEXE_DIR}/otbHyperspectralCompare
         -in1 ${DATA}/AVIRIS/extract512/f970619t01p02_r02_sc01_goodbands.hdr
         -in2 ${TEMP}/bfFCLSUnmixingImageFilter_AVIRIS512_iter100_Recomposed.hdr
         -out ${TEMP}/bfFCLSUnmixingImageFilter_AVIRIS512_iter100_SpectralAngle.hdr
         -s ${TEMP}/bfFCLSUnmixingImageFilter_AVIRIS512_iter100_SpectralAngle_Stats.txt )
SET_TESTS_PROPERTIES(bfTvFCLSUnmixingImageFilter_AVIRIS512_iter100_Compare
                     PROPERTIES DEPENDS bfTvFCLSUnmixingImageFilter_AVIRIS512_iter100_Recompose)


ADD_TEST(bfTvCLSPSTOUnmixingImageFilter_SYNTHETIC100
         ${TESTEXE_DIR}/otbHyperTests1
         otbCLSPSTOUnmixingImageFilterTest
         ${DATA}/SYNTHETIC100/hsi_cube.tif
         ${DATA}/SYNTHETIC100/endmembers.tif
         ${TEMP}/bfCLSPSTOUnmixingImageFilter_SYNTHETIC100.hdr
         10)
ADD_TEST(NAME bfTvCLSPSTOUnmixingImageFilter_SYNTHETIC100_Recompose
         COMMAND ${TESTEXE_DIR}/otbHyperspectralRecompose
         -in ${TEMP}/bfCLSPSTOUnmixingImageFilter_SYNTHETIC100.hdr
         -out ${TEMP}/bfCLSPSTOUnmixingImageFilter_SYNTHETIC100_Recomposed.hdr
         -e ${DATA}/SYNTHETIC100/endmembers.tif)
SET_TESTS_PROPERTIES(bfTvCLSPSTOUnmixingImageFilter_SYNTHETIC100_Recompose
                     PROPERTIES DEPENDS bfTvCLSPSTOUnmixingImageFilter_SYNTHETIC100)
ADD_TEST(NAME bfTvCLSPSTOUnmixingImageFilter_SYNTHETIC100_Compare
         COMMAND ${TESTEXE_DIR}/otbHyperspectralCompare
         -in1 ${DATA}/SYNTHETIC100/hsi_cube.tif
         -in2 ${TEMP}/bfCLSPSTOUnmixingImageFilter_SYNTHETIC100_Recomposed.hdr
         -out ${TEMP}/bfCLSPSTOUnmixingImageFilter_SYNTHETIC100_SpectralAngle.hdr
         -s ${TEMP}/bfCLSPSTOUnmixingImageFilter_SYNTHETIC100_SpectralAngle_Stats.txt )
SET_TESTS_PROPERTIES(bfTvCLSPSTOUnmixingImageFilter_SYNTHETIC100_Compare
                     PROPERTIES DEPENDS bfTvCLSPSTOUnmixingImageFilter_SYNTHETIC100_Recompose)


ADD_TEST(bfTuVectorImageToMatrixNew
         ${TESTEXE_DIR}/otbHyperTests1
         otbVectorImageToMatrixNewTest)

ADD_TEST(bfTvVectorImageToMatrix
         ${TESTEXE_DIR}/otbHyperTests1
         otbVectorImageToMatrixTest)

ADD_TEST(bfTuHorizontalSobelVectorImageFilterNew
         ${TESTEXE_DIR}/otbHyperTests1
         otbHorizontalSobelVectorImageFilterNewTest)

ADD_TEST(bfTvHorizontalSobelVectorImageFilter
         ${TESTEXE_DIR}/otbHyperTests1
         otbHorizontalSobelVectorImageFilterTest
		 -in ${DATA}/CupriteSubHsi/cupriteSubHsi.tif 
		 -out ${TEMP}/cupriteHoriSobelFlt.hdr)

ADD_TEST(bfTuVerticalSobelVectorImageFilterNew
         ${TESTEXE_DIR}/otbHyperTests1
         otbVerticalSobelVectorImageFilterNewTest)

ADD_TEST(bfTvVerticalSobelVectorImageFilter
         ${TESTEXE_DIR}/otbHyperTests1
         otbVerticalSobelVectorImageFilterTest
		 -in ${DATA}/CupriteSubHsi/cupriteSubHsi.tif 
		 -out ${TEMP}/cupriteVertSobelFlt.hdr)

ADD_TEST(bfTuSobelVectorImageFilterNew
         ${TESTEXE_DIR}/otbHyperTests1
         otbSobelVectorImageFilterNewTest)

ADD_TEST(bfTvSobelVectorImageFilter
         ${TESTEXE_DIR}/otbHyperTests1
         otbSobelVectorImageFilterTest
		 -in ${DATA}/CupriteSubHsi/cupriteSubHsi.tif 
		 -out ${TEMP}/cupriteSobelFlt.hdr)

ADD_TEST(bfTuLocalGradientVectorImageFilterNew
         ${TESTEXE_DIR}/otbHyperTests1
         otbLocalGradientVectorImageFilterNewTest)

ADD_TEST(bfTvLocalGradientVectorImageFilter
         ${TESTEXE_DIR}/otbHyperTests1
         otbLocalGradientVectorImageFilterTest
		 -in ${DATA}/CupriteSubHsi/cupriteSubHsi.tif 
		 -out ${TEMP}/cupriteLocalGradientFlt.hdr)

ADD_TEST(bfTuLocalActivityVectorImageFilterNew
         ${TESTEXE_DIR}/otbHyperTests1
         otbLocalActivityVectorImageFilterNewTest)

ADD_TEST(bfTvLocalActivityVectorImageFilter
         ${TESTEXE_DIR}/otbHyperTests1
         otbLocalActivityVectorImageFilterTest
		 -in ${DATA}/CupriteSubHsi/cupriteSubHsi.tif 
		 -out ${TEMP}/cupriteLocalActivityFlt.hdr)

ADD_TEST(bfTuNormalizeVectorImageFilter
         ${TESTEXE_DIR}/otbHyperTests1
		 otbNormalizeVectorImageFilterNewTest)

ADD_TEST(bfTvNormalizeVectorImageFilter
         ${TESTEXE_DIR}/otbHyperTests1
		 otbNormalizeVectorImageFilterTest
         -in ${DATA}/CupriteSubHsi/cupriteSubHsi.tif 
		 -out ${TEMP}/cupriteNormalizedFlt.hdr)

ADD_TEST(hyTuPCAImageFilterNew
         ${TESTEXE_DIR}/otbHyperTests1
		 otbPCAImageFilterNewTest)

ADD_TEST(hyTvPCAImageFilter
         ${TESTEXE_DIR}/otbHyperTests1
		 otbPCAImageFilterTest 
		 -in ${DATA}/CupriteSubHsi/cupriteSubHsi.tif
		 -inv ${TEMP}/cupritePCAinv.hdr
		 -out ${TEMP}/cupritePCA.hdr)

ADD_TEST(hyTuMNFImageFilterNew
         ${TESTEXE_DIR}/otbHyperTests1
		 otbMNFImageFilterNewTest)

ADD_TEST(hyTvMNFImageFilter
         ${TESTEXE_DIR}/otbHyperTests1
		 otbMNFImageFilterTest 
		 -norm 
		 -in ${DATA}/CupriteSubHsi/cupriteSubHsi.tif
		 -inv ${TEMP}/cupriteMNFinv.hdr
		 -out ${TEMP}/cupriteMNF.hdr)

ADD_TEST(hyTuNAPCAImageFilterNew
         ${TESTEXE_DIR}/otbHyperTests1
		 otbNAPCAImageFilterNewTest)

ADD_TEST(hyTvNAPCAImageFilter
         ${TESTEXE_DIR}/otbHyperTests1
		 otbNAPCAImageFilterTest 
		 -in ${DATA}/CupriteSubHsi/cupriteSubHsi.tif
		 -inv ${TEMP}/cupriteNAPCAinv.hdr
		 -out ${TEMP}/cupriteNAPCA.hdr)

ADD_TEST(bfTuFastICAInternalOptimizerVectorImageFilterNew 
		 ${TESTEXE_DIR}/otbHyperTests1
		 otbFastICAInternalOptimizerVectorImageFilterNewTest)

ADD_TEST(hyTuFastICAImageFilterNew
		${TESTEXE_DIR}/otbHyperTests1
		otbFastICAImageFilterNewTest)

ADD_TEST(hyTvFastICAImageFilter
		${TESTEXE_DIR}/otbHyperTests1
		otbFastICAImageFilterTest
		-in ${DATA}/CupriteSubHsi/cupriteSubHsi.tif
		-inv ${TEMP}/cupriteFastICAinv.hdr
		-out ${TEMP}/cupriteFastICA.hdr)

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
         ${DATA}/AVIRIS/extract512/f970619t01p02_r02_sc02_goodbands.hdr
         ${TEMP}/aviris_vcaimagefilter_13.hdr
         13 )

ADD_TEST(bfTuSparseWvltToAngleMapperListFilter
		${TESTEXE_DIR}/otbHyperTests1
		otbSparseWvltToAngleMapperListFilterNewTest)


###################
## Cuda
###################
IF( OTB_USE_CUDA )

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
         100 # maxiter
         512 # GPU block size
         1 # stream size in MB
         )

ADD_TEST(bfTvCudaFullyConstrainedLeastSquareImageFilter_AVIRIS512_block512
         ${TESTEXE_DIR}/otbHyperCublasTests1
         otbCudaFullyConstrainedLeastSquareImageFilterTest
         ${DATA}/AVIRIS/extract512/f970619t01p02_r02_sc01_goodbands.hdr
         ${TEMP}/aviris_vcaimagefilter_13_2.hdr
         ${TEMP}/bfCudaFullyConstrainedLeastSquareImageFilter_AVIRIS512.hdr
         100 # maxiter
         512 # GPU block size
         1 # stream size in MB
         )

ENDIF( OTB_USE_CUDA )

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
