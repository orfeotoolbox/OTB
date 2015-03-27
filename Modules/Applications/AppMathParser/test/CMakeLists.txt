otb_module_test()

#----------- BandMath TESTS ----------------
otb_test_application(NAME apTvUtBandMath
                     APP  BandMath
                     OPTIONS -il ${INPUTDATA}/poupees_sub_c1.png
                                  ${INPUTDATA}/poupees_sub_c2.png
                                  ${INPUTDATA}/poupees_sub.png
                             -out ${TEMP}/apTvUtBandMathOutput.tif
                             -exp "cos(im1b1)+im2b1*im3b1-im3b2+ndvi(im3b3,im3b4)"
                     VALID   --compare-image ${NOTOL}
                             ${INPUTDATA}/apTvUtBandMathOutput.tif
                             ${TEMP}/apTvUtBandMathOutput.tif)

