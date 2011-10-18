

IF(OTB_DATA_USE_LARGEINPUT)
  SET(LARGEINPUT ${OTB_DATA_LARGEINPUT_ROOT} )
ENDIF(OTB_DATA_USE_LARGEINPUT)

set(TEST_DRIVER otbTestDriver
    --add-before-env ITK_AUTOLOAD_PATH $<TARGET_FILE_DIR:otbapp_ReadImageInfo> )

IF(OTB_DATA_USE_LARGEINPUT)

add_test(NAME apTvUtORthorectificationTest_UTM
         COMMAND otbTestDriver
                 --compare-image 0.0
                 ${BASELINE}/owTvOrthorectifTest_UTM.tif
                 ${TEMP}/owTvOrthorectifTest_UTM.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 OrthoRectification
                 $<TARGET_FILE_DIR:otbapp_OrthoRectification>
                 --in ${LARGEINPUT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
                 --out ${TEMP}/owTvOrthorectifTest_UTM.tif 
                 --dem ${INPUTDATA}/DEM/srtm_directory/
                 --outputs.ulx  374100.8
                 --outputs.uly  4829184.8
                 --outputs.sizex 500
                 --outputs.sizey 500
                 --outputs.spacingx  0.5
                 --outputs.spacingy  -0.5
                 --map utm
                 --gridspacing 4 # Spacing of the deformation field
                 )

ENDIF(OTB_DATA_USE_LARGEINPUT)


