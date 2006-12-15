PROJECT(BasicFilters)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(LeeImageFilter LeeImageFilter.cxx )
#TARGET_LINK_LIBRARIES(LeeImageFilter OTBCommon OTBIO ITKCommon ITKIO ITKNumerics ITKBasicFilters ITKCommon ITKStatistics itkvnl_inst itkvnl_algo itkvnl itkvcl itknetlib itksys  ITKNrrdIO itkpng itktiff itkjpeg8 itkjpeg12 itkjpeg16 ITKMetaIO itkzlib ITKDICOMParser ITKEXPAT )
TARGET_LINK_LIBRARIES(LeeImageFilter OTBCommon OTBIO ITKCommon ITKIO ITKNumerics ITKBasicFilters ITKCommon ITKStatistics itkvnl_inst itkvnl_algo itkvnl itkvcl itksys  ITKNrrdIO itkpng itktiff itkjpeg8 itkjpeg12 itkjpeg16 ITKMetaIO itkzlib ITKDICOMParser ITKEXPAT )


