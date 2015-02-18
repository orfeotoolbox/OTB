set(DOCUMENTATION "This module contains classes that ease the implementation of
applications (see chapter 'How to write an application' of the SoftwareGuide).")

otb_module(OTBApplicationEngine 
  DEPENDS
    OTBVectorDataBase
    OTBImageManipulation
    OTBImageIO
    OTBProjection
    OTBTinyXML
    OTBVectorDataIO
    OTBTransform
    OTBCommon
    OTBImageBase
    OTBBoost
    OTBOSSIMAdapters
    OTBObjectList
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
