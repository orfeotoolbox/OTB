otb_module_test()
#----------- PolarSynth TESTS ----------------

otb_test_application(NAME  apTvSARPolarSynth
                     APP  SARPolarSynth
                     OPTIONS -in ${INPUTDATA}/RSAT2_AltonaExtract_1000_1000_100_100.hdr
               	             -out ${TEMP}/resApMultiPolarimetricSynthesis1.tif
                             -psii 10.0 
                             -khii 0.0
                             -psir 0.0
                             -khir 0.0
                     )
                             
