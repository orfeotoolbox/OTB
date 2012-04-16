
#--- HyperspectralUnmixing ---#

OTB_TEST_APPLICATION(NAME  apTvHyHyperspectralUnmixing_UCLS
                     APP  HyperspectralUnmixing
                     OPTIONS -in ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
                			 -ie ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
                 			 -out ${TEMP}/apTvHyHyperspectralUnmixing_UCLS.tif double
                 			 -ua ucls 
                     VALID   --compare-image ${EPSILON_9}
                              ${BASELINE}/apTvHyHyperspectralUnmixing_UCLS.tif
                  			  ${TEMP}/apTvHyHyperspectralUnmixing_UCLS.tif)

OTB_TEST_APPLICATION(NAME  apTvHyHyperspectralUnmixing_NCLS
                     APP  HyperspectralUnmixing
                     OPTIONS -in ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
                			 -ie ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
                 			 -out ${TEMP}/apTvHyHyperspectralUnmixing_NCLS.tif double
                 			 -ua ncls 
                     VALID   --compare-image ${EPSILON_9}
                              ${BASELINE}/apTvHyHyperspectralUnmixing_NCLS.tif
                  			  ${TEMP}/apTvHyHyperspectralUnmixing_NCLS.tif)

# Too long !                
#add_test(NAME apTvHyHyperspectralUnmixing_ISRA
#         COMMAND otbTestDriver
#                 --compare-image ${EPSILON_9}
#                   ${BASELINE}/apTvHyHyperspectralUnmixing_ISRA.tif
#                   ${TEMP}/apTvHyHyperspectralUnmixing_ISRA.tif
#                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
#                 HyperspectralUnmixing
#                 $<TARGET_FILE_DIR:otbapp_HyperspectralUnmixing>
#                 -in ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
#                 -ie ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
#                 -out ${TEMP}/apTvHyHyperspectralUnmixing_ISRA.tif double
#                 -ua isra )

#--- VertexComponentAnalysis ---#
OTB_TEST_APPLICATION(NAME  apTvHyVertexComponentAnalysis
                     APP  VertexComponentAnalysis
                     OPTIONS -in ${OTB_DATA_ROOT}/Input/Hyperspectral/synthetic/hsi_cube.tif
                             -ne 5
                             -rand 121212
                 	     -outendm ${TEMP}/apTvHyVertexComponentAnalysis.tif double
                             VALID   --compare-image ${EPSILON_9}
                                     ${BASELINE}/TvHyVertexComponentAnalysisImage.tif
                  	             ${TEMP}/apTvHyVertexComponentAnalysis.tif
)

    
