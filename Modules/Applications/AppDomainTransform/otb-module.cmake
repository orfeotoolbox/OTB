set(DOCUMENTATION "Application to perform Wavelet and Fourier transform. This application is not streamed")

otb_module(OTBAppDomainTransform
DEPENDS
  OTBWavelet
  OTBApplicationEngine

  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine

  DESCRIPTION
    "${DOCUMENTATION}"
)

