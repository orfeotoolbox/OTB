otb_module_test()
#----------- SARPolarMatrixConvert TESTS ----------------

#1
otb_test_application(NAME  apTvSARPolarMatrixConvertRecCoherency
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv msinclairtocoherency 
							 -outc ${TEMP}/apTvSARPolarMatrixConvertRecCoherency.tif
							 
                     )
            
#2                            
otb_test_application(NAME  apTvSARPolarMatrixConvertRecCovariance
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv msinclairtocovariance 
							 -outc ${TEMP}/apTvSARPolarMatrixConvertRecCovariance.tif
							 
                     )

#3
otb_test_application(NAME  apTvSARPolarMatrixConvertRecCirCovariance
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv msinclairtocircovariance 
							 -outc ${TEMP}/apTvSARPolarMatrixConvertRecCirCovariance.tif
							 
                     )

#4
otb_test_application(NAME  apTvSARPolarMatrixConvertRecCohToMueller
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inc ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
							 -conv mcoherencytomueller
							 -outf ${TEMP}/apTvSARPolarMatrixConvertRecCohToMueller.tif
							 
                     )
                             
                             
#5
otb_test_application(NAME  apTvSARPolarMatrixConvertRecCovToCohDeg
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inc ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
							 -conv mcovariancetocoherencydegree
							 -outc ${TEMP}/apTvSARPolarMatrixConvertRecCovToCohDeg.tif
							 
                     )
                             

#6 - interleaved (failed)
otb_test_application(NAME  apTvSARPolarMatrixConvertRecCovToRecCoh
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inc ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
							 -conv mcovariancetocoherency
							 -outc ${TEMP}/apTvSARPolarMatrixConvertRecCovToRecCoh.tif
							 
                     )

#7 - interleaved (failed)
otb_test_application(NAME  apTvSARPolarMatrixConvertRecLinCovToRecCirCov
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inc ${BASELINE}/saTvSinclairImageFilter_SinclairToReciprocalCovariance.tif
							 -conv mlinearcovariancetocircularcovariance
							 -outc ${TEMP}/apTvSARPolarMatrixConvertRecCovToRecCoh.tif
							 
                     )
                             
                             
#8 - interleaved (failed)
otb_test_application(NAME  apTvSARPolarMatrixConvertMuellerToRecCov
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inf ${BASELINE}/saTvSinclairImageFilter_SinclairToMueller.tif
							 -conv muellertomcovariance
							 -outc ${TEMP}/apTvSARPolarMatrixConvertMuellerToRecCov.tif
							 
                     )


#9
otb_test_application(NAME  apTvSARPolarMatrixConvertBiSincToCoherency
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invh ${INPUTDATA}/RSAT_imageryC_VH.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv bsinclairtocoherency
							 -outc ${TEMP}/apTvSARPolarMatrixConvertBiSincToCoherency.tif
							 
                     )
                             
                             
#10
otb_test_application(NAME  apTvSARPolarMatrixConvertBiSincToCovariance
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invh ${INPUTDATA}/RSAT_imageryC_VH.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv bsinclairtocovariance
							 -outc ${TEMP}/apTvSARPolarMatrixConvertBiSincToCovariance.tif
							 
                     )
                             
#11
otb_test_application(NAME  apTvSARPolarMatrixConvertBiSincToCirCovariance
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invh ${INPUTDATA}/RSAT_imageryC_VH.tif
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv bsinclairtocircovariance
							 -outc ${TEMP}/apTvSARPolarMatrixConvertBiSincToCirCovariance.tif
							 
                     )
   
                             
#12 - interleaved (failed)
otb_test_application(NAME  apTvSARPolarMatrixConvertSincToMueller
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inhh ${INPUTDATA}/RSAT_imageryC_HH.tif
							 -inhv ${INPUTDATA}/RSAT_imageryC_HV.tif
							 -invh ${INPUTDATA}/RSAT_imageryC_HV.tif #monostatic
							 -invv ${INPUTDATA}/RSAT_imageryC_VV.tif
							 -conv sinclairtomueller
							 -outf ${TEMP}/apTvSARPolarMatrixConvertSincToMueller.tif
							 
                     )
                             
                             
#13 
otb_test_application(NAME  apTvSARPolarMatrixConvertMuellerToPolDeGPow
                     APP  SARPolarMatrixConvert
                     OPTIONS 
							 -inf ${BASELINE}/saTvSinclairImageFilter_SinclairToMueller.tif
							 -conv muellertopoldegandpower
							 -outf ${TEMP}/apTvSARPolarMatrixConvertMuellerToPolDeGPow.tif
							 
                     )
