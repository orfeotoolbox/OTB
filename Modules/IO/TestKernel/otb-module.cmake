set(DOCUMENTATION "This module is concerned with running the tests and
submitting them to the dashboard. For each class, at minimum there is a test which tries
to instantiate it and another one which uses the class. The output of each test
(image, text file, binary file) is controlled against a baseline to make sure that
the result hasn't changed.")

otb_module(OTBTestKernel 
  DEPENDS
    OTBGdalAdapters
    OTBImageIO
    OTBImageBase
    OTBCommon
    OTBGDAL
    OTBOSSIMAdapters
    OTBImageManipulation
  
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
