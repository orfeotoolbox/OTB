
OTB_CREATE_APPLICATION(NAME           ExtractROI
                       SOURCES        otbExtractROI.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           Rescale
                       SOURCES        otbRescale.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           Smoothing
                       SOURCES        otbSmoothing.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           ReadImageInfo
                       SOURCES        otbReadImageInfo.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           MultiResolutionPyramid
                       SOURCES        otbMultiResolutionPyramid.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           ConcatenateImages
                       SOURCES        otbConcatenateImages.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           Convert
                       SOURCES        otbConvert.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           BandMath
                       SOURCES        otbBandMath.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           SplitImage
                       SOURCES        otbSplitImage.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           KmzExport
                       SOURCES        otbKmzExport.cxx
                       LINK_LIBRARIES OTBIO)

OTB_CREATE_APPLICATION(NAME           OSMDownloader
                       SOURCES        otbOSMDownloader.cxx
                       LINK_LIBRARIES OTBIO)

OTB_CREATE_APPLICATION(NAME           ConcatenateVectorData
                       SOURCES        otbConcatenateVectorData.cxx
                       LINK_LIBRARIES OTBIO)

OTB_CREATE_APPLICATION(NAME           CompareImages
                       SOURCES        otbCompareImages.cxx
                       LINK_LIBRARIES OTBIO)

OTB_CREATE_APPLICATION(NAME           VectorDataSetField
                       SOURCES        otbVectorDataSetField.cxx
                       LINK_LIBRARIES OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           VectorDataTransform
                       SOURCES        otbVectorDataTransform.cxx
                       LINK_LIBRARIES OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           VectorDataExtractROIApplication
                       SOURCES        otbVectorDataExtractROIApplication.cxx
                       LINK_LIBRARIES OTBBasicFilters)

#OTB_CREATE_APPLICATION(NAME           DEMConvert
#                       SOURCES        otbDEMConvert.cxx
#                       LINK_LIBRARIES OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           Quicklook
                       SOURCES        otbQuicklook.cxx
                       LINK_LIBRARIES OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           ColorMapping
                       SOURCES        otbColorMapping.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters;OTBFeatureExtraction)

OTB_CREATE_APPLICATION(NAME           PixelValue
                       SOURCES        otbPixelValue.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon)

OTB_CREATE_APPLICATION(NAME           TileFusion
                       SOURCES        otbTileFusion.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon)
