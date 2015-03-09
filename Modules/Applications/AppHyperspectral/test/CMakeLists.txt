otb_module_test()
#----------- HyperspectralUnmixing TESTS ----------------
otb_test_application(NAME  apTvHyHyperspectralUnmixing_UCLS
                     APP  HyperspectralUnmixing
                     OPTIONS -in ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
                			 -ie ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
                 			 -out ${TEMP}/apTvHyHyperspectralUnmixing_UCLS.tif double
                 			 -ua ucls
                     VALID   --compare-image ${EPSILON_9}
                              ${BASELINE}/apTvHyHyperspectralUnmixing_UCLS.tif
                  			  ${TEMP}/apTvHyHyperspectralUnmixing_UCLS.tif)

otb_test_application(NAME  apTvHyHyperspectralUnmixing_NCLS
                     APP  HyperspectralUnmixing
                     OPTIONS -in ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
                			 -ie ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
                 			 -out ${TEMP}/apTvHyHyperspectralUnmixing_NCLS.tif double
                 			 -ua ncls
                     VALID   --compare-image ${EPSILON_9}
                              ${BASELINE}/apTvHyHyperspectralUnmixing_NCLS.tif
                  			  ${TEMP}/apTvHyHyperspectralUnmixing_NCLS.tif)


#----------- VertexComponentAnalysis TESTS ----------------
otb_test_application(NAME  apTvHyVertexComponentAnalysis
                     APP  VertexComponentAnalysis
                     OPTIONS -in ${OTB_DATA_ROOT}/Input/Hyperspectral/synthetic/hsi_cube.tif
                             -ne 5
                             -rand 121212
                 	     -outendm ${TEMP}/apTvHyVertexComponentAnalysis.tif double
                             VALID   --compare-image ${EPSILON_9}
                                     ${BASELINE}/TvHyVertexComponentAnalysisImage.tif
                  	             ${TEMP}/apTvHyVertexComponentAnalysis.tif
)

