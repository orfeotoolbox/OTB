include(WrapperMacros)

OTB_CREATE_APPLICATION(NAME Addition        SOURCES otbAddition.cxx)
OTB_CREATE_APPLICATION(NAME AllocateOutput  SOURCES otbAllocateOutput.cxx LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)
OTB_CREATE_APPLICATION(NAME CopyInput       SOURCES otbCopyInput.cxx LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)
OTB_CREATE_APPLICATION(NAME InternalReader  SOURCES otbInternalReader.cxx LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)
OTB_CREATE_APPLICATION(NAME TestApplication SOURCES otbTestApplication.cxx LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)
