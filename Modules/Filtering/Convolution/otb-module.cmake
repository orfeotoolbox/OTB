set(DOCUMENTATION "This module contains classes to compute an image which is
the convolution of the input image with a given kernel. Note that two methods are
used: classical convolution and convolution based on the Overlap-save algorithm.")

otb_module(OTBConvolution 
  DEPENDS
    OTBCommon
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
