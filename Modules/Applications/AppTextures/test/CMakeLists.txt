otb_module_test()
#----------- HaralickTextureExtraction TESTS ----------------
otb_test_application(NAME  apTvFEHaralickTextureExtraction
                     APP  HaralickTextureExtraction
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -channel 1
                             -texture simple
                             -out ${TEMP}/apTvFEHaralickTextureExtraction.tif
                             -parameters.min 127
                             -parameters.max 1578
                     VALID   --compare-image ${EPSILON_15}
                   			 ${BASELINE}/apTvFEHaralickTextureExtraction.tif
                 		     ${TEMP}/apTvFEHaralickTextureExtraction.tif)


#----------- SFSTextureExtraction TESTS ----------------
otb_test_application(NAME  apTvFESFSTextureExtraction
                     APP  SFSTextureExtraction
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -channel 1
                             -out ${TEMP}/apTvFESFSTextureExtraction.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/apTvFESFSTextureExtraction.tif
                 		     ${TEMP}/apTvFESFSTextureExtraction.tif)

