otb_module_test()
otb_test_application(NAME apTvDomainTransform_wav_db20_fwd
  APP  DomainTransform
  OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  -mode wavelet
  -mode.wavelet.form db20
  -mode.wavelet.nlevels 2
  -direction forward
  -out ${TEMP}/apTvDomainTransform_wav_db20_fwd.tif
  )

otb_test_application(NAME apTvDomainTransform_wav_haar_inv
  APP  DomainTransform
  OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  -mode wavelet
  -mode.wavelet.form haar
  -direction inverse
  -out ${TEMP}/apTvDomainTransform_wav_haar_inv.tif
  )

otb_test_application(NAME apTvDomainTransform_fft_shift_fwd
  APP  DomainTransform
  OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  -mode fft
  -mode.fft.shift true
  -direction forward
  -out ${TEMP}/apTvDomainTransform_fft_shift_fwd.tif
  )


otb_test_application(NAME apTvDomainTransform_fft_inv
  APP  DomainTransform
  OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  -mode fft
  -direction inverse
  -out ${TEMP}/apTvDomainTransform_fft_shift_inv.tif
  )

