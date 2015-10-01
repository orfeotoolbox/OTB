otb_module_test()
#----------- SARConvert TESTS ----------------

#1
otb_test_application(NAME  apTvSARConvertRecCoherency
                     APP  SARConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv msinclairtocoherency 
							 -outc ${TEMP}/apTvSARConvertRecCoherency.tif
							 
                     )
            
#2                            
otb_test_application(NAME  apTvSARConvertRecCovariance
                     APP  SARConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv msinclairtocovariance 
							 -outc ${TEMP}/apTvSARConvertRecCovariance.tif
							 
                     )

#3
otb_test_application(NAME  apTvSARConvertRecCirCovariance
                     APP  SARConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv msinclairtocircovariance 
							 -outc ${TEMP}/apTvSARConvertRecCirCovariance.tif
							 
                     )

#4
otb_test_application(NAME  apTvSARConvertRecCohToMueller
                     APP  SARConvert
                     OPTIONS 
							 -inc ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
							 -conv mcoherencytomueller
							 -outf ${TEMP}/apTvSARConvertRecCohToMueller.tif
							 
                     )
                             
                             
#5
otb_test_application(NAME  apTvSARConvertRecCovToCohDeg
                     APP  SARConvert
                     OPTIONS 
							 -inc ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
							 -conv mcovariancetocoherencydegree
							 -outc ${TEMP}/apTvSARConvertRecCovToCohDeg.tif
							 
                     )
                             

#6 - interleaved (failed)
otb_test_application(NAME  apTvSARConvertRecCovToRecCoh
                     APP  SARConvert
                     OPTIONS 
							 -inc ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
							 -conv mcovariancetocoherency
							 -outc ${TEMP}/apTvSARConvertRecCovToRecCoh.tif
							 
                     )

#7 - interleaved (failed)
otb_test_application(NAME  apTvSARConvertRecLinCovToRecCirCov
                     APP  SARConvert
                     OPTIONS 
							 -inc ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
							 -conv mlinearcovariancetocircularcovariance
							 -outc ${TEMP}/apTvSARConvertRecCovToRecCoh.tif
							 
                     )
                             
                             
#8 - interleaved (failed)
otb_test_application(NAME  apTvSARConvertMuellerToRecCov
                     APP  SARConvert
                     OPTIONS 
							 -inf ${BASELINE}/saTvSinclairImageFilter_SinclairToMueller.tif
							 -conv muellertomcovariance
							 -outc ${TEMP}/apTvSARConvertMuellerToRecCov.tif
							 
                     )


#9
otb_test_application(NAME  apTvSARConvertBiSincToCoherency
                     APP  SARConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invh ${INPUTDATA}/RSAT_imageryC_VH.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv bsinclairtocoherency
							 -outc ${TEMP}/apTvSARConvertBiSincToCoherency.tif
							 
                     )
                             
                             
#10
otb_test_application(NAME  apTvSARConvertBiSincToCovariance
                     APP  SARConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invh ${INPUTDATA}/RSAT_imageryC_VH.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv bsinclairtocovariance
							 -outc ${TEMP}/apTvSARConvertBiSincToCovariance.tif
							 
                     )
                             
#11
otb_test_application(NAME  apTvSARConvertBiSincToCirCovariance
                     APP  SARConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invh ${INPUTDATA}/RSAT_imageryC_VH.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv bsinclairtocircovariance
							 -outc ${TEMP}/apTvSARConvertBiSincToCirCovariance.tif
							 
                     )
   
                             
#12 - interleaved (failed)
otb_test_application(NAME  apTvSARConvertSincToMueller
                     APP  SARConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invh ${INPUTDATA}/RSAT_imageryC_HV.tif #monostatic
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv sinclairtomueller
							 -outf ${TEMP}/apTvSARConvertSincToMueller.tif
							 
                     )
                             
                             
#13 
otb_test_application(NAME  apTvSARConvertMuellerToPolDeGPow
                     APP  SARConvert
                     OPTIONS 
							 -inf ${BASELINE}/saTvSinclairImageFilter_SinclairToMueller.tif
							 -conv muellertopoldegandpower
							 -outf ${TEMP}/apTvSARConvertMuellerToPolDeGPow.tif
							 
                     )
