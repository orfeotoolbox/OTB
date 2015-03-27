otb_module_test()

#----------- BandMathX TESTS ----------------
otb_test_application(NAME apTvUtBandMathX
                     APP  BandMathX
                     OPTIONS -il ${INPUTDATA}/poupees_sub_c1.png
                                  ${INPUTDATA}/poupees_sub_c2.png
                                  ${INPUTDATA}/poupees_sub.png
                             -out ${TEMP}/apTvUtBandMathXOutput.tif
                             -incontext ${INPUTDATA}/apTvUtExportBandMathX.txt
                     VALID   --compare-image ${NOTOL}
                             ${INPUTDATA}/apTvUtBandMathOutput.tif
                             ${TEMP}/apTvUtBandMathXOutput.tif)

