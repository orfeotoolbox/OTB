otb_module_test()
otb_test_application(NAME apTvDomainTransform_wav_db20_fwd
  APP  DomainTransform
  OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  -mode wavelet
  -mode.wavelet.form db20
  -mode.wavelet.nlevels 2
  -dir fwd
  -out ${TEMP}/apTvDomainTransform_wav_db20_fwd.tif
  )

otb_test_application(NAME apTvDomainTransform_wav_haar_inv
  APP  DomainTransform
  OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  -mode wavelet
  -mode.wavelet.form haar
  -dir inv
  -out ${TEMP}/apTvDomainTransform_wav_haar_inv.tif
  )

otb_test_application(NAME apTvDomainTransform_fft_shift_fwd
  APP  DomainTransform
  OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  -mode fft
  -mode.fft.shift true
  -dir fwd
  -out ${TEMP}/apTvDomainTransform_fft_shift_fwd.tif
  )

otb_test_application(NAME apTvDomainTransform_fft_fwd
  APP  DomainTransform
  OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  -mode fft
  -dir fwd
  -out ${TEMP}/apTvDomainTransform_fft_fwd.tif
  )

otb_test_application(NAME apTvDomainTransform_fft_shift_inv
  APP  DomainTransform
  OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  -mode fft
  -mode.fft.shift true
  -dir inv
  -out ${TEMP}/apTvDomainTransform_fft_shift_inv.tif
  )

otb_test_application(NAME apTvDomainTransform_fft_inv
  APP  DomainTransform
  OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  -mode fft
  -dir inv
  -out ${TEMP}/apTvDomainTransform_fft_inv.tif
  )
		   
