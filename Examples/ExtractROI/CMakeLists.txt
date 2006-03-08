PROJECT(StartExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(ExtractROI ExtractROI.cxx )
TARGET_LINK_LIBRARIES(ExtractROI OTBCommon OTBIO ITKCommon ITKIO ITKNumerics ITKBasicFilters ITKCommon ITKStatistics itkvnl_inst itkvnl_algo itkvnl itkvcl itknetlib itksys  ITKNrrdIO itkpng itktiff itkjpeg8 itkjpeg12 itkjpeg16 ITKMetaIO itkzlib ITKDICOMParser ITKEXPAT )

#ADD_EXECUTABLE(TestFltk TestFltk.cxx )
#TARGET_LINK_LIBRARIES(TestFltk fltk fltk_forms fltk_gl fltk_images )

