otb_module_test()
#----------- BinaryMorphologicalOperation TESTS ----------------
otb_test_application(NAME  apTvFEBinaryMorphologicalOperation
                     APP  BinaryMorphologicalOperation
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN_Mask.tif
                             -channel 1
                             -structype ball
                             -structype.ball.xradius 10
                             -filter opening
                             -filter.opening.foreval 1.0
                             -filter.opening.backval 0.0
                             -out ${TEMP}/apTvFEBinaryMorphologicalOperation.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/apTvFEBinaryMorphologicalOperation.tif
                 		     ${TEMP}/apTvFEBinaryMorphologicalOperation.tif)


#----------- GrayScaleMorphologicalOperation TESTS ----------------
otb_test_application(NAME  apTvFEGrayScaleMorphologicalOperation
                     APP  GrayScaleMorphologicalOperation
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -channel 1
                             -structype ball
                             -structype.ball.xradius 10
                             -filter opening
                             -out ${TEMP}/apTvFEGrayScaleMorphologicalOperation.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/apTvFEGrayScaleMorphologicalOperation.tif
                 		     ${TEMP}/apTvFEGrayScaleMorphologicalOperation.tif)

