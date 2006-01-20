IF( NOT OTB_DISABLE_CXX_TESTING )

SET(BASELINE ${OTB_DATA_ROOT}/Baseline/Images)
SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTBTesting_BINARY_DIR}/Temporary)

ADD_TEST(tuInInstallationTest ${CXX_TEST_PATH}/otbInstallationTest      
  ${INPUTDATA}/poupees.jpg )

ADD_EXECUTABLE(otbInstallationTest otbInstallationTest.cxx )
#TARGET_LINK_LIBRARIES(otbInstallationTest OTBCommon ITKCommon ITKIO ITKNumerics ITKBasicFilters ITKCommon ITKStatistics itkvnl_inst itkvnl_algo itkvnl itkvcl itknetlib itksys  ITKNrrdIO itkpng itktiff itkjpeg8 itkjpeg12 itkjpeg16 ITKMetaIO itkzlib ITKDICOMParser ITKEXPAT )

#TARGET_LINK_LIBRARIES(otbInstallationTest OTBCommon )
TARGET_LINK_LIBRARIES(otbInstallationTest OTBCommon ${CAI_LIBRARY} ${TIFF_LIBRARY} vtkRendering )
#TARGET_LINK_LIBRARIES(otbInstallationTest OTBCommon vtkCommon fltk fltk_forms fltk_gl fltk_images )




#TARGET_LINK_LIBRARIES(TestFltk fltk fltk_forms fltk_gl fltk_images )

ENDIF( NOT OTB_DISABLE_CXX_TESTING )

