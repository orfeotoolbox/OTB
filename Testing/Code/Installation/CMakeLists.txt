IF( NOT OTB_DISABLE_CXX_TESTING )

ADD_TEST(otbInstallationTest ${CXX_TEST_PATH}/otbInstallationTest )

ADD_EXECUTABLE(otbInstallationTest otbInstallationTest.cxx )
#TARGET_LINK_LIBRARIES(otbInstallationTest OTBCommon ITKCommon ITKIO ITKNumerics ITKBasicFilters ITKCommon ITKStatistics itkvnl_inst itkvnl_algo itkvnl itkvcl itknetlib itksys  ITKNrrdIO itkpng itktiff itkjpeg8 itkjpeg12 itkjpeg16 ITKMetaIO itkzlib ITKDICOMParser ITKEXPAT )

#TARGET_LINK_LIBRARIES(otbInstallationTest OTBCommon )
TARGET_LINK_LIBRARIES(otbInstallationTest OTBCommon ${CAI_LIBRARY} ${TIFF_LIBRARY} vtkRendering )
#TARGET_LINK_LIBRARIES(otbInstallationTest OTBCommon vtkCommon fltk fltk_forms fltk_gl fltk_images )




#TARGET_LINK_LIBRARIES(TestFltk fltk fltk_forms fltk_gl fltk_images )

ENDIF( NOT OTB_DISABLE_CXX_TESTING )

