otb_module_test()
#----------- ComputeModulusAndPhase TESTS ----------------
otb_test_application(NAME apTvUtComputeModulusAndPhase_2inputs
                     APP  ComputeModulusAndPhase
                     OPTIONS -nbinput two
                             -nbinput.two.re ${INPUTDATA}/monobandFloat.tif
                             -nbinput.two.im ${INPUTDATA}/monobandFloat2.tif
                             -mod ${TEMP}/apTvUtMod2inputs.tif
                             -pha ${TEMP}/apTvUtPha2inputs.tif
                     VALID   --compare-n-images ${NOTOL} 2
                             ${BASELINE}/Mod_monobandFloat.tif
                             ${TEMP}/apTvUtMod2inputs.tif
                             ${BASELINE}/Pha_monobandFloat.tif
                             ${TEMP}/apTvUtPha2inputs.tif
                     )

otb_test_application(NAME apTvUtComputeModulusAndPhase_1inputComplex
                     APP  ComputeModulusAndPhase
                     OPTIONS -nbinput one
                             -nbinput.one.in ${INPUTDATA}/monobandComplexFloat.tif
                             -mod ${TEMP}/apTvUtMod1inputComplex.tif
                             -pha ${TEMP}/apTvUtPha1inputComplex.tif
                     VALID   --compare-n-images ${NOTOL} 2
                             ${BASELINE}/Mod_monobandComplexFloat.tif
                             ${TEMP}/apTvUtMod1inputComplex.tif
                             ${BASELINE}/Pha_monobandComplexFloat.tif
                             ${TEMP}/apTvUtPha1inputComplex.tif
                     )

otb_test_application(NAME apTvUtComputeModulusAndPhase_1input2bands
                     APP  ComputeModulusAndPhase
                     OPTIONS -nbinput one
                             -nbinput.one.in ${INPUTDATA}/multibandFloat_2bands.tif
                             -mod ${TEMP}/apTvUtMod1input2bands.tif
                             -pha ${TEMP}/apTvUtPha1input2bands.tif
                     VALID   --compare-n-images ${NOTOL} 2
                             ${BASELINE}/Mod_multibandFloat_2bands.tif
                             ${TEMP}/apTvUtMod1input2bands.tif
                             ${BASELINE}/Pha_multibandFloat_2bands.tif
                             ${TEMP}/apTvUtPha1input2bands.tif
                     )
