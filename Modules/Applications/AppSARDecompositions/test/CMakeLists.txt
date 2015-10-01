otb_module_test()
#----------- SARDecompositions TESTS ----------------

otb_test_application(NAME  apTvSARDecompositions
                     APP  SARDecompositions
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -decomp haa
							 -out ${TEMP}/apTvReciprocalHAlpha.tif
							 
                     )
                             
