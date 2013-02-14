
OTB_CREATE_APPLICATION(NAME           StereoSensorModelToElevationMap
                       SOURCES        otbStereoSensorModelToElevationMap.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           FineRegistration
                       SOURCES        otbFineRegistration.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           StereoRectificationGridGenerator
                       SOURCES        otbStereoRectificationGridGenerator.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           BlockMatching
                       SOURCES        otbBlockMatching.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           DisparityMapToElevationMap
                       SOURCES        otbDisparityMapToElevationMap.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           StereoFramework
                       SOURCES        otbStereoFramework.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           GeneratePlyFile
                       SOURCES        otbGeneratePlyFile.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)
