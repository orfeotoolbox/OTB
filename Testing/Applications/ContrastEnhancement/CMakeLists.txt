#--- MonoChannel Histogram Equalization ---#
#OTB_TEST_APPLICATION(NAME  apTvCeMonoChannelHistogramEqualizeTest
#                     APP  MonoChannelHistogramEqualize
#                     OPTIONS -in ${INPUTDATA}/ToulouseQuickBird_Extrait_1500_3750.tif
#                             -out ${TEMP}/apTvCeMonoChannelHistogramEqualizeTest.tif
#                     VALID   --compare-image ${NOTOL}
#                             ${BASELINE}/apTvCeMonoChannelHistogramEqualizeTest.tif
#                             ${TEMP}/apTvCeMonoChannelHistogramEqualizeTest.tif)

#--- Color Histogram Equalization ---#
#OTB_TEST_APPLICATION(NAME  apTvCeColorHistogramEqualizeTest
#                     APP  ColorHistogramEqualize
#                     OPTIONS -in ${INPUTDATA}/poupees.tif
#                             -type uint8
#                             -out ${TEMP}/apTvCeColorHistogramEqualizeTest.tif
#                     VALID   --compare-image ${NOTOL}
#                             ${BASELINE}/apTvCeColorHistogramEqualizeTest.tif
#                             ${TEMP}/apTvCeColorHistogramEqualizeTest.tif)

#--- ColorSpace Transform YUV2RGB Equalization ---#
#OTB_TEST_APPLICATION(NAME  apTvCeColorSpaceTransformTest
#                     APP  ColorSpaceTransform
#                    OPTIONS -in ${INPUTDATA}/poupees.tif
#                             -type uint8
#                             -out ${TEMP}/apTvCeColorSpaceTransformTest.tif
#                     VALID   --compare-image ${NOTOL}
#                             ${BASELINE}/apTvCeColorSpaceTransformTest.tif
#                             ${TEMP}/apTvCeColorSpaceTransformTest.tif)

