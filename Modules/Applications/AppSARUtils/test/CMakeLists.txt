otb_module_test()
#----------- ComputeModulusAndPhase TESTS ----------------
otb_test_application(NAME apTvUtComputeModulusAndPhase_1inputComplex
                     APP  ComputeModulusAndPhase
                     OPTIONS -in ${INPUTDATA}/monobandComplexFloat.tif
                             -modulus ${TEMP}/apTvUtMod1inputComplex.tif
                             -phase ${TEMP}/apTvUtPha1inputComplex.tif
                     VALID   --compare-n-images ${EPSILON_6} 2
                             ${BASELINE}/Mod_monobandComplexFloat.tif
                             ${TEMP}/apTvUtMod1inputComplex.tif
                             ${BASELINE}/Pha_monobandComplexFloat.tif
                             ${TEMP}/apTvUtPha1inputComplex.tif
                     )
