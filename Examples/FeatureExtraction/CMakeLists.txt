PROJECT(FeatureExtraction)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(AlignmentsExample AlignmentsExample.cxx )
TARGET_LINK_LIBRARIES(AlignmentsExample OTBCommon OTBIO ITKCommon ITKIO ITKNumerics ITKBasicFilters ITKCommon ITKStatistics itkvnl_inst itkvnl_algo itkvnl itkvcl itknetlib itksys  ITKNrrdIO itkpng itktiff itkjpeg8 itkjpeg12 itkjpeg16 ITKMetaIO itkzlib ITKDICOMParser ITKEXPAT )

ADD_EXECUTABLE(TouziEdgeDetectorExample TouziEdgeDetectorExample.cxx )
TARGET_LINK_LIBRARIES(TouziEdgeDetectorExample OTBCommon OTBIO ITKCommon ITKIO ITKNumerics ITKBasicFilters ITKCommon ITKStatistics itkvnl_inst itkvnl_algo itkvnl itkvcl itknetlib itksys  ITKNrrdIO itkpng itktiff itkjpeg8 itkjpeg12 itkjpeg16 ITKMetaIO itkzlib ITKDICOMParser ITKEXPAT )

ADD_EXECUTABLE(HarrisExample HarrisExample.cxx )
TARGET_LINK_LIBRARIES(HarrisExample OTBCommon OTBIO ITKCommon ITKIO ITKNumerics ITKBasicFilters ITKCommon ITKStatistics itkvnl_inst itkvnl_algo itkvnl itkvcl itknetlib itksys  ITKNrrdIO itkpng itktiff itkjpeg8 itkjpeg12 itkjpeg16 ITKMetaIO itkzlib ITKDICOMParser ITKEXPAT )

ADD_EXECUTABLE(ComplexMomentImageExample ComplexMomentImageExample.cxx )
TARGET_LINK_LIBRARIES(ComplexMomentImageExample OTBCommon OTBIO ITKCommon ITKIO ITKNumerics ITKBasicFilters ITKCommon ITKStatistics itkvnl_inst itkvnl_algo itkvnl itkvcl itknetlib itksys  ITKNrrdIO itkpng itktiff itkjpeg8 itkjpeg12 itkjpeg16 ITKMetaIO itkzlib ITKDICOMParser ITKEXPAT )


