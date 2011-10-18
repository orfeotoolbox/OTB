
#--- HyperspectralUnmixing ---#

add_test(NAME apTvHyHyperspectralUnmixing_UCLS
         COMMAND otbTestDriver
                 --compare-image ${EPSILON_9}
                   ${BASELINE}/apTvHyHyperspectralUnmixing_UCLS.tif
                   ${TEMP}/apTvHyHyperspectralUnmixing_UCLS.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 HyperspectralUnmixing
                 $<TARGET_FILE_DIR:otbapp_HyperspectralUnmixing>
                 --in ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
                 --ie ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
                 --out ${TEMP}/apTvHyHyperspectralUnmixing_UCLS.tif double
                 --ua ucls )

add_test(NAME apTvHyHyperspectralUnmixing_NCLS
         COMMAND otbTestDriver
                 --compare-image ${EPSILON_9}
                   ${BASELINE}/apTvHyHyperspectralUnmixing_NCLS.tif
                   ${TEMP}/apTvHyHyperspectralUnmixing_NCLS.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 HyperspectralUnmixing
                 $<TARGET_FILE_DIR:otbapp_HyperspectralUnmixing>
                 --in ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
                 --ie ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
                 --out ${TEMP}/apTvHyHyperspectralUnmixing_NCLS.tif double
                 --ua ncls )
# Too long !                
#add_test(NAME apTvHyHyperspectralUnmixing_ISRA
#         COMMAND otbTestDriver
#                 --compare-image ${EPSILON_9}
#                   ${BASELINE}/apTvHyHyperspectralUnmixing_ISRA.tif
#                   ${TEMP}/apTvHyHyperspectralUnmixing_ISRA.tif
#                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
#                 HyperspectralUnmixing
#                 $<TARGET_FILE_DIR:otbapp_HyperspectralUnmixing>
#                 --in ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
#                 --ie ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
#                 --out ${TEMP}/apTvHyHyperspectralUnmixing_ISRA.tif double
#                 --ua isra )
                 
add_test(NAME apTvHyHyperspectralUnmixing_FCLS
         COMMAND otbTestDriver
                 --compare-image ${EPSILON_9}
                   ${BASELINE}/apTvHyHyperspectralUnmixing_FCLS.tif
                   ${TEMP}/apTvHyHyperspectralUnmixing_FCLS.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 HyperspectralUnmixing
                 $<TARGET_FILE_DIR:otbapp_HyperspectralUnmixing>
                 --in ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
                 --ie ${INPUTDATA}/Hyperspectral/synthetic/endmembers.tif
                 --out ${TEMP}/apTvHyHyperspectralUnmixing_FCLS.tif double
                 --ua fcls )
                 
add_test(NAME apTvHyHyperspectralUnmixing_VCA
         COMMAND otbTestDriver
                 --compare-image ${EPSILON_9}
                   ${BASELINE}/apTvHyHyperspectralUnmixing_VCA.tif
                   ${TEMP}/apTvHyHyperspectralUnmixing_VCA.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 HyperspectralUnmixing
                 $<TARGET_FILE_DIR:otbapp_HyperspectralUnmixing>
                 --in ${INPUTDATA}/Hyperspectral/synthetic/hsi_cube.tif
                 --ne 5 
                 --oe ${TEMP}/apTvHyHyperspectralUnmixing_VCA.tif double )

add_test(NAME apTvHyHyperspectralUnmixing_VCA_UCLS
         COMMAND otbTestDriver
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 HyperspectralUnmixing
                 $<TARGET_FILE_DIR:otbapp_HyperspectralUnmixing>
                 --in ${OTB_DATA_ROOT}/Input/Hyperspectral/synthetic/hsi_cube.tif
                 --out ${TEMP}/apTvHyHyperspectralUnmixing_ucls.tif double
                 --ne 5
                 --ua ucls )

add_test(NAME apTvHyHyperspectralUnmixing_VCA_FCLS
         COMMAND otbTestDriver
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 HyperspectralUnmixing
                 $<TARGET_FILE_DIR:otbapp_HyperspectralUnmixing>
                 --in ${OTB_DATA_ROOT}/Input/Hyperspectral/synthetic/hsi_cube.tif
                 --out ${TEMP}/apTvHyHyperspectralUnmixing_fcls.tif double
                 --ne 5
                 --ua fcls )

    