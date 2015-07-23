otb_module_test()
#----------- SARConvert TESTS ----------------

otb_test_application(NAME  apTvSARConvert
                     APP  SARConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv msinclairtocoherency 
							 -outc ${TEMP}/apTvConvCoherency.tif
							 
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/apTvReciprocalHAlpha.tif
                             ${TEMP}/apTvConvCoherency.tif)
                             
