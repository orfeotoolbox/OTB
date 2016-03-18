otb_module_test()
#----------- SARDecompositions TESTS ----------------

otb_test_application(NAME  apTvSARDecompositionsHAAlpha
                     APP  SARDecompositions
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -decomp haa
							 -out ${TEMP}/apTvReciprocalHAlpha.tif
							 
                     )
                         
                             
otb_test_application(NAME  apTvSARDecompositionsBarnes
                     APP  SARDecompositions
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -decomp barnes
							 -out ${TEMP}/apTvReciprocalBarnes.tif
							 
                     )
                     
otb_test_application(NAME  apTvSARDecompositionsHuynen
                     APP  SARDecompositions
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -decomp barnes
							 -out ${TEMP}/apTvReciprocalHuynen.tif
							 
                     )
